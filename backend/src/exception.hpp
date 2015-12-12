#include <ast.hpp>
#include <typeinfo>

class TermException : public std::exception {
public:
  TermException(const ast::Term *const term, const ast::Type *const type);
private:
  const ast::Term *const term_;
  const ast::Type *const type_;
};

class TypeNotMatch : public TermException {
public:
  TypeNotMatch(const TermException &exception, const ast::Type *const expect);
private:
  const ast::Type *const expect_;
};

class ClassNotMatch : public TermException {
public:
  ClassNotMatch(const TermException &exception, const std::type_info &expect);
private:
  const std::type_info &expect_;
};

class NumberNotMatch : public TermException {
public:
  NumberNotMatch(const TermException &exception, const size_t expect);
private:
  const size_t expect;
};

class TypeException : public std::exception {
public:
  TypeException(const ast::Type *type);
private:
  const ast::Type *type;
};
