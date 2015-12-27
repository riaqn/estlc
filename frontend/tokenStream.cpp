#include "tokenStream.h"

Token::Token(TokenType type, string name, unsigned nrow, unsigned ncol)
	:type(type), name(name), nrow(nrow), ncol(ncol)
{
}
Token::Token(const Token& token)
	:Token(token.type, token.name, token.nrow, token.ncol)
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
		throw new syntax_error("_TokenStream", (it - 1)->nrow, "TokenStream ends unexpectedly");
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