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
	unsigned line = 1;
	TokenStream tokenStream;
	while (true){
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
			tokenStream.append(Token(Token::TokenType(n), buffer, line));
			buffer.clear();
			continue;
		}
		else if (isdigit(peek)){
			do{
				buffer += peek;
				peek = is.get();
			} while (isdigit(peek));
			is.putback(peek);
			tokenStream.append(Token(Token::INT, buffer, line));
			buffer.clear();
			continue;
		}
		//else if (buffer.length() == 0 && (peek == '+' || peek == '-')){
		//	ch = is.get();
		//	is.putback(ch);
		//	if (isdigit(ch)){
		//		buffer += peek;
		//		continue;
		//	}
		//}
		switch (peek){
		case '\r':
			ch = is.get();
			if (ch != '\n'){
				is.putback(ch);
			}
		case '\n':
			line++;
			break;
		case '\t':
		case ' ':
			break;
		case '#':
			getline(is, buffer);
			tokenStream.append(Token(Token::COM, buffer, line));
			line++;
			buffer.clear();
			break;
		case '+':
			tokenStream.append(Token(Token::ADD, "+", line));
			break;
		case '-':
			ch = is.get();
			if (ch == '>'){
				tokenStream.append(Token(Token::PRODUCT, "->", line));
			}
			else{
				is.putback(ch);
				tokenStream.append(Token(Token::SUB, "-", line));
			}
			break;
		case '*':
			tokenStream.append(Token(Token::MUL, "*", line));
			break;
		case '/':
			tokenStream.append(Token(Token::DIV, "/", line));
			break;
		case '<':
			ch = is.get();
			if (ch == '='){
				tokenStream.append(Token(Token::CMPLE, "<=", line));
			}
			else if (ch == '>'){
				tokenStream.append(Token(Token::CMPNE, ">=", line));
			}
			else{
				is.putback(ch);
				tokenStream.append(Token(Token::CMPL, "<", line));
			}
			break;
		case '>':
			ch = is.get();
			if (ch == '='){
				tokenStream.append(Token(Token::CMPGE, ">=", line));
			}
			else{
				is.putback(ch);
				tokenStream.append(Token(Token::CMPG, ">", line));
			}
			break;
		case '=':
			ch = is.get();
			if (ch == '='){
				tokenStream.append(Token(Token::CMPE, "==", line));

			}
			else if (ch == '>'){
				tokenStream.append(Token(Token::CHOICE, "=>", line));
			}
			else{
				is.putback(ch);
				tokenStream.append(Token(Token::EQL, "=", line));
			}
			break;
		case '|':
			tokenStream.append(Token(Token::OR, "|", line));
			break;
		case ':':
			tokenStream.append(Token(Token::COLON, ":", line));
			break;
		case '(':
			tokenStream.append(Token(Token::LPAR, "(", line));
			break;
		case ')':
			tokenStream.append(Token(Token::RPAR, ")", line));
			break;
		default:
			// unknown character
			throw lexical_error(line, peek);
			break;
		}
	}
	return tokenStream;
}
