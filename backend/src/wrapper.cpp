#include <iostream>

extern void *libmain(void **stack);

struct list_nat {
  uint32_t idx; //fuck
  uint32_t *x;
  list_nat *next;
};

int main() {
  void **stack = new void*[1024]; // allow at most 1024 variables in
                                  // the stack
  unsigned n;
  std::cin >> n;
  list_nat **cur = &stack[0];
  for (unsigned i = 0; i < n; ++i) {
    unsigned x;
    std::cin >> x;
    *cur = new list_nat{1, new unsigned(x), NULL};
    cur = &(*cur)->next;
  }
  *cur = new list_nat{0, NULL, NULL};
  
  list_nat *l = libmain(stack);
  wihle (l->idx != 0) {
    std::cout << *l->x << ' ';
    l = l->next;
  }
  std::cout << std::endl;
  return 0;
}
