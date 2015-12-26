#include <ast.hpp>
#include <lexicalAnalyzer.h>
#include <syntaxAnalyzer.h>
#include <fstream>

using namespace ast;

Program *getProgram() {
  std::ifstream ifs("qs.estlc");
  LexicalAnalyzer la;
  TokenStream ts(la.parse(ifs));
  
  SyntaxAnalyzer *sa = new SyntaxAnalyzer(ts);
  return sa->getProgram();
}

