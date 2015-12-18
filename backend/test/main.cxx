#include <ast.hpp>
#include <string>
#include <codegen.hpp>
#include <exception.hpp>
#include <iostream>

using namespace ast;

extern Term *getTerm();
extern std::vector<const Type *> getTypes();

int main() {
  Program program(getTypes(), getTerm());

  Codegen codegen;
  Codegen::Term v = codegen.generate(program);
  (void)v;
  codegen.dump();
}
