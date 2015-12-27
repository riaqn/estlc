#include "codegen.hpp"
#include "exception.hpp"
#include <vector>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Function.h>

#include <cstdarg>
#include <debug.hpp>

#include <iostream>

using namespace llvm;

Codegen::Codegen()
  : context(getGlobalContext()),
    module(new Module("", context)),
    builder(context),
    layout(module) {
  module->setTargetTriple("x86_64-pc-linux-gnu");

  refType = PointerType::get(IntegerType::get(context, 8), 0);

  std::vector<Type *> elems;
  elems.push_back(refType);
  elems.push_back(refType);
  productType = StructType::get(context, elems);

  indexType = IntegerType::get(context, 32);
  elems.clear();
  elems.push_back(indexType);
  elems.push_back(refType);
  sumType = StructType::get(context, elems);
  
  stackType = PointerType::get(IntegerType::get(context, 8), 0);
  
  elems.clear();
  elems.push_back(stackType);
  funcType = FunctionType::get(refType, elems, false);
  PfuncType = PointerType::get(funcType, 0);
  
  elems.clear();
  elems.push_back(PointerType::get(funcType, 0));
  elems.push_back(stackType);
  closureType = StructType::get(context, elems);

  PclosureType = PointerType::get(closureType, 0);
    }

Codegen::Term Codegen::generate(const ast::Term *term, Env<APInt> &env) {
  Term term0;
  if (const ast::Application *app = dynamic_cast<const ast::Application *>(term))
    term0 = generate(app, env);
  else if (const ast::Abstraction *abs = dynamic_cast<const ast::Abstraction *>(term))
    term0 = generate(abs, env);
  else if (const ast::Reference *ref = dynamic_cast<const ast::Reference *>(term))
    term0 = generate(ref, env);
  else if (const ast::Desum *des = dynamic_cast<const ast::Desum *>(term))
    term0 = generate(des, env);
  else if (const ast::Deproduct *dep = dynamic_cast<const ast::Deproduct *>(term))
    term0 = generate(dep, env);
  else if (const ast::Fixpoint *fix = dynamic_cast<const ast::Fixpoint *>(term))
    term0 = generate(fix, env);
  else
    throw TermNotMatch(term, typeid(ast::Term));
  map.insert(std::make_pair(term, term0));
  return term0;
}

Codegen::Term Codegen::generate(const ast::Application *app, Env<APInt> &env) {
  Term func = generate(app->func, env);
  Term arg = generate(app->arg, env);

  //type check
  const ast::FunctionType *func_type = static_cast<const ast::FunctionType *>(func.type);
  if (func_type == NULL)
    throw ClassNotMatch(TermException(app->func, func.type), typeid(ast::FunctionType));
  if (*func_type->left != *arg.type)
    throw TypeNotMatch(TermException(app->arg, arg.type), func_type->left);

  Function *f = Function::Create(funcType, Function::ExternalLinkage, func_type->to_string(), module);
  BasicBlock *bb = BasicBlock::Create(context, "", f);
  builder.SetInsertPoint(bb);

  Value *stack = f->arg_begin();

  Value *call0 = generateEval(func.value, stack);
  generatePrintf("call0 = %p\n", call0);
  Value *call1 = generateEval(arg.value, stack);

  auto pair = generateDeclosure(call0);
  Value *func0 = pair.first;
  Value *stack0 = pair.second;

  generatePush(call1, stack0);

  Value *call = generateEval(func0, stack0);

  builder.CreateRet(call);

  verifyFunction(*f);
  return Term{f, func_type->right};
}

Codegen::Term Codegen::generate(const ast::Reference *ref, Env<APInt> &env) {
  Function *f = Function::Create(funcType, Function::ExternalLinkage, "ref " + ref->name, module);
  BasicBlock *bb = BasicBlock::Create(context, "", f);
  builder.SetInsertPoint(bb);
  Value *stack = f->arg_begin();
  Value *value;
  const ast::Type *type;

  size_t idx;
  
  int num;
  try {
    num = std::stoi(ref->name, &idx, 10);
  } catch (std::invalid_argument e) {
    idx = 0;
  }
  if (idx == ref->name.size()) {
    value = generateMalloc(IntegerType::get(context, 32));
    builder.CreateStore(ConstantInt::get(context, APInt(num, 32)), value);
    type = new ast::PrimitiveType("Int");
  } else {
    auto v = env.find(ref->name);
    
    Value *v_p = builder.CreateInBoundsGEP(stack, ConstantInt::get(context, v.first - env.size()));
    Value *v_p_c = builder.CreateBitCast(v_p, PointerType::get(refType, 0));
    
    value = builder.CreateLoad(refType, v_p_c);
    type = v.second;
  }
  builder.CreateRet(value);
  verifyFunction(*f);
  return Term{f, type};
}

