#include <map>
#include <string>
#include <ast.hpp>
#include <vector>
#include <memory>

#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>

#include "env.hpp"

class Codegen {
private:
  uint64_t pointerSize;
  std::vector<llvm::Type *> argsType;
  llvm::Type *stackType, *dataType, *closureType, *funcType;
  llvm::Module *module;
  llvm::IRBuilder<> builder;
  llvm::DataLayout layout;
public:
  struct Term {
    llvm::Function *value;
    const ast::Type *type;
  };
  Codegen();
  Term generate(const ast::Term *const term, Env<llvm::APInt> &env);
  Term generate(const ast::Application *const app, Env<llvm::APInt> &env);
  Term generate(const ast::Abstraction *const abs, Env<llvm::APInt> &env);
  Term generate(const ast::Reference *const ref, Env<llvm::APInt> &env);
  Term generate(const ast::Deproduct *const dep, Env<llvm::APInt> &env);
  Term generate(const ast::Desum *const des, Env<llvm::APInt> &env);
};
