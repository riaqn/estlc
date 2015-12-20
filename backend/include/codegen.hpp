#include <map>
#include <string>
#include <ast.hpp>
#include <vector>
#include <memory>
#include <utility>

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
  llvm::Type  *productType, *indexType, *sumType, *closureType;
  llvm::PointerType *PclosureType, *PfuncType, *stackType, *refType;
  llvm::FunctionType *funcType;

  ast::SumType *Bool;
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
  Term generate(const ast::Fixpoint *const fix, Env<llvm::APInt> &env);
  Term generate(const ast::SumType *sum, const uint32_t idx);
  Term generate(const ast::ProductType *product);

  Term generate(const ast::Program &prog);
  
  void generatePush(llvm::Value *const value, llvm::Value *&stack);
  llvm::LoadInst *generatePop(llvm::Type *type, llvm::Value *&stack);
  llvm::Value *generateEval(llvm::Value *eval, llvm::Value *stack);
  llvm::Value *generateMalloc(llvm::Type *type);
  llvm::Value *generateMalloc(llvm::Value *size);
  llvm::Value *generateMemmove(llvm::Value *dst, llvm::Value *src, llvm::Value *n);
  llvm::Value *generatePrintf(const char *const fmt, llvm::Value *val);
  llvm::Value *generateClosure(llvm::Value *func, llvm::Value *stack);
  llvm::Value *generateLoad(llvm::Type *type, llvm::Value *ptr);
  llvm::Value *generateSum(llvm::Value *idx, llvm::Value *ref);

  std::pair<llvm::Value *, llvm::Value *> generateDeclosure(llvm::Value *clo);
  Term generatePrimitive(const std::string &prim);
  llvm::Function *generateBinary(llvm::Function *f0);

  void dump();
};