Codegen::Term Codegen::generate(const ast::Abstraction *const abs, Env<APInt> &env) {
  env.push(abs->arg, abs->type, APInt(32, layout.getPointerSize()));
  Term term = generate(abs->term, env);
  env.pop();

  Function *f = Function::Create(funcType, Function::ExternalLinkage, "abs " + abs->arg, module);
  BasicBlock *bb = BasicBlock::Create(getGlobalContext(), "", f);
  builder.SetInsertPoint(bb);
  Value *stack = f->arg_begin();

  Value *stack_begin = builder.CreateGEP(stack, ConstantInt::get(context, -env.size()));
  Value *stack0_begin = generateMalloc(ConstantInt::get(context, APInt(64, 4096)));
  Value *size = ConstantInt::get(context, env.size());
  generateMemmove(stack0_begin, stack_begin, size);
  Value *stack0 = builder.CreateGEP(stack0_begin, size);

  Value *clo = generateClosure(term.value, stack0);

  builder.CreateRet(clo);
  verifyFunction(*f);
  return Term{f, new ast::FunctionType(abs->type, term.type)};

}
Codegen::Term Codegen::generate(const ast::Deproduct *const dep, Env<APInt> &env) {

  Term product = generate(dep->product, env);
  const ast::ProductType *type = dynamic_cast<const ast::ProductType *>(product.type);
  if (type == NULL)
    throw ClassNotMatch(TermException(dep->product, product.type), typeid(ast::ProductType));

  auto n = dep->names.size();
  if (type->types.size() != n)
    throw NumberNotMatch(TermException(dep->product, type), n);

  std::vector<Type *> elems;
  for (size_t i = 0; i < n; ++i) {
    elems.push_back(refType);
    env.push(dep->names[i], type->types[i], APInt(64, layout.getTypeAllocSize(refType)));
  }
  StructType *productType = StructType::get(context, elems);

  Term term = generate(dep->term, env);
  for (unsigned i = 0; i < n; ++i)
    (void)env.pop();

  Function *f = Function::Create(funcType, Function::ExternalLinkage, "dep", module);
  BasicBlock *bb = BasicBlock::Create(getGlobalContext(), "", f);
  builder.SetInsertPoint(bb);

  Value *stack = f->arg_begin();

  Value *size = ConstantInt::get(context, env.size());
  Value *stack_begin = builder.CreateGEP(stack, ConstantInt::get(context, -env.size()));
  Value *stack0_begin = generateMalloc(ConstantInt::get(context, APInt(64, 4096)));
  generateMemmove(stack0_begin, stack_begin, size);
  Value *stack0 = builder.CreateGEP(stack0_begin, size);

  
  Value *p = generateEval(product.value, stack);
  Value *p_c = builder.CreateBitCast(p, PointerType::get(productType, 0));
  Value *index[2] = {ConstantInt::get(context, APInt(32, 0))};
  for (size_t i = 0; i < n; ++i) {
    index[1] = ConstantInt::get(context, APInt(32, i));
    Value *v_p = builder.CreateGEP(p_c, index);
    Value *v = builder.CreateLoad(refType, v_p);
    generatePush(v, stack0);
  }

  Value *call = generateEval(term.value, stack0);
  builder.CreateRet(call);
  verifyFunction(*f);

  return Term{f, term.type};
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
    else if (*termtype != *term.type)
      throw TypeNotMatch(TermException(pair.second, term.type), termtype);
  }

  
  Constant *jt = ConstantVector::get(consts);

  Function *f = Function::Create(funcType, Function::ExternalLinkage, "desum ", module);
  BasicBlock *bb = BasicBlock::Create(context, "", f);
  builder.SetInsertPoint(bb);


  Value *stack = f->arg_begin();

  Value *size = ConstantInt::get(context, env.size());
  Value *stack_begin = builder.CreateGEP(stack, ConstantInt::get(context, -env.size()));
  Value *stack0_begin = generateMalloc(ConstantInt::get(context, APInt(64, 4096)));
  generateMemmove(stack0_begin, stack_begin, size);
  Value *stack0 = builder.CreateGEP(stack0_begin, size);

  Value *call = generateEval(sum.value, stack);
  Value *value = builder.CreateBitCast(call, PointerType::get(sumType, 0));
  generatePrintf("begin Desum [%p]\n", value);

  //read the first 4 bytes of index
  Value *index[2];
  index[0] = ConstantInt::get(context, APInt(32, 0));
  index[1] = ConstantInt::get(context, APInt(32, 0));
  Value *idx_p = builder.CreateGEP(value, index);
  index[1] = ConstantInt::get(context, APInt(32, 1));
  Value *ref_p = builder.CreateGEP(value, index);

  Value *idx = builder.CreateLoad(indexType, idx_p);
  Value *ref = builder.CreateLoad(refType, ref_p);

  //read the function pointer
  Value *func = builder.CreateExtractElement(jt, idx);

  //call it.

  //then we push the remainer into the stack
  generatePush(ref, stack0);
    
  Value *casecall = generateEval(func, stack0);
  builder.CreateRet(casecall);
  verifyFunction(*f);

  return Term{f, termtype};
}

