#include "syntaxAnalyzer.h"
#include "myException.h"

SyntaxAnalyzer::SyntaxAnalyzer(TokenStream& stream)
{
	stream.initIter();
	root = buildBlock(stream);
}

SyntaxAnalyzer::~SyntaxAnalyzer()
{
	if (root != NULL){
		delete root;
	}
	for (map<string, ast::Type*>::iterator it = types.begin(); it != types.end(); it++){
		delete it->second;
	}
}

ast::Type* SyntaxAnalyzer::getType(const string& s){
	if (types.find(s) == types.end()){
		types[s] = new ast::PrimitiveType(s);
	}
	return types[s];
}

ast::Term* SyntaxAnalyzer::buildBlock(TokenStream& stream){
	while (stream.hasNext()){
		Token token = stream.next();
		switch (token.type){
		case Token::TYPE:
			buildTypeDef(stream);
			break;
		case Token::FUNC:
			return buildFuncDef(stream);
		case Token::COM:
			break;
		default:
			stream.back();
			return buildExpr(stream);
		}
	}
	return NULL;
}

void SyntaxAnalyzer::buildTypeDef(TokenStream& stream){
	Token token = stream.next();
	if (token.type != Token::ID){
		throw syntax_error(token.name, token.line, "Should be an ID");
	}
	string typeId = token.name;
	ast::SumType* sumType = new ast::SumType();
	types[typeId] = sumType;

	token = stream.next();
	if (token.type == Token::COLON){
		token = stream.next();	// base type
		// Do something about the base&sub class
		token = stream.next();	// '='
	}
	if (token.type != Token::EQL){
		throw syntax_error(token.name, token.line, "Should be '='");
	}
	token = stream.next();
	if (token.type != Token::OR){
		throw syntax_error(token.name, token.line, "Should be '|'");
	}
	do{
		token = stream.next();
		if (token.type != Token::ID && token.type != Token::NIL){
			throw syntax_error(token.name, token.line, "Expected nil or identifier");
		}
		std::pair<ast::Type*, std::string> pair = { NULL, token.name };

		token = stream.next();
		if (token.type != Token::COLON){
			throw syntax_error(token.name, token.line, "Should be ':'");
		}

		ast::Type** curr = &(pair.first);
		string arg;
		while (true){
			token = stream.next();	// type id
			if (token.type != Token::ID && token.type != Token::NAT && token.type != Token::BOOL){
				throw syntax_error(token.name, token.line, "Expected type id");
			}
			arg = token.name;

			token = stream.next();	// ->
			if (token.type != Token::PRODUCT){
				stream.back();
				if (arg != typeId){
					throw syntax_error(arg, token.line, "Expected " + typeId);
				}
				if (pair.first == NULL){
					pair.first = getType("unit");
				}
				break;
			}
			// add arg to tree
			if (*curr == NULL){
				*curr = getType(arg);
			}
			else{
				*curr = new ast::ProductType(*curr, getType(arg));
				curr = &(((ast::ProductType*)*curr)->y);
			}
		}
		sumType->types.push_back(pair);

		token = stream.next();			// if '|', go on, else stop
	} while (token.type == Token::OR);
	stream.back();		// put back the token
	// do something with the type
}

// return a type, possible primitive or algebraic or production of them
ast::Type* SyntaxAnalyzer::buildFuncType(TokenStream& stream){
	Token token = stream.next();	// type id
	if (token.type != Token::ID && token.type != Token::NAT && token.type != Token::BOOL){
		throw syntax_error(token.name, token.line, "Should be a type id");
	}
	ast::Type* type = getType(token.name);
	token = stream.next();
	if (token.type != Token::PRODUCT){
		stream.back();
		return type;
	}
	return new ast::FunctionType(type, buildFuncType(stream));
}

// return the deepest term reference
ast::Term* SyntaxAnalyzer::buildFuncDef(TokenStream& stream){
	Token token = stream.next();	// func id
	if (token.type != Token::ID){
		throw syntax_error(token.name, token.line, "Should be an ID");
	}
	string funcId = token.name;

	ast::Type* type = NULL;
	ast::Term* term = NULL;

	vector<string> ids;
	vector<ast::Type*> tps;

	token = stream.next();
	do{
		switch (token.type){
		case Token::LPAR:{
			token = stream.next();	// param id
			if (token.type != Token::ID){
				throw syntax_error(token.name, token.line, "Expected param id");
			}
			string param = token.name;
			token = stream.next();	// ':'
			if (token.type != Token::COLON){
				throw syntax_error(token.name, token.line, "Should be ':'");
			}
			ids.push_back(param);
			tps.push_back(buildFuncType(stream));

			token = stream.next();	// ')'
			if (token.type != Token::RPAR){
				throw syntax_error(token.name, token.line, "Should b ')'");
			}
			break;
		}
		case Token::COLON:{		// ':' return type
			type = buildFuncType(stream);
			break;
		}
		default:
			throw syntax_error(token.name, token.line, "Expected parameter or return type after func id");
		}
		token = stream.next();	// '='
	} while (token.type != Token::EQL);

	// func definition expression
	term = buildExpr(stream);

	for (int i = ids.size() - 1; i >= 0; i--){
		type = new ast::FunctionType(tps[i], type);
		term = new ast::Abstraction(ids[i], tps[i], term);
	}
	ast::Abstraction* func = new ast::Abstraction(funcId, type, buildBlock(stream));

	return new ast::Application(func, term);
}

