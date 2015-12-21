#include <ast.hpp>
#include "types.hpp"


static ast::Term *filter() {
  using namespace ast;
  using namespace std;
  static Term *term = NULL;
  if (term == NULL) {
    term = new Fixpoint(new Abstraction("filter", new FunctionType(new FunctionType(Int(), Bool()), new FunctionType(list_int(), list_int())), new Abstraction("f", new FunctionType(Int(), Bool()), new Abstraction("l", list_int(), new Desum(new Reference("l"), std::vector<pair<const string, const Term *> >({make_pair("l0", new Reference("l")), make_pair("l1", new Deproduct(new Reference("l1"), std::vector<string>({"x", "l'"}), new Desum(new Application(new Reference("f"), new Reference("x")), vector<pair<const string, const Term *> >({make_pair("false", new Application(new Application(new Reference("filter"), new Reference("f")), new Reference("l'"))), make_pair("true", new Application(new Reference("l_1"), new Application(new Application(new Reference("ill"), new Reference("x")), new Application(new Application(new Reference("filter"), new Reference("f")), new Reference("l'")))))}))))}))))));
  }
  return term;
}

static ast::Term *app() {
  using namespace ast;
  using namespace std;
  static Term *term = NULL;
  if (term == NULL) {
    term = new Fixpoint(new Abstraction("app", new FunctionType(list_int(), new FunctionType(list_int(), list_int())), new Abstraction("l0", list_int(), new Abstraction("l1", list_int(), new Desum(new Reference("l0"), vector<pair<const string, const Term *> >({make_pair("l00", new Reference("l1")), make_pair("l01", new Deproduct(new Reference("l01"), std::vector<string>({"x", "l0'"}), new Application(new Reference("l_1"), new Application(new Application(new Reference("ill"), new Reference("x")), new Application(new Application(new Reference("app"), new Reference("l0'")), new Reference("l1"))))))}))))));
  }
  return term;
}
