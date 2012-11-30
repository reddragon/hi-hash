#ifndef _HI_HASH_
#define _HI_HASH_

#include <vector>
#include <set>
#include <cassert>
#include <iostream>

enum HIHashTableError {
  HASH_TABLE_FULL   = -1,
  HASH_TABLE_EMPTY  = -2,
  ENTRY_NOT_FOUND   = -3
};

template <typename T>
struct HashEntry {
  T *value;
  bool exists;
  size_t rank;
};

template <typename T>
class HIHashTable {
  public:
  std::vector< HashEntry<T> > table;
  uint32_t (*hash)(T *);
  bool (*equals)(T *, T*);
  bool (*greater)(T *, T *);
  size_t numElems, tableSize;
  
  size_t probe(T *, size_t);

  HIHashTable(uint32_t (*)(T*), bool (*)(T*, T*), bool (*)(T *, T *), size_t);
  ~HIHashTable();
  int add(T *);
  int remove(T *);
  int exists(T *);
  std::pair<size_t, size_t> size();
  size_t rank(T *t, size_t loc);
  int next(size_t loc);
};

template <typename T>
HIHashTable<T>::HIHashTable(uint32_t h(T *), bool eq(T *, T*), bool gr(T *, T *), size_t n) : tableSize(n) {
  hash = h; 
  equals = eq;
  greater = gr;
  table.resize(tableSize);
}

template <typename T>
size_t HIHashTable<T>::probe(T *t, size_t i) {
  return (hash(t) + i) % tableSize;
}

template <typename T>
int HIHashTable<T>::add(T * t) {
  // We cannot add any more elements, since we
  // are running out of space.
  if (numElems == tableSize) {
    return HASH_TABLE_FULL;
  }
  
  size_t loc = probe(t, 0), i = 0;
  T *tp = t;
  while (table[loc].exists != false) {
    if (equals(table[loc].value, tp)) {
      return 0;
    } else if (greater(table[loc].value, tp)) {
      i++;
      loc = probe(tp, i);
    } else if (greater(tp, table[loc].value)) {
      std::swap(tp, table[loc].value);
      i = table[loc].rank;
      i++;
      loc = probe(tp, i);
    }
  }
  table[loc].value = tp;
  table[loc].exists = true;
  table[loc].rank = i;
  numElems++;
  return 0;
}

template <typename T>
int HIHashTable<T>::exists(T * t) {
  for (size_t i = 0; i < tableSize; i++) {
    size_t loc = probe(t, i);
    
    // There is no entry at this location, and
    // hence, we would not find the entry that
    // we are looking for, even if we keep probing.
    if (table[loc].exists == false) {
      return ENTRY_NOT_FOUND;
    }

    // If the entry at this location is of greater
    // rank, then we cannot find t, since at next
    // locations, we would only find elements which
    // are even smaller than t.
    if (greater(t, table[loc].value)) {
      return ENTRY_NOT_FOUND;
    }
    

    // Is the element at this location is the same
    // as t?
    if (equals(t, table[loc].value)) {
      return loc;
    }
  }

  // Did not find the element anywhere
  return ENTRY_NOT_FOUND;
}

int moduloAdd(int a, int b, int m) {
  int res = a + b;
  while(res < 0) {
    res += m;  
  }
  return res % m;
}

template <typename T>
size_t HIHashTable<T>::rank(T *t, size_t loc) {
  int res = moduloAdd(loc, -hash(t), tableSize);
  assert(res >= 0 && res < (int)tableSize);
  return (size_t) res;
}

template <typename T>
int HIHashTable<T>::next(size_t loc) {
  int oloc = (loc + 1) % tableSize;
  while (table[oloc].exists) {
    int current_rank = rank(table[oloc].value, (int)oloc);
    int prospective_rank = rank(table[oloc].value, (int)loc);
    if (prospective_rank < current_rank) {
      return oloc;
    }
    oloc = (oloc + 1) % tableSize;
  }
  return ENTRY_NOT_FOUND;
}

template <typename T>
int HIHashTable<T>::remove(T *t) {
  int loc = exists(t);
  if (loc == ENTRY_NOT_FOUND) {
    return ENTRY_NOT_FOUND;
  }
  
  int n, x;
  while ((x = next(loc)) != ENTRY_NOT_FOUND) {
    n = x;
    table[loc] = table[n];
    loc = n;
  }
  table[loc].exists = false;
  return HASH_TABLE_EMPTY;
}

template <typename T>
HIHashTable<T>::~HIHashTable() {
  // TODO Fill up
}

#endif
