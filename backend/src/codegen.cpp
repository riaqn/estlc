#include "codegen.hpp"
#include "exception.hpp"
#include <vector>
#include <llvm/IR/Verifier.h>

using namespace llvm;

Codegen::Codegen()
  : module(new Module("", getGlobalContext())),
    builder(getGlobalContext()),
    layout(module),
    context(getGlobalContext()) {
  refType = PointerType::get(Type::getVoidTy(context), 0);
  
  std::vector<Type *> elems;
  elems.push_back(refType);
  elems.push_back(refType);
  productType = StructType::get(context, elems);

  indexType = IntegerType::get(context, 32);
  elems.clear();
  elems.push_back(indexType);
  elems.push_back(refType);
  sumType = StructType::get(context, elems);

  elems.clear();
  elems.push_back(stackType);
  funcType = FunctionType::get(refType, elems, false);

  stackType = PointerType::get(refType, 0);
  
  elems.clear();
  elems.push_back(funcType);
  elems.push_back(stackType);
  closureType = StructType::get(getGlobalContext(), elems);
}

Codegen::Term Codegen::generate(const ast::Term *term, Env<APInt> &env) {
  if (const ast::Application *app = dynamic_cast<const ast::Application *>(term))
    return generate(app, env);
  else if (const ast::Abstraction *abs = dynamic_cast<const ast::Abstraction *>(term))
    return generate(abs, env);
}

Codegen::Term Codegen::generate(const ast::Application *app, Env<APInt> &env) {
  Term func = generate(app->func, env);
  Term arg = generate(app->arg, env);

  //type check
  const ast::FunctionType *func_type = static_cast<const ast::FunctionType *>(func.type);
  if (func_type == NULL)
    throw ClassNotMatch(TermException(app->func, func.type), typeid(ast::FunctionType));
  if (func_type->left != arg.type)
    throw TypeNotMatch(TermException(app->arg, arg.type), func_type->left);

  FunctionType *ft = FunctionType::get(refType, argsType, false);
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

  (void)builder.CreateStore(call1, stack, false);
  Value *stack0 = builder.CreateAdd(stack, ConstantInt::get(getGlobalContext(), APInt(32, layout.getPointerSize())));

  std::vector<Value *> args0;
  args0.push_back(stack0);
  CallInst *call = builder.CreateCall(func_p, args0);

  builder.CreateRet(call);

  verifyFunction(*f);
  return Term{f, func_type->right};
}

Codegen::Term Codegen::generate(const ast::Reference *ref, Env<APInt> &env) {
  FunctionType *ft = FunctionType::get(refType, argsType, false);
  Function *f = Function::Create(ft, Function::ExternalLinkage, "", module);
  BasicBlock *bb = BasicBlock::Create(getGlobalContext(), "", f);
  builder.SetInsertPoint(bb);

  auto value = env.find(ref->name);
  Argument *stack = f->arg_begin();
  Value *value_p = builder.CreateSub(stack, ConstantInt::get(getGlobalContext(), env.size() - value.first));
  LoadInst *load = builder.CreateLoad(refType, value_p);
  builder.CreateRet(load);
  verifyFunction(*f);
  return Term{f, value.second};
}

Codegen::Term Codegen::generate(const ast::Abstraction *const abs, Env<APInt> &env) {
  env.push(abs->arg, abs->type, APInt(32, layout.getPointerSize()));
  Term term = generate(abs->term, env);
  env.pop();
  return Term{term.value, new ast::FunctionType(abs->type, term.type)};
}

Codegen::Term Codegen::generate(const ast::Deproduct *const dep, Env<APInt> &env) {
  Term term =  generate(dep->product, env);
  const ast::ProductType *type = dynamic_cast<const ast::ProductType *>(term.type);
  if (type == NULL)
    throw ClassNotMatch(TermException(dep->product, term.type), typeid(ast::ProductType));

  FunctionType *ft = FunctionType::get(refType, argsType, false);
  Function *f = Function::Create(ft, Function::ExternalLinkage, "", module);
  BasicBlock *bb = BasicBlock::Create(getGlobalContext(), "", f);
  builder.SetInsertPoint(bb);

  std::vector<Value *> args;
  for (auto &i : f->args())
    args.push_back(&i);

  Value *stack = f->arg_begin();
  CallInst *product = builder.CreateCall(term.value, args);
  //get x
  LoadInst *x = builder.CreateLoad(refType, product);
  APInt size_x(64, layout.getTypeAllocSize(x->getType()));
  //put x onto the stack
  (void)builder.CreateStore(x, stack);
  stack = builder.CreateAdd(stack, ConstantInt::get(getGlobalContext(), size_x));

  //calculate the address of y
  Value *y_p = builder.CreateAdd(product, ConstantInt::get(getGlobalContext(), size_x));
  //get y
  LoadInst *y = builder.CreateLoad(refType, y_p);
  APInt size_y(64, layout.getTypeAllocSize(y->getType()));
  //put y onto stack
  (void)builder.CreateStore(y, stack);
  stack = builder.CreateAdd(stack, ConstantInt::get(getGlobalContext(), size_y));
  
  env.push(dep->x, type->x, size_x);
  env.push(dep->y, type->y, size_y);
  Term term0 = generate(dep->term, env);
  env.pop();
  env.pop();

  args.clear();
  args.push_back(stack);
  CallInst *call = builder.CreateCall(term0.value, args);
  builder.CreateRet(call);
  verifyFunction(*f);
  return Term{f, term0.type};
}