void Codegen::generatePush(Value *const value, Value *&stack) {
  Value *stack_c = builder.CreateBitCast(stack, PointerType::get(value->getType(), 0));
  generatePrintf("Store [%p] ", stack_c);
  generatePrintf("= %p\n", value);
  (void)builder.CreateStore(value, stack_c);
  stack = builder.CreateInBoundsGEP(stack, ConstantInt::get(context, APInt(64, layout.getTypeAllocSize(value->getType()))));
}

LoadInst *Codegen::generatePop(Type *const type, Value *&stack) {
  stack = builder.CreateInBoundsGEP(stack, ConstantInt::get(context, APInt(64, -layout.getTypeAllocSize(type))));
  Value *stack_c = builder.CreateBitCast(stack, PointerType::get(type, 0));
  return builder.CreateLoad(type, stack_c);
}


Codegen::Term Codegen::generate(const ast::Program &prog) {
  Env<APInt> env(APInt(layout.getTypeAllocSizeInBits(refType), 0));
  std::vector<Term> funcs;
  //generate bool
  std::vector<std::pair<const ast::Type *, const std::string>> types;
  types.push_back(std::make_pair(new ast::PrimitiveType("Unit"), "false"));
  types.push_back(std::make_pair(new ast::PrimitiveType("Unit"), "true"));
  Bool = new ast::SumType(types);
  uint32_t idx = 0;
  for (auto pair : Bool->types) {
    Term term = generate(Bool, idx++);
    funcs.push_back(term);
    env.push(pair.second, term.type, APInt(64, layout.getTypeAllocSize(term.value->getType())));
  }

  std::string prims[] = {"<", ">="};
  for (auto prim : prims) {
    Term term = generatePrimitive(prim);
    funcs.push_back(term);
    env.push(prim, term.type, APInt(64, layout.getTypeAllocSize(term.value->getType())));
  }

  // generate construcotr for tyeps
  for (const ast::Type *type : prog.types) {
    std::cout << typeid(*type).name() << std::endl;
    if (auto prim = dynamic_cast<const ast::PrimitiveType *>(type)) {
      (void)prim;
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
        Term term = generate(sum, idx++);
        funcs.push_back(term);
        env.push(pair.second, term.type, APInt(64, layout.getTypeAllocSize(term.value->getType())));
      }
    } else if (auto product = dynamic_cast<const ast::ProductType *>(type)) {
      Term term = generate(product);
      funcs.push_back(term);
      env.push(product->cons, term.type, APInt(64, layout.getTypeAllocSize(term.value->getType())));
    } else {
      throw TypeException(type);
    }

  }


  Term term = generate(prog.term, env);

  Function *f = Function::Create(funcType, Function::ExternalLinkage, "umain", module);
  Value *arg = f->arg_begin();
  BasicBlock *bb = BasicBlock::Create(context, "", f);
  builder.SetInsertPoint(bb);

  Value *stack = generateMalloc(ConstantInt::get(context, APInt(64, 4096)));
  for (auto term : funcs) {
    // call the functions that will generator constructor for us.
    // the arg is NULL, since the process generating constructor does
    // not need any other informations.
    Value *clo = builder.CreateCall(term.value, ConstantPointerNull::get(stackType));
    generatePush(clo, stack);
  }
  
  //now call the main term
  CallInst *call = builder.CreateCall(term.value, {stack});
  const ast::FunctionType *type = dynamic_cast<const ast::FunctionType *>(term.type);
  if (type == NULL)
    throw new ClassNotMatch(TermException(prog.term, term.type), typeid(ast::FunctionType));
  auto pair = generateDeclosure(call);
  Value *func0 = pair.first;
  Value *stack0 = pair.second;
  
  generatePush(arg, stack0);
  Value *ret = builder.CreateCall(func0, {stack0});
  builder.CreateRet(ret);
  verifyFunction(*f);
  return Term{f, term.type};
}