ast::Term* SyntaxAnalyzer::buildFuncDesig(TokenStream& stream){
	Token token = stream.next();	// funct id
	string funcId = token.name;
	token = stream.next();
	if (token.type == Token::RPAR){
		return new ast::Reference(funcId);
	}
	stream.back();
	ast::Application *term = new ast::Application(new ast::Reference(funcId), buildExpr(stream));
	token = stream.next();
	while (token.type != Token::RPAR){
		stream.back();
		term = new ast::Application(term, buildExpr(stream));
		token = stream.next();
	}
	return term;
}

ast::Term* SyntaxAnalyzer::buildFactor(TokenStream& stream){
	Token token = stream.next();
	ast::Term* factor = NULL;
	switch (token.type){
	case Token::MATCH:
	case Token::NIL:
	case Token::TRUE:
	case Token::FALSE:
	case Token::ID:
	case Token::INT:
	case Token::ADD:
	case Token::SUB:
	case Token::MUL:
	case Token::DIV:
	case Token::CMPE:
	case Token::CMPG:
	case Token::CMPGE:
	case Token::CMPL:
	case Token::CMPLE:
	case Token::CMPNE:
		factor = new ast::Reference(token.name);
		break;
	case Token::LPAR:
		factor = buildFuncDesig(stream);
		break;
	default:
		throw syntax_error(token.name, token.line, "Expected a factor");
	}
	return factor;
}

ast::Term* SyntaxAnalyzer::buildTerm(TokenStream& stream){
	ast::Term *func = NULL;
	ast::Term *arg = buildFactor(stream);
	if (!stream.hasNext()){
		return arg;
	}
	Token token = stream.next();
	while (token.type == Token::MUL || token.type == Token::DIV){
		func = new ast::Application(new ast::Reference(token.name), arg);
		arg = new ast::Application(func, buildFactor(stream));
	}
	stream.back();
	return arg;
}

ast::Term* SyntaxAnalyzer::buildExpr(TokenStream& stream){
	Token token = stream.next();
	if (token.type == Token::MATCH){
		stream.back();
		return buildMatchExpr(stream);
	}
	else{
		stream.back();
		ast::Term *term = buildSimExpr(stream);
		if (!stream.hasNext()){
			return term;
		}
		token = stream.next();
		switch (token.type){
		case Token::CMPE:
		case Token::CMPG:
		case Token::CMPGE:
		case Token::CMPL:
		case Token::CMPLE:
		case Token::CMPNE:
			term = new ast::Application(new ast::Reference(token.name), term);
			term = new ast::Application(term, buildSimExpr(stream));
			break;
		default:
			stream.back();
			break;
		}
		return term;
	}
}

ast::Term* SyntaxAnalyzer::buildSimExpr(TokenStream& stream){
	ast::Term *term = buildTerm(stream);
	if (!stream.hasNext()){
		return term;
	}
	Token token = stream.next();
	switch (token.type){
	case Token::ADD:
	case Token::SUB:
		term = new ast::Application(new ast::Reference(token.name), term);
		term = new ast::Application(term, buildTerm(stream));
		break;
	default:
		stream.back();
		break;
	}
	return term;
}

ast::Term* SyntaxAnalyzer::buildMatchExpr(TokenStream& stream){
	Token token = stream.next();
	if (token.type != Token::MATCH){
		throw syntax_error(token.name, token.line, "Expected match");
	}
	ast::Application* left = new ast::Application(new ast::Reference(token.name), buildExpr(stream));
	ast::Desum* right = new ast::Desum();
	while (stream.hasNext()){
		token = stream.next();
		if (token.type != Token::OR){
			stream.back();
			break;
		}
		right->cases.push_back(buildChoice(stream));
	}
	return new ast::Application(left, right);
}

ast::Term* SyntaxAnalyzer::buildChoice(TokenStream& stream){
	Token token = stream.next();
	ast::Application* app = NULL;
	switch (token.type){
	case Token::NIL:
	case Token::OTHER:
	case Token::TRUE:
	case Token::FALSE:
	case Token::INT:{
		string name = token.name;
		token = stream.next();	// =>
		if (token.type != Token::CHOICE){
			throw syntax_error(token.name, token.line, "Should be \"=>\"");
		}
		app = new ast::Application(new ast::Reference(name), buildExpr(stream));
		break;
	}
	case Token::ID:{
		ast::Term *left = new ast::Reference(token.name);
		token = stream.next();	// id
		while (token.type == Token::ID){
			left = new ast::Application(left, new ast::Reference(token.name));
			token = stream.next();
		}
		if (token.type != Token::CHOICE){
			throw syntax_error(token.name, token.line, "Should be \"=>\"");
		}
		app = new ast::Application(left, buildExpr(stream));
		break;
	}
	default:
		throw syntax_error(token.name, token.line, "Expected match type or value");
		break;
	}
	app = new ast::Application(new ast::Reference("cons"), app);
	return new ast::Application(app, buildChoice(stream));
}