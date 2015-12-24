#pragma once
#include "tokenStream.h"
#include <ast.hpp>
#include <map>

class SyntaxAnalyzer
{
public:
	SyntaxAnalyzer(TokenStream& stream);
	~SyntaxAnalyzer();
	ast::Program* getProgram()const;

private:

	ast::Term* buildBlock(TokenStream& stream);
	void buildTypeDef(TokenStream& stream);
	const ast::Type* buildFuncType(TokenStream& stream);
	ast::Term* buildFuncDef(TokenStream& stream);
	ast::Term* buildFuncDesig(TokenStream& stream);
	ast::Term* buildFactor(TokenStream& stream);
	ast::Term* buildTerm(TokenStream& stream);
	ast::Term* buildExpr(TokenStream& stream);
	ast::Term* buildSimExpr(TokenStream& stream);
	ast::Term* buildMatchExpr(TokenStream& stream);

	const ast::Type* getType(const std::string&);

	ast::Term* root;
	map<const std::string, const ast::Type*> types;
};

