#include "syntaxAnalyzer.h"
#include "myException.h"
#include <iostream>
SyntaxAnalyzer::SyntaxAnalyzer(TokenStream& stream)
{
	stream.initIter();

	// add support of type bool manually
	// only a temporary fix
	vector<pair<const ast::Type *, const string>> bools;
	bools.push_back(pair<const ast::Type *, const string>(getType("unit"), "false"));
	bools.push_back(pair<const ast::Type *, const string>(getType("unit"), "true"));
	types["bool"] = new ast::SumType(bools);

	root = buildBlock(stream);

}

SyntaxAnalyzer::~SyntaxAnalyzer()
{
	if (root != NULL){
		delete root;
	}
	for (map<const string, const ast::Type*>::iterator it = types.begin(); it != types.end(); it++){
		delete it->second;
	}
}

ast::Program* SyntaxAnalyzer::getProgram()const{
	return new ast::Program(types, root);
}

const ast::Type* SyntaxAnalyzer::getType(const string& s){
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
		throw syntax_error(token.name, token.nrow, "Should be an ID");
	}
	string typeId = token.name;
	vector<pair<const ast::Type*, const string>> sumTypes;
	ast::SumType* sumType = new ast::SumType(sumTypes);
	types[typeId] = sumType;

	token = stream.next();
	if (token.type == Token::COLON){
		token = stream.next();	// base type
		// Do something about the base&sub class
		token = stream.next();	// '='
	}
	if (token.type != Token::EQL){
		throw syntax_error(token.name, token.nrow, "Should be '='");
	}
	token = stream.next();
	if (token.type != Token::OR){
		throw syntax_error(token.name, token.nrow, "Should be '|'");
	}
	do{
		token = stream.next();
		if (token.type != Token::ID && token.type != Token::NIL){
			throw syntax_error(token.name, token.nrow, "Expected nil or identifier");
		}
		string cons = token.name;

		token = stream.next();
		if (token.type != Token::COLON){
			throw syntax_error(token.name, token.nrow, "Should be ':'");
		}
		vector<const ast::Type*> productTypes;
		while (true){
			token = stream.next();	// type id
			if (token.type != Token::ID && token.type != Token::NAT && token.type != Token::BOOL){
				throw syntax_error(token.name, token.nrow, "Expected type id");
			}
			string arg = token.name;

			token = stream.next();	// ->
			if (token.type != Token::PRODUCT){
				stream.back();
				if (arg != typeId){
					throw syntax_error(arg, token.nrow, "Expected " + typeId);
				}
				break;
			}
			productTypes.push_back(getType(arg));
		}
		if (productTypes.size() == 0){	// not a product type
			sumType->types.push_back(pair<const ast::Type*, const string>(getType("unit"), cons));
		}
		else{
			sumType->types.push_back(pair<const ast::Type*, const string>(new ast::ProductType(productTypes, cons), cons));
		}

		token = stream.next();			// if '|', go on, else stop
	} while (token.type == Token::OR);
	stream.back();		// put back the token

}

const ast::Type* SyntaxAnalyzer::buildFuncType(TokenStream& stream){
	Token token = stream.next();	// type id
	if (token.type != Token::ID && token.type != Token::NAT && token.type != Token::BOOL){
		throw syntax_error(token.name, token.nrow, "Should be a type id");
	}
	const ast::Type* type = getType(token.name);
	token = stream.next();
	if (token.type != Token::PRODUCT){
		stream.back();
		return type;
	}
	return new ast::FunctionType(type, buildFuncType(stream));
}

ast::Term* SyntaxAnalyzer::buildFuncDef(TokenStream& stream){
	Token token = stream.next();	// func id
	if (token.type != Token::ID){
		throw syntax_error(token.name, token.nrow, "Should be an ID");
	}
	string funcId = token.name;
	unsigned nrow = token.nrow, ncol = token.ncol;

	const ast::Type* type = NULL;
	ast::Term* term = NULL;

	vector<string> ids;
	vector<const ast::Type*> tps;
	vector<unsigned> rows;
	vector<unsigned> cols;

	token = stream.next();
	do{
		switch (token.type){
		case Token::LPAR:{
			token = stream.next();	// param id
			if (token.type != Token::ID){
				throw syntax_error(token.name, token.nrow, "Expected param id");
			}
			rows.push_back(token.nrow);
			cols.push_back(token.ncol);
			string param = token.name;
			token = stream.next();	// ':'
			if (token.type != Token::COLON){
				throw syntax_error(token.name, token.nrow, "Should be ':'");
			}
			ids.push_back(param);
			tps.push_back(buildFuncType(stream));

			token = stream.next();	// ')'
			if (token.type != Token::RPAR){
				throw syntax_error(token.name, token.nrow, "Should b ')'");
			}
			break;
		}
		case Token::COLON:{		// ':' return type
			type = buildFuncType(stream);
			break;
		}
		default:
			throw syntax_error(token.name, token.nrow, "Expected parameter or return type after func id");
		}
		token = stream.next();	// '='
	} while (token.type != Token::EQL);

	// func definition expression
	term = buildExpr(stream);

	// special case for main function
	if (funcId == "main"){
		term = new ast::Abstraction(ids[0], tps[0], term);
		term->nrow = nrow;
		term->ncol = ncol;
		return term;
	}

	// else, not main func
	for (int i = ids.size() - 1; i >= 0; i--){
		type = new ast::FunctionType(tps[i], type);
		term = new ast::Abstraction(ids[i], tps[i], term);
		term->nrow = rows[i];
		term->ncol = cols[i];
	}

	term = new ast::Abstraction(funcId, type, term);
	term->nrow = nrow;
	term->ncol = ncol;
	term = new ast::Fixpoint(term);
	term->nrow = nrow;
	term->ncol = ncol;
	ast::Abstraction* func = new ast::Abstraction(funcId, type, buildBlock(stream));
	func->nrow = nrow;
	func->ncol = ncol;
	term = new ast::Application(func, term);
	term->nrow = nrow;
	term->ncol = ncol;
	return term;
}

