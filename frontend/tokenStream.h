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

	Token(TokenType type, string name, unsigned line);
	Token(const Token& token);
	bool operator==(const Token& token);
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
	TokenStream();
	~TokenStream();
	Token& operator[](unsigned i);
	void initIter();
	const Token& next();
	void back(unsigned i = 1);
	bool hasNext() const;
	unsigned size()const;
	void append(Token token);
	void clear();

private:
	vector<Token> stream;
	vector<Token>::const_iterator it;
};
