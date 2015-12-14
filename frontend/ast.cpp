#include "../include/ast.hpp"

ast::Type::~Type(){}

ast::PrimitiveType::PrimitiveType(const std::string& name)
	:name(name)
{
}

ast::ProductType::ProductType(Type *x, Type *y, std::string cons)
	: x(x), y(y), cons(cons)
{
}

ast::ProductType::~ProductType(){
	if (x != NULL && ((PrimitiveType*)x) == NULL){	// prevent duplicate delete
		delete x;
	}
	if (y != NULL && ((PrimitiveType*)y) == NULL){
		delete y;
	}
}

ast::FunctionType::FunctionType(Type *left, Type *right)
	:left(left), right(right)
{
}

ast::FunctionType::~FunctionType(){
	if (left != NULL && ((PrimitiveType*)left) == NULL){	// prevent duplicate delete
		delete left;
	}
	if (right != NULL && ((PrimitiveType*)right) == NULL){
		delete right;
	}
}

ast::Term::~Term(){}

ast::Reference::Reference(const std::string name) 
	:name(name)
{
}


ast::Abstraction::Abstraction(const std::string& arg, Type* type, Term* term) 
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


ast::Application::Application(Term* func, Term* arg) 
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