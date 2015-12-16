#include <ast.hpp>
#include <string>
#include <codegen.hpp>
#include "common/types.hpp"

using namespace ast;

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
