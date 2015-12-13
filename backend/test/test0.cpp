#include <ast.hpp>
#include <string>
#include <codegen.hpp>

using namespace ast;

Type *int0() {
  static Type *type = NULL;
  if (type == NULL) 
    type = new PrimitiveType("int");
  return type;
}

Type *unit() {
  static Type *type = NULL;
  if (type == NULL) {
    type = new PrimitiveType("unit");
  }
  return type;
}

Type *list_int();

Type *list_int_y(const Type *list_int) {
  static Type *type = NULL;
  if (type == NULL) {
    type = new ProductType(int0(), list_int, "list_int_y_cons");
  }
  return type;
}

Type *list_int() {
  static SumType *type = NULL;
  if (type == NULL) {
    std::vector<std::pair<const Type *, const std::string> > types;

    types.push_back(std::make_pair(unit(), "unit2list_int"));
    types.push_back(std::make_pair(list_int_y(NULL), "list_int_y2list_int"));
    type = new SumType(types);
    ((ProductType *)type->types[1].first)->y = type;
  }
  return type;
}

int main() {
  std::vector<const Type *> types;
  types.push_back(int0());
  types.push_back(list_int_y(list_int()));
  types.push_back(list_int());

  Term *term = new Abstraction("l", list_int(), new Reference("l"));
  
  Program program(types, term);

  Codegen codegen;
  Codegen::Term v = codegen.generate(program);

  codegen.dump();
}
