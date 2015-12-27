#include <ast.hpp>
#include "common/types.hpp"
#include "common/terms.hpp"

using namespace ast;
using namespace std;

std::vector<const Type *> getTypes() {
  std::vector<const Type *> types;
  types.push_back(Int());
  types.push_back(list_int_y(list_int()));
  types.push_back(list_int());
  return types;
}

Program *getProgram() {
  Term *term0 = new Application(new Reference("qs"), new Application(new Application(new Reference("filter"), new Application(new Reference("<"), new Reference("x"))), new Reference("l'")));
  Term *term1 = new Application(new Reference("qs"), new Application(new Application(new Reference("filter"), new Application(new Reference(">="), new Reference("x"))), new Reference("l'")));
  //term for qs
  Term *term2 = new Abstraction("l", list_int(), new Desum(new Reference("l"), std::vector<pair<const string, const Term *> >({make_pair("l0", new Reference("l")), make_pair("l1", new Deproduct(new Reference("l1"), std::vector<string>({"x", "l'"}), new Application(new Application(new Reference("app"), term0), new Application(new Reference("l_1"), new Application(new Application(new Reference("ill"), new Reference("x")), term1)))))})));
  
  Term *term = new Application(new Abstraction("filter", new FunctionType(new FunctionType(Int(), Bool()), new FunctionType(list_int(), list_int())), new Application(new Abstraction("app", new FunctionType(list_int(), new FunctionType(list_int(), list_int())), new Fixpoint(new Abstraction("qs", new FunctionType(list_int(), list_int()), term2))), app())), filter());
  return new Program(getTypes(), term);
}


