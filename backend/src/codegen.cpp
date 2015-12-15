#include "codegen.hpp"
#include "exception.hpp"
#include <vector>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Function.h>

#include <iostream>

using namespace llvm;

Codegen::Codegen()
  : context(getGlobalContext()),
    module(new Module("", context)),
    builder(context),
    layout(module) {

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
  
  elems.clear();
  elems.push_back(PointerType::get(funcType, 0));
  elems.push_back(stackType);
  closureType = StructType::get(context, elems);
    }

Codegen::Term Codegen::generate(const ast::Term *term, Env<APInt> &env) {
  if (const ast::Application *app = dynamic_cast<const ast::Application *>(term))
    return generate(app, env);
  else if (const ast::Abstraction *abs = dynamic_cast<const ast::Abstraction *>(term))
    return generate(abs, env);
  else if (const ast::Reference *ref = dynamic_cast<const ast::Reference *>(term))
    return generate(ref, env);
  else if (const ast::Desum *des = dynamic_cast<const ast::Desum *>(term))
    return generate(des, env);
  else if (const ast::Deproduct *dep = dynamic_cast<const ast::Deproduct *>(term))
    return generate(dep, env);
  else
    throw ClassNotMatch(TermException(term, new ast::PrimitiveType("WTF")), typeid(ast::Term));
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

  Function *f = Function::Create(funcType, Function::ExternalLinkage, "", module);
  BasicBlock *bb = BasicBlock::Create(context, "", f);
  builder.SetInsertPoint(bb);
  
  Value *args[1] = {f->arg_begin()};
  CallInst *call0 = builder.CreateCall(func.value, args);
  Value *call1 = builder.CreateCall(arg.value, args);

  Value *func_p = builder.CreateLoad(funcType, call0);
  Value *stack_p = builder.CreateAdd(call0, ConstantInt::get(context, APInt(64, layout.getTypeAllocSize(funcType))));
  Value *stack = builder.CreateLoad(stackType, stack_p);

  generatePush(call1, stack);

  Value *args0[1] = {stack};
  CallInst *call = builder.CreateCall(func_p, args0);

  builder.CreateRet(call);

  verifyFunction(*f);
  return Term{f, func_type->right};
}

Codegen::Term Codegen::generate(const ast::Reference *ref, Env<APInt> &env) {
  Function *f = Function::Create(funcType, Function::ExternalLinkage, ref->name, module);
  BasicBlock *bb = BasicBlock::Create(context, "", f);
  builder.SetInsertPoint(bb);

  auto value = env.find(ref->name);
  Value *stack = f->arg_begin();

  Value *v_p = builder.CreateInBoundsGEP(stack, ConstantInt::get(context, value.first - env.size()));
  Value *v_p_c = builder.CreateBitCast(v_p, PointerType::get(refType, 0));

  LoadInst *load = builder.CreateLoad(refType, v_p_c);
  builder.CreateRet(load);
  verifyFunction(*f);
  return Term{f, value.second};
}

