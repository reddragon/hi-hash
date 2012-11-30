#include "hi_hash.h"
#include <iostream>
#include <cassert>

uint32_t hash(int *t) {
  return *t;
}

bool equals(int *a, int *b) {
  return (*a == *b);
}

bool greater(int *a, int *b) {
  return (*a > *b);
}

void printContents(HIHashTable<int> *h) {
  for (size_t i = 0; i < h->table.size(); i++) {
    HashEntry<int> e = h->table[i];
    if (e.exists) {
      std::cout << *((int *)e.value) << std::endl;
    } else {
      std::cout << "---" << std::endl;
    }
  }
}

int main() {
  HIHashTable<int> h(&hash, &equals, &greater, 10);
  std::vector<int> v(4);
  v[0] = 19; v[1] = 9; v[2] = 39; v[3] = 29;
  for (size_t i = 0; i < v.size(); i++) {
    h.add(&v[i]);
    std::cout << "Adding another element" << std::endl;
    printContents(&h);
    assert(h.exists(&v[i]) != ENTRY_NOT_FOUND);
  }

  for (size_t i = 0; i < v.size(); i++) {
    assert(h.exists(&v[i]) != ENTRY_NOT_FOUND);
  }
  
  for (size_t i = 0; i < v.size(); i++) {
    h.remove(&v[i]);
    std::cout << "Removing another element" << std::endl;
    printContents(&h);
    assert(h.exists(&v[i]) == ENTRY_NOT_FOUND);
  }
}