Codegen::Term Codegen::generate(const ast::Desum *const des, Env<APInt> &env) {
  Term sum = generate(des->sum, env);
  auto type = dynamic_cast<const ast::SumType *>(sum.type);
  if (type == NULL)
    throw ClassNotMatch(TermException(des->sum, sum.type), typeid(ast::SumType));
  size_t n = type->types.size();
  if (n != des->cases.size())
    throw NumberNotMatch(TermException(des->sum, sum.type), des->cases.size());

  std::vector<Constant *> consts;
  const ast::Type *termtype = NULL;
  for (size_t i = 0; i < n ; ++i) {
    std::pair<const std::string, const ast::Term *> pair = des->cases[i];
    env.push(pair.first, type->types[i].first, APInt(64, layout.getTypeAllocSize(refType)));
    Term term = generate(pair.second, env);
    env.pop();
    consts.push_back(term.value);
    if (termtype == NULL)
      termtype = term.type;
    else if (termtype != term.type)
      throw TypeNotMatch(TermException(pair.second, term.type), termtype);
  }

  Constant *jt = ConstantArray::get(ArrayType::get(funcType, n), consts);

  FunctionType *ft = FunctionType::get(refType, argsType, false);
  Function *f = Function::Create(ft, Function::ExternalLinkage, "", module);
  BasicBlock *bb = BasicBlock::Create(getGlobalContext(), "", f);
  builder.SetInsertPoint(bb);

  std::vector<Value *> args;
  for (auto &i : f->args())
    args.push_back(&i);
  CallInst *call = builder.CreateCall(sum.value, args);

  //read the first 4 bytes of index
  LoadInst *idx = builder.CreateLoad(IntegerType::get(context, layout.getTypeAllocSize(indexType)),
                                     call);

  //multiply it with the length of function pointer
  Value *offset = builder.CreateMul(idx, ConstantInt::get(IntegerType::get(getGlobalContext(), 64), layout.getTypeAllocSize(funcType)));

  //add it to the jumptable
  Value *addr = builder.CreateAdd(jt, offset);

  //read the function pointer
  LoadInst *func = builder.CreateLoad(funcType, addr);

  //call it.

  //first we calculate the address of the remnant,
  // aka, the sub type data
  Value *remnant = builder.CreateAdd(call, ConstantInt::get(context, APInt(64, layout.getTypeAllocSize(indexType))));
  
  //then we push the remainer into the stack
  Value *stack = f->arg_begin();
  (void)builder.CreateStore(remnant, stack);
  stack = builder.CreateAdd(stack, ConstantInt::get(getGlobalContext(), APInt(64, layout.getTypeAllocSize(remnant->getType()))));
    
  args.clear();
  args.push_back(stack);
  CallInst *casecall = builder.CreateCall(func, args);
  builder.CreateRet(casecall);
  verifyFunction(*f);

  return Term{f, termtype};
}

Value *Codegen::generatePush(Value *value, Value *stack) {
  (void)builder.CreateStore(value, stack);
  stack = builder.CreateAdd(stack, ConstantInt::get(context, APInt(64, layout.getTypeAllocSize(value->getType()))));
  return stack;
}

