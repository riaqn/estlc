#include <map>
#include <vector>
#include <tuple>
#include <string>
#include <ast.hpp>
#include <exception>

template<typename ptr_t>
class Env {
public:
  class NotFound : public std::exception {};
  
  ptr_t push(const std::string &name, const ast::Type *const type, const ptr_t size);
  void pop();
  std::pair<const ptr_t , const ast::Type *> find(const std::string &name);
  ptr_t size();
private:
  std::vector<std::tuple<const std::string, const ptr_t, const ast::Type *> > stack_;
  ptr_t size_;
};

template<typename ptr_t>
ptr_t Env<ptr_t>::push(const std::string &name, const ast::Type *type, const ptr_t size) {
  stack_.push_back(std::make_tuple(name, size_, type));
  size_ += size;
  return std::get<1>(stack_.back());
}

template<typename ptr_t>
void Env<ptr_t>::pop() {
  stack_.pop_back();
}

template<typename ptr_t>
std::pair<const ptr_t, const ast::Type *> Env<ptr_t>::find(const std::string &name) {
  for (auto i = stack_.rbegin(); i != stack_.rend(); ++i) {
    if (std::get<0>(*i) == name)
      return std::make_pair(std::get<1>(*i), std::get<2>(*i));
  }
  throw NotFound();
}

template<typename ptr_t>
ptr_t Env<ptr_t>::size() {
  return size_;
}
