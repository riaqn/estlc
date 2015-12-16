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

  Term *term = new Abstraction("l", list_int(), new Reference("l"));
  
  Program program(types, term);

  Codegen codegen;
  Codegen::Term v = codegen.generate(program);
  (void)v;

  codegen.dump();
}
