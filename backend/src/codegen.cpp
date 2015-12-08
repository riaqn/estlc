#include "codegen.hpp"
#include "exception.hpp"
#include <vector>
#include <llvm/IR/Verifier.h>

using namespace llvm;

Codegen::Codegen()
  : module(new Module("", getGlobalContext())),
    builder(getGlobalContext()),
    layout(module) {
  stackType = PointerType::get(PointerType::get(Type::getVoidTy(getGlobalContext()), 0), 0);

  argsType.push_back(stackType);
  
  std::vector<Type *> elems;
  elems.push_back(PointerType::get(funcType, 0));
  elems.push_back(stackType);
  closureType = StructType::get(getGlobalContext(), elems);

  dataType = PointerType::get(Type::getVoidTy(getGlobalContext()), 0);

  funcType = FunctionType::get(dataType, argsType, false);
}

Term Codegen::generate(const ast::Application *app, Env &env) {
  Term func = generate(app->func, env);
  Term arg = generate(app->arg, env);

  //type check
  ast::FunctionType *func_type = static_cast<ast::FunctionType *>(func.type);
  if (func_type == NULL)
    throw new NotFunction(TermException(app->func, func.type));
  if (func_type->left != arg.type)
    throw new TypeNotMatch(TermException(app->arg, arg.type), func_type->left);


  FunctionType *ft = FunctionType::get(dataType, argsType, false);
  Function *f = Function::Create(ft, Function::ExternalLinkage, "", module);

  BasicBlock *bb = BasicBlock::Create(getGlobalContext(), "", f);
  builder.SetInsertPoint(bb);
  std::vector<Value *> args;
  for (auto &arg : f->args())
    args.push_back(&arg);
  CallInst *call0 = builder.CreateCall(func.value, args);
  CallInst *call1 = builder.CreateCall(arg.value, args);

  Value *func_p = builder.CreateLoad(funcType, call0);
  Value *stack_p = builder.CreateAdd(call0, ConstantInt::get(getGlobalContext(), APInt(32, layout.getPointerSize())));
  Value *stack = builder.CreateLoad(stackType, stack_p);

  StoreInst *store = builder.CreateStore(call1, stack, false);
  Value *stack0 = builder.CreateAdd(stack, ConstantInt::get(getGlobalContext(), APInt(32, layout.getPointerSize())));

  std::vector<Value *> args0;
  args0.push_back(stack0);
  CallInst *call = builder.CreateCall(func_p, args0);

  builder.CreateRet(call);

  verifyFunction(*f);
  return Term{f, func_type->right};
}
