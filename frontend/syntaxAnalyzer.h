#pragma once
#include "tokenStream.h"
#include <ast.hpp>
#include <map>

class SyntaxAnalyzer
{
public:
	SyntaxAnalyzer(TokenStream& stream);
	~SyntaxAnalyzer();
	ast::Term* getRoot()const{ return root; }

private:

	ast::Term* buildBlock(TokenStream& stream);
	void buildTypeDef(TokenStream& stream);
	ast::Type* buildFuncType(TokenStream& stream);
	ast::Term* buildFuncDef(TokenStream& stream);
	ast::Term* buildFuncDesig(TokenStream& stream);
	ast::Term* buildFactor(TokenStream& stream);
	ast::Term* buildTerm(TokenStream& stream);
	ast::Term* buildExpr(TokenStream& stream);
	ast::Term* buildSimExpr(TokenStream& stream);
	ast::Application* buildMatchExpr(TokenStream& stream);
	ast::Term* buildChoice(TokenStream& stream);

	ast::Term* root;
	map<string, ast::Type*> types;
};

