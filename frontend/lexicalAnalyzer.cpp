#include "lexicalAnalyzer.h"
#include <fstream>
#include "myException.h"

LexicalAnalyzer::LexicalAnalyzer()
{
}


LexicalAnalyzer::~LexicalAnalyzer()
{
}


TokenStream LexicalAnalyzer::parse(istream& is){
	string buffer;
	char peek, ch;
	unsigned nrow = 1, ncol = 0;
	TokenStream tokenStream;
	while (true){
		ncol++;
		peek = is.get();
		if (peek == EOF){
			break;
		}
		if (isalpha(peek)){
			do{
				buffer += peek;
				peek = is.get();
			} while (isalnum(peek) || peek == '_' || peek == '\'');
			is.putback(peek);
			unsigned n;
			for (n = Token::TYPE; n < Token::ID; n++){
				if (buffer == TokenName[n]){
					break;
				}
			}
			tokenStream.append(Token(Token::TokenType(n), buffer, nrow, ncol));
			buffer.clear();
			continue;
		}
		else if (isdigit(peek)){
			do{
				buffer += peek;
				peek = is.get();
			} while (isdigit(peek));
			is.putback(peek);
			tokenStream.append(Token(Token::INT, buffer, nrow, ncol));
			buffer.clear();
			continue;
		}
		switch (peek){
		case '\r':
			ch = is.get();
			if (ch != '\n'){
				is.putback(ch);
			}
		case '\n':
			nrow++;
			ncol = 0;
			break;
		case '#':
			getline(is, buffer);
			tokenStream.append(Token(Token::COM, buffer, nrow, ncol));
			buffer.clear();
			nrow++;
			ncol = 0;
			break;
		case '\t':
		case ' ':
			break;
		case '+':
			tokenStream.append(Token(Token::ADD, "+", nrow, ncol));
			break;
		case '-':
			ch = is.get();
			if (ch == '>'){
				tokenStream.append(Token(Token::PRODUCT, "->", nrow, ncol));
			}
			else{
				is.putback(ch);
				tokenStream.append(Token(Token::SUB, "-", nrow, ncol));
			}
			break;
		case '*':
			tokenStream.append(Token(Token::MUL, "*", nrow, ncol));
			break;
		case '/':
			tokenStream.append(Token(Token::DIV, "/", nrow, ncol));
			break;
		case '<':
			ch = is.get();
			if (ch == '='){
				tokenStream.append(Token(Token::CMPLE, "<=", nrow, ncol));
			}
			else if (ch == '>'){
				tokenStream.append(Token(Token::CMPNE, ">=", nrow, ncol));
			}
			else{
				is.putback(ch);
				tokenStream.append(Token(Token::CMPL, "<", nrow, ncol));
			}
			break;
		case '>':
			ch = is.get();
			if (ch == '='){
				tokenStream.append(Token(Token::CMPGE, ">=", nrow, ncol));
			}
			else{
				is.putback(ch);
				tokenStream.append(Token(Token::CMPG, ">", nrow, ncol));
			}
			break;
		case '=':
			ch = is.get();
			if (ch == '='){
				tokenStream.append(Token(Token::CMPE, "==", nrow, ncol));

			}
			else if (ch == '>'){
				tokenStream.append(Token(Token::CHOICE, "=>", nrow, ncol));
			}
			else{
				is.putback(ch);
				tokenStream.append(Token(Token::EQL, "=", nrow, ncol));
			}
			break;
		case '|':
			tokenStream.append(Token(Token::OR, "|", nrow, ncol));
			break;
		case ':':
			tokenStream.append(Token(Token::COLON, ":", nrow, ncol));
			break;
		case '(':
			tokenStream.append(Token(Token::LPAR, "(", nrow, ncol));
			break;
		case ')':
			tokenStream.append(Token(Token::RPAR, ")", nrow, ncol));
			break;
		default:
			// unknown character
			throw lexical_error(nrow, peek);
			break;
		}
	}
	return tokenStream;
}
