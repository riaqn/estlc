#include <ast.hpp>
#include <string>
#include <codegen.hpp>
#include <exception.hpp>
#include <iostream>

using namespace ast;

extern Program *getProgram();

int main() {
  Program *program = getProgram();

  Codegen codegen;
  Codegen::Term v = codegen.generate(*program);
  (void)v;
  codegen.dump();
}