Codegen::Term Codegen::generate(const ast::Program &prog) {
  Env<APInt> env;
  Function *f = Function::Create(funcType, Function::ExternalLinkage, "umain", module);
  BasicBlock *bb = BasicBlock::Create(context, "", f);
  builder.SetInsertPoint(bb);

  Value *stack = f->arg_begin();
  
  // generate construcotr for tyeps
  for (const ast::Type *type : prog.types) {
    if (auto prim = dynamic_cast<const ast::PrimitiveType *>(type)) {
      //do nothing for the primitive type
    } else if (auto sum = dynamic_cast<const ast::SumType *>(type)) {
      //what to do with the sum type?
      //there 're some constructors
      size_t n = sum->types.size();
      if ((n >> layout.getTypeAllocSizeInBits(indexType)) > 1) {
        // the idxlen is too short
        throw TypeException(sum);
      }
      uint32_t idx = 0;
      for (auto pair : sum->types) {
        const std::string cons = pair.second;
        Term term = generate(sum, idx++);
        env.push(cons, term.type, APInt(64, layout.getTypeAllocSize(term.value->getType())));
        stack = generatePush(term.value, stack);
      }
    } else if (auto product = dynamic_cast<const ast::ProductType *>(type)) {
      Term term = generate(product);
      stack = generatePush(term.value, stack);
    } else {
      throw TypeException(type);
    }
  }

  Term term = generate(prog.term, env);
  /*
  //make sure it's list_nat -> list_nat
  if (*term.type != umain_type)
    throw TypeNotMatch(TermException(prog.term, term.type),
  &umain_type);
  */
  
  //now call the main term
  Value *args[1] = {stack};
  CallInst *call = builder.CreateCall(term.value, args);
  builder.CreateRet(call);
  verifyFunction(*f);
  return Term{f, term.type};
}

Codegen::Term Codegen::generate(const ast::SumType *sum, const uint32_t idx) {
  Function *f = Function::Create(funcType, Function::ExternalLinkage, "", module);
  BasicBlock *bb = BasicBlock::Create(getGlobalContext(), "", f);
  builder.SetInsertPoint(bb);

  Type* ITy = Type::getInt32Ty(context);
  Constant* AllocSize = ConstantExpr::getSizeOf(sumType);
  AllocSize = ConstantExpr::getTruncOrBitCast(AllocSize, ITy);
  Instruction* m = CallInst::CreateMalloc(builder.GetInsertBlock(),
                                          ITy, sumType, AllocSize);

  Value *stack = f->arg_begin();
  Value *x_p = builder.CreateSub(stack, ConstantInt::get(context, APInt(64, layout.getTypeAllocSize(refType))));
  LoadInst *x = builder.CreateLoad(refType, x_p);

  Value *idx_p_ = m;
  Value *x_p_ = builder.CreateAdd(m, ConstantInt::get(context, APInt(64, layout.getTypeAllocSize(indexType))));

  (void)builder.CreateStore(ConstantInt::get(indexType, idx), idx_p_);
  (void)builder.CreateStore(x, x_p_);
  
  builder.CreateRet(m);
  verifyFunction(*f);
  ast::Type *type = new ast::FunctionType(sum->types[idx].first, sum);
  return Term{f, type};
}

Codegen::Term Codegen::generate(const ast::ProductType *const product) {
  Function *f = Function::Create(funcType, Function::ExternalLinkage, "", module);
  BasicBlock *bb = BasicBlock::Create(getGlobalContext(), "", f);
  builder.SetInsertPoint(bb);
  // need malloc here
  Type* ITy = Type::getInt32Ty(context);
  Constant* AllocSize = ConstantExpr::getSizeOf(productType);
  AllocSize = ConstantExpr::getTruncOrBitCast(AllocSize, ITy);
  Instruction* m = CallInst::CreateMalloc(builder.GetInsertBlock(),
                                               ITy, productType, AllocSize);

  Value *stack = f->arg_begin();
  Value *x_p = builder.CreateSub(stack, ConstantInt::get(context, APInt(64, layout.getTypeAllocSize(refType) * 2)));
  Value *y_p = builder.CreateSub(stack, ConstantInt::get(context, APInt(64, layout.getTypeAllocSize(refType))));

  LoadInst *x = builder.CreateLoad(refType, x_p);
  LoadInst *y = builder.CreateLoad(refType, y_p);

  Value *x_p_ = m;
  Value *y_p_ = builder.CreateAdd(m, ConstantInt::get(context, APInt(64, layout.getTypeAllocSize(refType))));
  (void)builder.CreateStore(x, x_p_);
  (void)builder.CreateStore(y, y_p_);

  builder.CreateRet(m);
  verifyFunction(*f);
  ast::Type *t = new ast::FunctionType(product->y, product);
  ast::Type *t0 = new ast::FunctionType(product->x, t);
  return Term{f, t0};
}
