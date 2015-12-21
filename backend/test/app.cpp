#include <ast.hpp>
#include "common/types.hpp"

using namespace ast;

std::vector<const Type *> getTypes() {
  std::vector<const Type *> types;
  types.push_back(int0());
  types.push_back(list_int_y(list_int()));
  types.push_back(list_int());
  return types;
}

Term *getTerm() {
  Type *l2l = new FunctionType(list_int(), list_int());
  Term *term = new Abstraction("l", list_int(), new Application(new Abstraction("id", l2l, new Application(new Reference("id"), new Reference("l"))),
                                                                new Abstraction("l'", list_int(), new Reference("l'"))
                                                                ));
  return term;
}
