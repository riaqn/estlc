#include <map>
#include <string>
#include <ast.hpp>
#include <vector>
#include <memory>

#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>

#include "env.hpp"

class Codegen {
private:
  llvm::LLVMContext &context;
  llvm::Module *module;
  llvm::IRBuilder<> builder;
  llvm::DataLayout layout;

  std::vector<llvm::Type *> argsType;
  llvm::Type  *refType, *productType, *indexType, *sumType, *closureType, *stackType;
  llvm::FunctionType *funcType;


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
  Term generate(const ast::SumType *sum, const uint32_t idx);
  Term generate(const ast::ProductType *product);
  Term generate(const ast::Program &prog);
  
  void generatePush(llvm::Value *const value, llvm::Value *&stack);
  llvm::LoadInst *generatePop(llvm::Type *type, llvm::Value *&stack);
  llvm::Value *generateMalloc(llvm::Type *type);
  llvm::Value *generateMalloc(llvm::Value *size);

  llvm::Value *generateClosure(llvm::Value *func, llvm::Value *stack);

  void dump();
};
