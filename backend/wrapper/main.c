#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

extern void *umain(void *arg);

struct list_nat;
  
struct list_nat_y {
  uint32_t *x;
  struct list_nat *next;
};
  
struct list_nat {
  uint32_t idx; //fuck
  void *y;
};

int main() {

  void *arg;
  unsigned n;
  scanf("%u", &n);
  struct list_nat **cur = (struct list_nat **)&arg;
  for (unsigned i = 0; i < n; ++i) {
    unsigned x;
    scanf("%u", &x);
    
    struct list_nat_y *y = (struct list_nat_y *)malloc(sizeof(struct list_nat_y));
    y->x = (unsigned *)malloc(sizeof(unsigned));
    *y->x = x;
    y->next = NULL;
    
    *cur = (struct list_nat *)malloc(sizeof(struct list_nat));
    (*cur)->idx = 1;
    (*cur)->y = y;
    cur = &y->next;
  }
  *cur = (struct list_nat *)malloc(sizeof(struct list_nat));
  (*cur)->idx = 0;
  (*cur)->y = NULL;
  
  struct list_nat *l = (struct list_nat *)umain(arg);
  while (l->idx != 0) {
    struct list_nat_y *y = (struct list_nat_y *)l->y;
    printf("%p %u\n", y->x, *(y->x));
    l = y->next;
  }
  printf("\n");
  return 0;
}
