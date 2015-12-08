#include "exception.hpp"

TermException::TermException(const ast::Term *const term,
                             const ast::Type *const type)
  :term_(term), type_(type) {}

TypeNotMatch::TypeNotMatch(const TermException &exception,
                           const ast::Type *const expect)
  :TermException(exception), expect_(expect) {}

NotFunction::NotFunction(const TermException &exception)
  :TermException(exception) {}
