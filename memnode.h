
#pragma once
#include <stdint.h>
#include <stddef.h>
#include "bitarray.h"

typedef struct memnode {
  bitarray *reservemask;
  void *sequence;
  size_t size;
  struct memnode *nextnode;
} memnode;

#define MAKE_LOCAL_MEMNODE(varname, size, nextnode)\
MAKE_LOCAL_BITARRAY(__ ## varname ## _reservemask, size);\
uint8_t __ ## varname ## _sequence[size];\
memnode __ ## varname = { __ ## varname ## _reservemask, __ ## varname ## _sequence, size, nextnode };\
memnode *varname = &__ ## varname;

extern void memnode_init (void*, size_t, bitarray*, memnode*, memnode*);
extern memnode *make_memnode (size_t, memnode*);
extern void free_memnode (memnode*);
extern void *memnode_allocate (size_t, memnode*);
extern memnode *memnode_next (memnode*);
extern bool memnode_addressp (void*, memnode*);
extern bool memnode_allocatep (void*, size_t, memnode*);
extern memnode *memnode_find (void*, memnode*);
extern int memnode_free (void*, size_t, memnode*);
extern bool memnode_emptyp (memnode*);
