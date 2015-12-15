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
  Env(const ptr_t &base);
  ptr_t push(const std::string &name, const ast::Type *const type, const ptr_t &size);
  std::tuple<const std::string, const ptr_t, const ast::Type *> pop();
  std::pair<const ptr_t , const ast::Type *> find(const std::string &name);
  ptr_t size();
private:
  std::vector<std::tuple<const std::string, const ptr_t, const ast::Type *> > stack_;
  ptr_t size_;
};

template<typename ptr_t>
Env<ptr_t>::Env(const ptr_t &base)
  :size_(base) {}
   
template<typename ptr_t>
ptr_t Env<ptr_t>::push(const std::string &name, const ast::Type *type, const ptr_t &size) {
  stack_.push_back(std::make_tuple(name, size, type));
  size_ += size;
  return std::get<1>(stack_.back());
}

template<typename ptr_t>
std::tuple<const std::string, const ptr_t, const ast::Type *> Env<ptr_t>::pop() {
  auto ret = stack_.back();
  stack_.pop_back();
  return ret;
}

template<typename ptr_t>
std::pair<const ptr_t, const ast::Type *> Env<ptr_t>::find(const std::string &name) {
  ptr_t size = size_;
  for (auto i = stack_.rbegin(); i != stack_.rend(); ++i) {
    size -= std::get<1>(*i);
    if (std::get<0>(*i) == name)
      return std::make_pair(size, std::get<2>(*i));
  }
  throw NotFound();
}

template<typename ptr_t>
ptr_t Env<ptr_t>::size() {
  return size_;
}
