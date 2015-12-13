#include "exception.hpp"

TermException::TermException(const ast::Term *const term,
                             const ast::Type *const type)
  :term_(term), type_(type) {}

TypeNotMatch::TypeNotMatch(const TermException &exception,
                           const ast::Type *const expect)
  :TermException(exception), expect_(expect) {}

ClassNotMatch::ClassNotMatch(const TermException &exception,
                             const std::type_info &expect)
  :TermException(exception), expect_(expect) {}

NumberNotMatch::NumberNotMatch(const TermException &exception,
                               const size_t expect)
  :TermException(exception), expect_(expect) {}

TypeException::TypeException(const ast::Type *type)
  :type_(type) {}
