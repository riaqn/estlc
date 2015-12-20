#pragma once
#include <ast.hpp>

static ast::Type *Int();
static ast::Type *Unit();
static ast::Type *list_int();
static ast::Type *list_int_y(const ast::Type *list_int);

using namespace ast;

static Type *Int() {
  static Type *type = NULL;
  if (type == NULL) 
    type = new PrimitiveType("Int");
  return type;
}

static Type *list_int_y(const Type *list_int) {
  static Type *type = NULL;
  if (type == NULL) {
    std::vector<const Type *> types;
    types.push_back(Int());
    types.push_back(list_int);
    type = new ProductType(types, "ill");
  }
  return type;
}

static Type *Unit() {
  static PrimitiveType *type = NULL;
  if (type == NULL) {
    type = new PrimitiveType("Unit");
  }
  return type;
}

static Type *Bool() {
  static SumType *type = NULL;
  if (type == NULL) {
    std::vector<std::pair<const Type *, const std::string> > types;
    types.push_back(std::make_pair(Unit(), "false"));
    types.push_back(std::make_pair(Unit(), "true"));
    type = new SumType(types);
  }
  return type;
}

static Type *list_int() {
  static SumType *type = NULL;
  if (type == NULL) {
    std::vector<std::pair<const Type *, const std::string> > types;

    types.push_back(std::make_pair(Unit(), "l_0"));
    types.push_back(std::make_pair(list_int_y(NULL), "l_1"));
    type = new SumType(types);
    ((ProductType *)type->types[1].first)->types[1] = type;
  }
  return type;
}
