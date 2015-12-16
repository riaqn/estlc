#include <ast.hpp>

ast::Type *int0();
ast::Type *unit();
ast::Type *list_int();
ast::Type *list_int_y(const ast::Type *list_int);

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

Type *list_int_y(const Type *list_int) {
  static Type *type = NULL;
  if (type == NULL) {
    std::vector<const Type *> types;
    types.push_back(int0());
    types.push_back(list_int);
    type = new ProductType(types, "int -> list_int -> list_int");
  }
  return type;
}

Type *list_int() {
  static SumType *type = NULL;
  if (type == NULL) {
    std::vector<std::pair<const Type *, const std::string> > types;

    types.push_back(std::make_pair(unit(), "unit -> list_int"));
    types.push_back(std::make_pair(list_int_y(NULL), "list_int_y -> list_int"));
    type = new SumType(types);
    ((ProductType *)type->types[1].first)->types[1] = type;
  }
  return type;
}

