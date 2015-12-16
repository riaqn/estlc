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

int main() {
  std::vector<const Type *> types;
  types.push_back(int0());
  types.push_back(list_int_y(list_int()));
  types.push_back(list_int());

  std::vector< std::pair<const std::string, const Term *> > cases;
  cases.push_back(std::make_pair("l_1", new Reference("l")));
  std::vector<std::string> names;
  names.push_back("car");
  names.push_back("cdr");
  cases.push_back(std::make_pair("l_2", new Deproduct(new Reference("l_2"), names, new Reference("cdr"))));
  Term *term = new Abstraction("l", list_int(), new Desum(new Reference("l"), cases));
  Program program(types, term);

  Codegen codegen;
  Codegen::Term v = codegen.generate(program);
  (void)v;

  codegen.dump();
}
