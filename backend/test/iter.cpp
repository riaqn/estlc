#include <ast.hpp>
#include "common/types.hpp"

using namespace ast;
Term *getTerm() {
  Type *iter_type = new FunctionType(list_int(), list_int());

  //WTF?
  Term *term = new Fixpoint(new Abstraction("iter",
                                            iter_type,
                                            new Abstraction("l",
                                                            list_int(),
                                                            new Desum(new Reference("l"),
                                                                      std::vector<std::pair<const std::string, const Term *> >({
                                                                          std::make_pair("l0", new Reference("l")),
                                                                            std::make_pair("l1",
                                                                                           new Deproduct(new Reference("l1"),
                                                                                                         std::vector<std::string>({"x","l'"}),
                                                                                                         new Application(new Reference("l_1"),
                                                                                                                         new Application(
                                                                                                                                                     new Application(new Reference("ill"),
                                                                                                                                                                     new Reference("x")),
                                                                                                                                                     new Application(new Reference("iter"),
                                                                                                                                                                     new Reference("l'"))))))})))));

  return term;
}

std::vector<const Type *> getTypes() {
  std::vector<const Type *> types;
  types.push_back(int0());
  types.push_back(list_int_y(list_int()));
  types.push_back(list_int());
  return types;
}

