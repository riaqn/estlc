#pragma once
#include <vector>
#include <string>
#include <utility>

namespace ast{
  struct Type {
    virtual ~Type() = 0;
  };

  struct PrimitiveType : public Type {
    const std::string name;
    PrimitiveType(const std::string& name);
  };

  struct SumType : public Type {
    /* 
       pair is the type and the converter from subtypes to supertype
    */
    std::vector<std::pair<const Type *, const std::string> > types;
    SumType(std::vector<std::pair<const Type *, const std::string> > &);
  };
  
  struct ProductType : public Type {
    const Type *x, *y;
    const std::string cons;
    ProductType(const Type *const x, const Type *const y, const std::string &cons);
  };

  struct FunctionType : public Type {
    const Type *left, *right;
    FunctionType(const Type *const left, const Type *const right);
    ~FunctionType();
  };

  struct Term {
    virtual ~Term() = 0;
  };

  struct Reference : public Term {
    const std::string name;
    Reference(const std::string &name);
  };

  struct Abstraction : public Term {
    const std::string arg;
    const Type* type; //type of arg
    const Term* term;
    Abstraction(const std::string& arg, const Type* const type, const Term* const term);
    ~Abstraction();
  };

  struct Application : public Term {
    const Term* func;
    const Term* arg;
    Application(const Term* const func, const Term* const arg);
    ~Application();
  };

  struct Desum : public Term {
    /* 
       the terms corresponding to subtypes
    */
    const Term *sum;
    std::vector<std::pair<const std::string, const Term *> > cases;
  };

  struct Deproduct : public Term {
    const Term *product;
    const std::string x, y;
    const Term *term;
  };

  struct Program {
    const std::vector<const Type *> types;
    const Term *term;
    Program(const std::vector<const Type *> types, const Term *term);
  };
}

