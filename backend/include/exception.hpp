#include <ast.hpp>
#include <typeinfo>

class TermException : public std::exception {
public:
  TermException(const ast::Term *const term, const ast::Type *const type);
  const ast::Term *const term_;
  const ast::Type *const type_;
};

class TypeNotMatch : public TermException {
public:
  TypeNotMatch(const TermException &exception, const ast::Type *const expect);
  const ast::Type *const expect_;
};

class ClassNotMatch : public TermException {
public:
  ClassNotMatch(const TermException &exception, const std::type_info &expect);
  const std::type_info &expect_;
};

class TermNotMatch : public std::exception {
public:
  TermNotMatch(const ast::Term *const term, const std::type_info &expect);
  const ast::Term *term_;
  const std::type_info &expect_;
};

class NumberNotMatch : public TermException {
public:
  NumberNotMatch(const TermException &exception, const size_t expect);
  const size_t expect_;
};

class TypeException : public std::exception {
public:
  TypeException(const ast::Type *type);
  const ast::Type *type_;
};