Codegen::Term Codegen::generate(const ast::SumType *sum, const uint32_t idx) {
  Function *f = Function::Create(funcType, Function::ExternalLinkage, sum->types[idx].second, module);
  {
    BasicBlock *bb = BasicBlock::Create(context, "", f);
    builder.SetInsertPoint(bb);

    Value* m = generateMalloc(sumType);

    Value *stack = f->arg_begin();
    LoadInst *ref = generatePop(refType, stack);

    std::vector<Value *> idxs;
    idxs.push_back(ConstantInt::get(context, APInt(32, 0)));
    idxs.push_back(ConstantInt::get(context, APInt(32, 0)));
  
    Value *idx_p = builder.CreateGEP(m, idxs);

    idxs.clear();
    idxs.push_back(ConstantInt::get(context, APInt(32, 0)));
    idxs.push_back(ConstantInt::get(context, APInt(32, 1)));
    Value *ref_p = builder.CreateGEP(m, idxs);

    builder.CreateStore(ConstantInt::get(context, APInt(32, idx)), idx_p);
    builder.CreateStore(ref, ref_p);

    Value *m_c = builder.CreateBitCast(m, refType);
    builder.CreateRet(m_c);
    verifyFunction(*f);
  }

  Function *f0 = Function::Create(funcType, Function::ExternalLinkage, "ret " + sum->types[idx].second, module);
  BasicBlock *bb = BasicBlock::Create(context, "", f0);
  builder.SetInsertPoint(bb);
  

  Value *stack = generateMalloc(refType);
  Value *stack_c = builder.CreateBitCast(stack, refType);

  Value *clo = generateClosure(f, stack_c);

  Value *clo_c = builder.CreateBitCast(clo, refType);

  builder.CreateRet(clo_c);
  verifyFunction(*f0);
  
  ast::Type *type = new ast::FunctionType(sum->types[idx].first, sum);
  return Term{f0, type};
}

Value *Codegen::generateMalloc(Value *size) {
  generatePrintf("begin Malloc for size %u\n", size);
  static Function *malloc = NULL;
  if (malloc == NULL) {
    std::vector<Type*> types(1, IntegerType::get(context, 64));
    FunctionType *malloc_type = FunctionType::get(refType, types, false);
    malloc = Function::Create(malloc_type, Function::ExternalLinkage, "GC_malloc", module);
  }
  Value *args[1] = {size};
  CallInst *call = builder.CreateCall(malloc, args);
  generatePrintf("end Malloc = %p\n", call);
  return call;

}

Value *Codegen::generateMemmove(Value *dst, Value *src, Value *n) {
  static Function *memmove = NULL;
  if (memmove == NULL) {
    FunctionType *type = FunctionType::get(refType, {refType, refType, IntegerType::get(context, 64)}, false);
    memmove = Function::Create(type, Function::ExternalLinkage, "memmove", module);
  }
  CallInst *call = builder.CreateCall(memmove, {dst, src, n});
  return call;

}

Value *Codegen::generateMalloc(Type *type) {
  generatePrintf("begin Malloc for type\n", ConstantPointerNull::get(stackType));
  Value *m = generateMalloc(ConstantInt::get(context, APInt(64, layout.getTypeAllocSize(type))));
  generatePrintf("end Malloc = %p\n", m);
  return builder.CreateBitCast(m, PointerType::get(type, 0));  
}

