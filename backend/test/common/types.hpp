#include <ast.hpp>

static ast::Type *int0();
static ast::Type *unit();
static ast::Type *list_int();
static ast::Type *list_int_y(const ast::Type *list_int);

using namespace ast;

static Type *int0() {
  static Type *type = NULL;
  if (type == NULL) 
    type = new PrimitiveType("int");
  return type;
}

static Type *unit() {
  static Type *type = NULL;
  if (type == NULL) {
    type = new PrimitiveType("unit");
  }
  return type;
}

static Type *list_int_y(const Type *list_int) {
  static Type *type = NULL;
  if (type == NULL) {
    std::vector<const Type *> types;
    types.push_back(int0());
    types.push_back(list_int);
    type = new ProductType(types, "ill");
  }
  return type;
}

static Type *list_int() {
  static SumType *type = NULL;
  if (type == NULL) {
    std::vector<std::pair<const Type *, const std::string> > types;

    types.push_back(std::make_pair(unit(), "l_0"));
    types.push_back(std::make_pair(list_int_y(NULL), "l_1"));
    type = new SumType(types);
    ((ProductType *)type->types[1].first)->types[1] = type;
  }
  return type;
}
