#pragma once
#include <string>
#include <vector>
#include "myException.h"

using namespace std;
struct Token{
	enum TokenType{
		// reserved symbols
		TYPE = 0,	// Type
		FUNC,	// Func
		MATCH,	// match
		OTHER,	// other
		NIL,	// nil
		NAT,	// nat, possibly has sign before it
		BOOL,	// bool
		TRUE,	// true
		FALSE,	// false

		ID,		// identifier
		INT,	// integer
		COM,	// comment, start with #, end with \n
		// operators
		ADD,	// +
		SUB,	// -
		MUL,	// *
		DIV,	// /
		CMPL,	// <
		CMPG,	// >
		CMPLE,	// <=
		CMPGE,	// >=
		CMPE,	// ==
		CMPNE,	// <>
		// other symbols
		EQL,	// =
		PRODUCT,// ->
		CHOICE,	// =>
		OR,		// |
		COLON,	// :
		LPAR,	// (
		RPAR	// )

	};
	TokenType type;
	string name;
	unsigned line;

	Token(TokenType type, string name, unsigned line)
		:type(type), name(name), line(line){}
	Token(const Token& token)
		:type(token.type), name(token.name), line(token.line){}
	bool operator==(const Token& token){
		return (type == token.type) && (name == token.name);
	}
};

const string TokenName[] = {
	"Type",		// Type
	"Func",		// Func
	"match",	// match
	"other",	// other
	"nil",		// nil
	"nat",		// nat
	"bool",		// bool
	"true",		// true
	"false",	// false
	"id",		// identifier
	"int",		// integer
	"comment",	// comment, start with #, end with \n
	// operators
	"+",	// +
	"-",	// -
	"*",	// *
	"/",	// /
	"<",	// <
	">",	// >
	"<=",	// <=
	">=",	// >=
	"==",	// ==
	"<>",	// <>
	// other symbols
	"=",	// =
	"->",	// ->
	"=>",	// =>
	"|",	// |
	":",	// :
	"(",	// (
	")"		// )
};

class TokenStream{
public:
	TokenStream()
	{
	}
	~TokenStream(){

	}
	Token& operator[](unsigned i) {
		return stream[i];
	}
	void initIter(){
		it = stream.begin();
	}
	const Token& next(){
		if (!hasNext()){
			throw new syntax_error("_TokenStream", (it-1)->line, "TokenStream ends unexpectedly");
		}
		return *it++;
	}
	void back(unsigned i = 1){
		it -= i;
	}
	bool hasNext() const{
		return it != stream.end();
	}
	unsigned size()const{
		return stream.size();
	}
	void append(Token token){
		stream.push_back(token);
	}
	void clear(){
		stream.clear();
	}
private:
	vector<Token> stream;
	vector<Token>::const_iterator it;
};