Codegen::Term Codegen::generate(const ast::Abstraction *const abs, Env<APInt> &env) {
  env.push(abs->arg, abs->type, APInt(32, layout.getPointerSize()));
  Term term = generate(abs->term, env);
  env.pop();

  Function *f = Function::Create(funcType, Function::ExternalLinkage, "abs " + abs->arg, module);
  BasicBlock *bb = BasicBlock::Create(getGlobalContext(), "", f);
  builder.SetInsertPoint(bb);
  Value *m = generateMalloc(closureType);
  std::vector<Value *> idx;
  idx.push_back(ConstantInt::get(context, APInt(32, 0)));
  idx.push_back(ConstantInt::get(context, APInt(32, 0)));
  Value *func_p = builder.CreateGEP(m, idx);
  builder.CreateStore(term.value, func_p);

  idx.clear();
  idx.push_back(ConstantInt::get(context, APInt(32, 0)));
  idx.push_back(ConstantInt::get(context, APInt(32, 1)));
  Value *stack_p = builder.CreateGEP(m, idx);
  builder.CreateStore(f->arg_begin(), stack_p);

  Value *m_c = builder.CreateBitCast(m, refType);
  builder.CreateRet(m_c);
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

  Function *f = Function::Create(funcType, Function::ExternalLinkage, "dep", module);
  BasicBlock *bb = BasicBlock::Create(getGlobalContext(), "", f);
  builder.SetInsertPoint(bb);

  Value *stack = f->arg_begin();
  
  CallInst *p = builder.CreateCall(product.value, stack);
  Value *p_c = builder.CreateBitCast(p, PointerType::get(productType, 0));
  for (size_t i = 0; i < n; ++i) {
    Value *v_p = builder.CreateGEP(p_c, ConstantInt::get(context, APInt(64, i)));
    Value *v = builder.CreateLoad(refType, v_p);
    generatePush(v, stack);
  }

  CallInst *call = builder.CreateCall(term.value, {stack});
  builder.CreateRet(call);
  verifyFunction(*f);

  for (unsigned i = 0; i < n; ++i)
    (void)env.pop();
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

void Codegen::generatePush(Value *const value, Value *&stack) {
  Value *stack_c = builder.CreateBitCast(stack, PointerType::get(value->getType(), 0));
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
  // generate construcotr for tyeps
  for (const ast::Type *type : prog.types) {
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
    Value *args[1] = {stack};
    Value *clo = builder.CreateCall(term.value, args);
    generatePush(clo, stack);
  }
  
  //now call the main term
  CallInst *call = builder.CreateCall(term.value, {stack});
  const ast::FunctionType *type = dynamic_cast<const ast::FunctionType *>(term.type);
  if (type == NULL)
    throw new ClassNotMatch(TermException(prog.term, term.type), typeid(ast::FunctionType));
  Value *clo = builder.CreateBitCast(call, PointerType::get(closureType, 0));
  Value *idx[2];
  idx[0] = ConstantInt::get(context, APInt(32, 0));
  idx[1] = ConstantInt::get(context, APInt(32, 0));
  Value *func_p = builder.CreateGEP(clo, idx);
  idx[1] = ConstantInt::get(context, APInt(32, 1));
  Value *stack_p = builder.CreateGEP(clo, idx);
  Value *func0 = builder.CreateLoad(PointerType::get(funcType, 0), func_p);
  Value *stack0 = builder.CreateLoad(stackType, stack_p);
  
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

  Function *f0 = Function::Create(funcType, Function::ExternalLinkage, "", module);
  BasicBlock *bb = BasicBlock::Create(context, "", f0);
  builder.SetInsertPoint(bb);
  

  //FIXME: we assume stack space needed for two arguments is smaller
  //than the sumtype
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
  static Function *malloc = NULL;
  if (malloc == NULL) {
    std::vector<Type*> types(1, IntegerType::get(context, 64));
    FunctionType *malloc_type = FunctionType::get(refType, types, false);
    malloc = Function::Create(malloc_type, Function::ExternalLinkage, "malloc", module);
  }
  Value *args[1] = {size};
  CallInst *call = builder.CreateCall(malloc, args);
  return call;

}

Value *Codegen::generateMalloc(Type *type) {
  Value *m = generateMalloc(ConstantInt::get(context, APInt(64, layout.getTypeAllocSize(type))));
  return builder.CreateBitCast(m, PointerType::get(type, 0));  
}

Value *Codegen::generateClosure(Value *func, Value *stack) {
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

  return m;
}

Codegen::Term Codegen::generate(const ast::ProductType *const product) {
  Env<APInt> env(APInt(layout.getTypeAllocSizeInBits(refType), 0));
  unsigned i = 0;
  std::vector<Type *> elems;
  for (auto type : product->types) {
    env.push(std::to_string(i++), type, APInt(64, layout.getTypeAllocSize(refType)));
    elems.push_back(refType);
  }

  /* generate the actually working function */
  Function *f = Function::Create(funcType, Function::ExternalLinkage, product->cons, module);
  const ast::Type *type = product;
  {
    BasicBlock *bb = BasicBlock::Create(getGlobalContext(), "", f);
    builder.SetInsertPoint(bb);

    Type *productType = StructType::get(context, elems);

    Value *m = generateMalloc(productType);
    Value *stack = f->arg_begin();

    for (int i = product->types.size() - 1; i >= 0; --i) {
      type = new ast::FunctionType(product->types[i], type);
      //calculate the address in m
    
      std::vector<Value *> idx;
      idx.push_back(ConstantInt::get(context, APInt(32, 0)));
      idx.push_back(ConstantInt::get(context, APInt(32, i)));
      Value *p = builder.CreateGEP(m, idx);

      //get the value from the stack
      Value *v = generatePop(refType, stack);

      //store the value to the m
      builder.CreateStore(v, p);
    }
    Value *m_c = builder.CreateBitCast(m, refType);
    builder.CreateRet(m_c);
    verifyFunction(*f);
  }


  /* now we will generate a serialize of useless function to be
     friendly to other components */
  for (auto i = product->types.size() - 1; i > 0; --i) {
    Function *f0 = Function::Create(funcType, Function::ExternalLinkage, product->cons + std::to_string(i), module);
    BasicBlock *bb = BasicBlock::Create(getGlobalContext(), "", f0);
    builder.SetInsertPoint(bb);

    Value *clo = generateClosure(f, f0->arg_begin());
    Value *clo_c = builder.CreateBitCast(clo, refType);
    builder.CreateRet(clo_c);
    verifyFunction(*f0);
    f = f0;
  }

  Function *f0 = Function::Create(funcType, Function::ExternalLinkage, "", module);
  BasicBlock *bb = BasicBlock::Create(context, "", f0);
  builder.SetInsertPoint(bb);
  Value *stack = generateMalloc(ConstantInt::get(context, APInt(64,(layout.getTypeAllocSize(refType) * product->types.size()))));

  Value *clo = generateClosure(f, stack);
  Value *clo_c = builder.CreateBitCast(clo, refType);
  
  builder.CreateRet(clo_c);
  verifyFunction(*f0);
  
  return Term{f0, type};
}


void Codegen::dump() {
  module->dump();
}