ast::Term* SyntaxAnalyzer::buildFuncDesig(TokenStream& stream){
	ast::Term *term = NULL;
	Token token = stream.next();	// funct id
	unsigned nrow = token.nrow, ncol = token.ncol;
	string funcId = token.name;
	token = stream.next();
	term = new ast::Reference(funcId);
	term->nrow = nrow;
	term->ncol = ncol;

	if (token.type == Token::RPAR){
		return term;
	}
	stream.back();
	term = new ast::Application(term, buildExpr(stream));
	term->nrow = nrow;
	term->ncol = ncol;

	token = stream.next();
	while (token.type != Token::RPAR){
		stream.back();
		term = new ast::Application(term, buildExpr(stream));
		term->nrow = nrow;
		term->ncol = ++ncol;
		token = stream.next();
	}
	return term;
}

ast::Term* SyntaxAnalyzer::buildFactor(TokenStream& stream){
	Token token = stream.next();
	unsigned nrow = token.nrow, ncol = token.ncol;
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
		factor->nrow = nrow;
		factor->ncol = ncol;
		break;
	case Token::LPAR:
		factor = buildFuncDesig(stream);
		break;
	default:
		throw syntax_error(token.name, token.nrow, "Expected a factor");
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
	unsigned nrow = token.nrow, ncol = token.ncol;
	while (token.type == Token::MUL || token.type == Token::DIV){
		func = new ast::Reference(token.name);
		func->nrow = nrow;
		func->ncol = ncol;
		func = new ast::Application(func, arg);
		func->nrow = nrow;
		func->ncol = ncol;
		arg = new ast::Application(func, buildFactor(stream));
		arg->nrow = nrow;
		arg->ncol = ncol;
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
		unsigned nrow = token.nrow, ncol = token.ncol;
		switch (token.type){
		case Token::CMPE:
		case Token::CMPG:
		case Token::CMPGE:
		case Token::CMPL:
		case Token::CMPLE:
		case Token::CMPNE:{
			ast::Reference* ref = new ast::Reference(token.name);
			ref->nrow = nrow;
			ref->ncol = ncol;
			term = new ast::Application(ref, term);
			term->nrow = nrow;
			term->ncol = ncol;
			term = new ast::Application(term, buildSimExpr(stream));
			term->nrow = nrow;
			term->ncol = ncol;
			break;
		}
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
	unsigned nrow = token.nrow, ncol = token.ncol;
	switch (token.type){
	case Token::ADD:
	case Token::SUB:{
		ast::Reference* ref = new ast::Reference(token.name);
		ref->nrow = nrow;
		ref->ncol = ncol;
		term = new ast::Application(ref, term);
		term->nrow = nrow;
		term->ncol = ncol;
		term = new ast::Application(term, buildTerm(stream));
		term->nrow = nrow;
		term->ncol = ncol;
		break;
	}
	default:
		stream.back();
		break;
	}
	return term;
}

// generate random string with length=len
string rands(const unsigned len) {
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
	string s;
	for (unsigned i = 0; i < len; i++) {
		s += alphanum[rand() % (sizeof(alphanum) - 1)];
	}
	return s;
}

ast::Term* SyntaxAnalyzer::buildMatchExpr(TokenStream& stream){
	Token token = stream.next();
	unsigned nrow = token.nrow, ncol = token.ncol;
	ast::Term* term = NULL;
	if (token.type != Token::MATCH){
		throw syntax_error(token.name, token.nrow, "Expected match");
	}
	ast::Term* expr = buildExpr(stream);
	vector<pair<const string, const ast::Term*>> cases;

	while (stream.hasNext()){
		token = stream.next();
		if (token.type != Token::OR){
			stream.back();
			break;
		}
		token = stream.next();
		string cons = token.name + '_' + rands(10);
		unsigned crow = token.nrow, ccol = token.ncol;

		token = stream.next();
		bool is_product = false;
		vector<string> names;
		while (token.type != Token::CHOICE){
			// if token is an id?
			if (token.type != Token::ID){
				throw syntax_error(token.name, token.nrow, "Expected id");
			}
			is_product = true;
			names.push_back(token.name);
			token = stream.next();
		}
		if (is_product){
			term = new ast::Reference(cons);
			term->nrow = crow;
			term->ncol = ccol;
			term = new ast::Deproduct(term, names, buildExpr(stream));
			term->nrow = crow;
			term->ncol = ccol;
			cases.push_back(pair<const string, const ast::Term*>(cons, term));
		}
		else{
			cases.push_back(pair<const string, const ast::Term*>(cons, buildExpr(stream)));
		}
	}
	term = new ast::Desum(expr, cases);
	term->nrow = nrow;
	term->ncol = ncol;
	return term;
}
