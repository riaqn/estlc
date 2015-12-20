#pragma once
#include <vector>
#include <string>
#include <utility>
#include <exception>

namespace ast{
  class Exception : public std::exception {
  };

  struct PrimitiveType;
  struct SumType;
  struct ProductType;
  struct FunctionType;
  
  struct Type {
    virtual ~Type() = 0;
    virtual bool operator ==(const Type &b) const;
    virtual bool operator ==(const PrimitiveType &b) const;
    virtual bool operator ==(const SumType &b) const;
    virtual bool operator ==(const ProductType &b) const;
    virtual bool operator ==(const FunctionType &b) const;
    virtual bool operator !=(const Type &b) const;
    virtual std::string to_string() const = 0;
  };

  struct PrimitiveType : public Type {
    const std::string name;
    PrimitiveType(const std::string& name);
    virtual bool operator ==(const PrimitiveType &b) const;
    virtual std::string to_string() const ;
  };

  struct SumType : public Type {
    /* 
       pair is the type and the converter from subtypes to supertype
    */
    std::vector<std::pair<const Type *, const std::string> > types;
    SumType(const std::vector<std::pair<const Type *, const std::string> > &);
    bool operator==(const SumType &b) const;
    virtual std::string to_string() const;
  };
  
  struct ProductType : public Type {
    const std::string cons;
    std::vector<const Type *> types;
    ProductType(const std::vector<const Type *> &types, const std::string &cons);
    bool operator==(const ProductType &b) const;
    virtual std::string to_string() const;
  };

  struct FunctionType : public Type {
    const Type *left, *right;
    FunctionType(const Type *const left, const Type *const right);
    ~FunctionType();
    bool operator==(const FunctionType &b) const;
    virtual std::string to_string() const;
  };

  struct Term {
    unsigned nrow, ncol;
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
    Desum(const Term *sum, const std::vector<std::pair<const std::string, const Term *> > &cases);
  };

  struct Deproduct : public Term {
    const Term *product;
    std::vector<std::string> names;
    const Term *term;
    Deproduct(const Term *const product, const std::vector<std::string> &names, const Term *const term);
  };

  struct Fixpoint : public Term {
    //this term must has type A->A
    const Term *term;
    Fixpoint(const Term *term);
  };

  struct Fixpoint : public Term {
    //this term must has type A->A
    const Term *term;
    Fixpoint(const Term *term);
  };

  struct Program {
    std::vector<const Type *> types;
    const Term *term;
    Program(const std::vector<const Type *> types, const Term *term);
  };
}

