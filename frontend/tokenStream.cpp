#include "tokenStream.h"

Token::Token(TokenType type, string name, unsigned line)
	:type(type), name(name), line(line)
{
}
Token::Token(const Token& token)
	:type(token.type), name(token.name), line(token.line)
{
}
bool Token::operator==(const Token& token){
	return (type == token.type) && (name == token.name);
}

TokenStream::TokenStream()
{
}
TokenStream::~TokenStream(){

}
Token& TokenStream::operator[](unsigned i) {
	return stream[i];
}
void TokenStream::initIter(){
	it = stream.begin();
}
const Token& TokenStream::next(){
	if (!hasNext()){
		throw new syntax_error("_TokenStream", (it - 1)->line, "TokenStream ends unexpectedly");
	}
	return *it++;
}
void TokenStream::back(unsigned i){
	it -= i;
}
bool TokenStream::hasNext() const{
	return it != stream.end();
}
unsigned TokenStream::size()const{
	return stream.size();
}
void TokenStream::append(Token token){
	stream.push_back(token);
}
void TokenStream::clear(){
	stream.clear();
}