Value *Codegen::generateClosure(Value *func, Value *stack) {
  generatePrintf("begin Clo [%p] ", func);
  generatePrintf("[%p]\n", stack);
  Value *m = generateMalloc(closureType);
  std::vector<Value *> idx;
  idx.push_back(ConstantInt::get(context, APInt(32, 0)));
  idx.push_back(ConstantInt::get(context, APInt(32, 0)));
  Value *func_p = builder.CreateGEP(m, idx);

  idx.clear();
  idx.push_back(ConstantInt::get(context, APInt(32, 0)));
  idx.push_back(ConstantInt::get(context, APInt(32, 1)));
  Value *stack_p = builder.CreateGEP(m, idx);

  builder.CreateStore(func, func_p);
  builder.CreateStore(stack, stack_p);
  //normally we want a refType
  Value *m_c = builder.CreateBitCast(m, refType);

  generatePrintf("end Clo [%p]\n", m);
  return m_c;
}

Codegen::Term Codegen::generate(const ast::ProductType *const product) {
  Env<APInt> env(APInt(layout.getTypeAllocSizeInBits(refType), 0));
  size_t n = product->types.size();
  std::vector<Type *> elems;
  for (unsigned i = 0; i < n; ++i) {
    env.push(std::to_string(i), product->types[i], APInt(64, layout.getTypeAllocSize(refType)));
    elems.push_back(refType);
  }

  /* generate the actually working function */
  Function *f = Function::Create(funcType, Function::ExternalLinkage, product->cons, module);
  const ast::Type *type = product;
  {
    BasicBlock *bb = BasicBlock::Create(context, "", f);
    builder.SetInsertPoint(bb);

    Type *productType = StructType::get(context, elems);

    Value *m = generateMalloc(productType);
    Value *stack = f->arg_begin();

    for (int i = n - 1; i >=0; --i) {
      type = new ast::FunctionType(product->types[i], type);
      //calculate the address in m
    
      std::vector<Value *> idx;
      idx.push_back(ConstantInt::get(context, APInt(32, 0)));
      idx.push_back(ConstantInt::get(context, APInt(32, i)));
      Value *p = builder.CreateGEP(m, idx);

      //get the value from the stack
      Value *v_p = builder.CreateGEP(stack, ConstantInt::get(context, env.find(std::to_string(i)).first - env.size()));
      Value *v_p_c = builder.CreateBitCast(v_p, PointerType::get(refType, 0));
      Value *v = generateLoad(refType, v_p_c);

      //store the value to the m
      builder.CreateStore(v, p);
    }
    Value *m_c = builder.CreateBitCast(m, refType);
    builder.CreateRet(m_c);
    verifyFunction(*f);
  }


  /* now we will generate a serialize of useless function to be
     friendly to other components */
  for (auto i = n - 1; i > 0; --i) {
    Function *f0 = Function::Create(funcType, Function::ExternalLinkage, product->cons + std::to_string(i), module);
    BasicBlock *bb = BasicBlock::Create(context, "", f0);
    builder.SetInsertPoint(bb);
    Value *stack = f0->arg_begin();

    
    Value *stack_begin = builder.CreateGEP(stack, ConstantInt::get(context, -env.find(std::to_string(i)).first));
    Value *stack0_begin = generateMalloc(ConstantInt::get(context, APInt(64, 4096)));
    Value *size = ConstantInt::get(context, env.find(std::to_string(i)).first);
    generateMemmove(stack0_begin, stack_begin, size);
    Value *stack0 = builder.CreateGEP(stack0_begin, size);

    Value *clo = generateClosure(f, stack0);
    builder.CreateRet(clo);
    verifyFunction(*f0);
    f = f0;
  }

  Function *f0 = Function::Create(funcType, Function::ExternalLinkage, "ret " + product->cons, module);
  BasicBlock *bb = BasicBlock::Create(context, "", f0);
  builder.SetInsertPoint(bb);
  Value *stack = generateMalloc(ConstantInt::get(context, env.size()));

  Value *clo = generateClosure(f, stack);
  
  builder.CreateRet(clo);
  verifyFunction(*f0);
  
  return Term{f0, type};
}

