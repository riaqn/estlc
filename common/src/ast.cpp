#include <ast.hpp>

using namespace ast;


Type::~Type() {}

Term::~Term() {}

bool Type::operator ==(const Type &b) const {
  if (const PrimitiveType *prim = dynamic_cast<const PrimitiveType *>(&b))
    return *this == *prim;
  else if (const ProductType *prod = dynamic_cast<const ProductType *>(&b))
    return *this == *prod;
  else if (const SumType *sum = dynamic_cast<const SumType *>(&b))
    return *this == *sum;
  else if (const FunctionType *func = dynamic_cast<const FunctionType *>(&b))
    return *this == *func;
  else
    throw Exception();
}
bool Type::operator==(const PrimitiveType &) const {return false;}
bool Type::operator==(const SumType &) const {return false;}
bool Type::operator==(const ProductType &) const {return false;}
bool Type::operator==(const FunctionType &) const {return false;}


bool Type::operator !=(const Type &b) const {
  return !(*this == b);
}


ProductType::ProductType(const std::vector<const Type *> &types, const std::string &cons)
  :types(types), cons(cons)
{}

bool ProductType::operator==(const ProductType &b) const {
  return this == &b;
}

SumType::SumType(const std::vector<std::pair<const Type *, const std::string> > &types)
  :types(types) {}

bool SumType::operator==(const SumType &b) const {
  return this == &b;
}

PrimitiveType::PrimitiveType(const std::string& name)
  :name(name)
{}

bool PrimitiveType::operator ==(const PrimitiveType &b) const {
  return name == b.name;
}

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

bool FunctionType::operator==(const FunctionType &b) const {
  return (*left == *b.left) && (*right == *b.right);
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

Desum::Desum(const Term *sum, const std::vector<std::pair<const std::string, const Term *> > &cases)
  :sum(sum), cases(cases)
{}

Deproduct::Deproduct(const Term *const product, const std::vector<std::string> &names, const Term *const term)
  :product(product), names(names), term(term)
{}

Fixpoint::Fixpoint(const Term *term)
  :term(term)
{}
   
Program::Program(const std::vector<const Type *> types, const Term *term)
  :types(types), term(term)
{}
