#include <map>
#include <string>
#include <ast.hpp>
#include <vector>
#include <memory>

#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>

struct Term {
  llvm::Function *value;
  ast::Type *type;
};

typedef unsigned VarIndex;
typedef std::map<std::string, VarIndex> Env;

class Codegen {
public:
  Codegen();
  Term generate(const ast::Term *term, Env &env);
  Term generate(const ast::Application *app, Env &env);
private:
  std::vector<llvm::Type *> argsType;
  llvm::Type *stackType, *dataType, *closureType, *funcType;
  llvm::Module *module;
  llvm::IRBuilder<> builder;
  llvm::DataLayout layout;
};