Function *Codegen::generateBinary(Function *f0) {
  Function *f = Function::Create(funcType, Function::ExternalLinkage, "binary", module);
  {
  BasicBlock *bb = BasicBlock::Create(context, "", f);
  builder.SetInsertPoint(bb);

  Value *stack = f->arg_begin();

  Value *x = generatePop(refType, stack);
  
  Value *stack0 = generateMalloc(ConstantInt::get(context, APInt(64, layout.getTypeAllocSize(refType) * 2)));
  generatePush(x, stack0);

  Value *clo = generateClosure(f0, stack0);
  builder.CreateRet(clo);
  }
  verifyFunction(*f);
  Function *f1 = Function::Create(funcType, Function::ExternalLinkage, "f1", module);
  BasicBlock *bb = BasicBlock::Create(context, "", f1);
  builder.SetInsertPoint(bb);
  Value *m = generateMalloc(ConstantInt::get(context, APInt(64, layout.getTypeAllocSize(refType))));
  Value *clo = generateClosure(f, m);
  builder.CreateRet(clo);
  verifyFunction(*f1);
  return f1;
}

Codegen::Term Codegen::generatePrimitive(const std::string &prim) {
  if (prim == "<") {
    Function *f = Function::Create(funcType, Function::ExternalLinkage, prim, module);
    BasicBlock *bb = BasicBlock::Create(context, "", f);
    builder.SetInsertPoint(bb);
    Value *stack = f->arg_begin();

    Value *y = generatePop(refType, stack);
    Value *y_v = generateLoad(IntegerType::get(context, 32), y);
    Value *x = generatePop(refType, stack);
    Value *x_v = generateLoad(IntegerType::get(context, 32), x);

    Value *res = builder.CreateICmpULT(x_v, y_v);
    Value *ret = generateSum(res, ConstantPointerNull::get(refType));
    builder.CreateRet(ret);
    verifyFunction(*f);

    return Term{generateBinary(f), new ast::FunctionType(new ast::PrimitiveType("Int"),
                                                     new ast::FunctionType(new ast::PrimitiveType("Int"),
                                                                           Bool))};
  } else if (prim == ">=") {
    Function *f = Function::Create(funcType, Function::ExternalLinkage, prim, module);
    BasicBlock *bb = BasicBlock::Create(context, "", f);
    builder.SetInsertPoint(bb);
    Value *stack = f->arg_begin();

    Value *y = generatePop(refType, stack);
    Value *y_v = generateLoad(IntegerType::get(context, 32), y);
    Value *x = generatePop(refType, stack);
    Value *x_v = generateLoad(IntegerType::get(context, 32), x);

    Value *res = builder.CreateICmpUGE(x_v, y_v);
    Value *ret = generateSum(res, ConstantPointerNull::get(refType));
    builder.CreateRet(ret);
    verifyFunction(*f);

    return Term{generateBinary(f), new ast::FunctionType(new ast::PrimitiveType("Int"),
                                                     new ast::FunctionType(new ast::PrimitiveType("Int"),
                                                                           Bool))};
  }
  
  return Term{NULL, NULL};
}
  

