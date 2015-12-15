#include <iostream>
#include <cstdint>

extern "C" {
  extern void *umain(void *arg);

  struct list_nat;
  
  struct list_nat_y {
    uint32_t *x;
    list_nat *next;
  };
  
  struct list_nat {
    uint32_t idx; //fuck
    void *y;
  };
};

int main() {

  void *arg;
  unsigned n;
  std::cin >> n;
  list_nat **cur = (list_nat **)&arg;
  for (unsigned i = 0; i < n; ++i) {
    unsigned x;
    std::cin >> x;
    list_nat_y *y = new list_nat_y{new unsigned{x}, NULL};
    *cur = new list_nat{1, y};
    cur = &y->next;
  }
  *cur = new list_nat{0, NULL};
  
  list_nat *l = (list_nat *)umain(arg);
  while (l->idx != 0) {
    list_nat_y *y = (list_nat_y *)l->y;
    std::cout << *(y->x) << ' ';
    l = y->next;
  }
  std::cout << std::endl;
  return 0;
}
