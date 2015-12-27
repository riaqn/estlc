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
  std::vector< std::pair<const std::string, const Term *> > cases;
  cases.push_back(std::make_pair("l_1", new Reference("l")));
  std::vector<std::string> names;
  names.push_back("car");
  names.push_back("cdr");
  cases.push_back(std::make_pair("l_2", new Deproduct(new Reference("l_2"), names, new Reference("cdr"))));
  Term *term = new Abstraction("l", list_int(), new Desum(new Reference("l"), cases));
  return term;
}