Codegen::Term Codegen::generate(const ast::Fixpoint *const fix, Env<llvm::APInt> &env) {
  const ast::Abstraction *abs = dynamic_cast<const ast::Abstraction *>(fix->term);
  if (abs == NULL)
    throw TermNotMatch(fix->term, typeid(ast::Abstraction));
  env.push(abs->arg, abs->type, APInt(64, layout.getTypeAllocSize(refType)));
  Term term = generate(abs->term, env);
  if (*term.type != *abs->type)
    throw TypeNotMatch(TermException(abs->term, term.type), abs->type);

  const ast::FunctionType *type = dynamic_cast<const ast::FunctionType *>(term.type);
  if (type == NULL)
    throw ClassNotMatch(TermException(abs->term, term.type), typeid(ast::FunctionType));

  Value *Gclo_p = new GlobalVariable(*module,
                                     refType,
                                      false,
                                     GlobalValue::InternalLinkage,
                                     ConstantPointerNull::get(PclosureType),
                                      "Gclo " + abs->arg);

  Function *co = Function::Create(funcType, Function::ExternalLinkage, "co " + abs->arg, module);
  {
    BasicBlock *bb = BasicBlock::Create(context, "", co);
    builder.SetInsertPoint(bb);
    Value *stack = co->arg_begin();

    Value *x = generatePop(refType, stack);
    Value *stack0 = generatePop(stackType, stack);

    Value *clo = generateEval(term.value, stack0);
    auto pair = generateDeclosure(clo);
    Value *func1 = pair.first;
    Value *stack1 = pair.second;

    Value *Gclo = builder.CreateLoad(refType, Gclo_p);
    generatePush(Gclo, stack1);
    generatePush(x, stack1);

    Value *ret = generateEval(func1, stack1);
    builder.CreateRet(ret);
  }
  verifyFunction(*co);

  Function *f = Function::Create(funcType, Function::ExternalLinkage, "fix " + abs->arg, module);
  BasicBlock *bb = BasicBlock::Create(context, "", f);
  builder.SetInsertPoint(bb);
  Value *stack = f->arg_begin();
  
  Value *stack_co = generateMalloc(ConstantInt::get(context, APInt(64, layout.getTypeAllocSize(refType) * 4)));
  generatePush(stack, stack_co);
  
  
  Value *clo = generateClosure(co, stack_co);
  builder.CreateStore(clo, Gclo_p);

  builder.CreateRet(clo);
  verifyFunction(*f);
  env.pop();
  return Term{f, abs->type};
}

std::pair<Value *, Value *> Codegen::generateDeclosure(Value *clo) {
  generatePrintf("begin Declo [%p]\n", clo);
  Value *clo_c = builder.CreateBitCast(clo, PclosureType);
  Value *index[2] = {ConstantInt::get(context, APInt(32, 0))};
  index[1] = ConstantInt::get(context, APInt(32, 0));
  Value *func_p = builder.CreateGEP(clo_c, index);
  index[1] = ConstantInt::get(context, APInt(32, 1));
  Value *stack_p = builder.CreateGEP(clo_c, index);

  generatePrintf("Load [%p]\n", func_p);
  Value *func = builder.CreateLoad(PfuncType, func_p);
  generatePrintf("Load [%p]\n", stack_p);
  Value *stack = builder.CreateLoad(stackType, stack_p);
  generatePrintf("end Declo [%p]\n", clo);
  return std::make_pair(func, stack);
}

Value *Codegen::generatePrintf(const char *const fmt, Value *val) {
  static Function *printf = NULL;
  if (printf == NULL) {
    FunctionType *printfType = FunctionType::get(IntegerType::get(context, 32),
                                                 {PointerType::get(IntegerType::get(context, 8), 0)},
                                                 true);
    printf = Function::Create(printfType, Function::ExternalLinkage, "printf", module);
  }
  std::vector<Value *> args;
  args.push_back(builder.CreateGlobalStringPtr(fmt));
  args.push_back(val);
  return builder.CreateCall(printf, args);
}

Value *Codegen::generateEval(Value *eval, Value *stack) {
  generatePrintf("begin eval [%p]", eval);
  generatePrintf("with stack %p\n", stack);
  Value *ret = builder.CreateCall(eval, {stack});
  generatePrintf("end eval [%p] ", eval);
  generatePrintf("= %p\n", ret);
  return ret;
}

Value *Codegen::generateLoad(Type *type, Value *ptr) {
  //comment this out since this disables LLVM type checking.
  //very dangerous.
  Value *ptr_c = builder.CreateBitCast(ptr, PointerType::get(type, 0));
  Value *val = builder.CreateLoad(type, ptr_c);
  return val;
}

void Codegen::dump() {
  module->dump();
}

Value *Codegen::generateSum(Value *idx, Value *ref) {
  Value *sum = generateMalloc(sumType);
  Value *index[2] = {ConstantInt::get(context, APInt(32, 0))};
  
  index[1] = ConstantInt::get(context, APInt(32, 0));
  Value *idx_p = builder.CreateGEP(sum, index);
  index[1] = ConstantInt::get(context, APInt(32, 1));
  Value *ref_p = builder.CreateGEP(sum, index);

  Value *idx_c = builder.CreateIntCast(idx, indexType, false);
  builder.CreateStore(idx_c, idx_p);
  builder.CreateStore(ref, ref_p);

  Value *sum_c = builder.CreateBitCast(sum, refType);

  return sum_c;
}
