#include <ast.hpp>
#include "common/types.hpp"

using namespace ast;
using namespace std;
std::vector<const Type *> getTypes() {
  std::vector<const Type *> types;
  types.push_back(int0());
  types.push_back(list_int_y(list_int()));
  types.push_back(list_int());
  return types;
}

Program *getProgram() {
  //WTF?
  Term *term = new Abstraction("l", list_int(), new Desum(new Reference("l"), std::vector<std::pair<const std::string, const Term *> >({make_pair("l0", new Reference("l")), make_pair("l1", new Deproduct(new Reference("l1"), std::vector<string>({"x", "l'"}), new Desum(new Reference("l'"), std::vector<std::pair<const string, const Term *> >({make_pair("l'0", new Reference("l")), make_pair("l'1", new Deproduct(new Reference("l'1"), std::vector<string>({"x'", "l''"}), new Desum(new Application(new Application(new Reference("<"), new Reference("x")), new Reference("x'")), vector<pair<const string ,const Term *> >({make_pair("false", new Reference("l")), make_pair("true", new Reference("l'"))}))))}))))})));

  return new Program(getTypes(), term);
}


