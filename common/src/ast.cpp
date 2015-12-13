#include <ast.hpp>

using namespace ast;

PrimitiveType::PrimitiveType(const std::string& name)
	:name(name)
{
}

FunctionType::FunctionType(const Type *const left, const Type *const right)
	:left(left), right(right)
{
}

FunctionType::~FunctionType(){
	if (left != NULL && ((PrimitiveType*)left) == NULL){	// prevent duplicate delete
		delete left;
	}
	if (right != NULL && ((PrimitiveType*)right) == NULL){
		delete right;
	}
}

ast::Reference::Reference(const std::string &name) 
	:name(name)
{
}


ast::Abstraction::Abstraction(const std::string& arg, const Type *const type, const Term *const term) 
	:arg(arg), type(type), term(term)
{
}

ast::Abstraction::~Abstraction(){
	if (type != NULL && ((PrimitiveType*)type) == NULL){
		delete type;
	}
	if (term != NULL){
		delete term;
	}
}


ast::Application::Application(const Term *const func, const Term *const arg) 
	:func(func), arg(arg)
{
}

ast::Application::~Application(){
	if (func != NULL){
		delete func;
	}
	if (arg != NULL){
		delete arg;
	}
}
