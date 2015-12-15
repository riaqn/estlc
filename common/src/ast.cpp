#include <ast.hpp>

using namespace ast;


Type::~Type() {}

Term::~Term() {}


ProductType::ProductType(std::vector<const Type *> &types, const std::string &cons)
  :types(types), cons(cons)
{}

SumType::SumType(std::vector<std::pair<const Type *, const std::string> > &types)
  :types(types) {}

PrimitiveType::PrimitiveType(const std::string& name)
  :name(name)
{}

ast::FunctionType::FunctionType(const Type *const left, const Type *const right)
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


Desum::Desum(const Term *sum, std::vector<std::pair<const std::string, const Term *> > &cases)
  :sum(sum), cases(cases)
{}

Deproduct::Deproduct(const Term *const product, std::vector<std::string> &names, const Term *const term)
  :product(product), names(names), term(term)
{}

Fixpoint::Fixpoint(const Term *term)
  :term(term)
{}
   
Program::Program(const std::vector<const Type *> types, const Term *term)
  :types(types), term(term)
{}
