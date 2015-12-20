#include <ast.hpp>
#include <lexicalAnalyzer.h>
#include <syntaxAnalyzer.h>
#include <fstream>

using namespace ast;

Program *getProgram() {
  std::ifstream ifs("lt.estlc");
  LexicalAnalyzer la;
  TokenStream ts(la.parse(ifs));
  
  SyntaxAnalyzer sa(ts);
  return sa.getProgram();
}

