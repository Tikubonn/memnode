
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "bitarray.h"
#include "memnode.h"

void memnode_init (void *sequence, size_t size, bitarray *reservemask, memnode *nextnode, memnode *mnode){
  mnode->sequence = sequence;
  mnode->size = size;
  mnode->reservemask = reservemask;
  mnode->nextnode = nextnode;
}

memnode *make_memnode (size_t size, memnode *nextnode){
  memnode *mnode = malloc(sizeof(memnode));
  if (mnode == NULL){
    return NULL;
  }
  uint8_t *mnodeseq = malloc(size);
  if (mnodeseq == NULL){
    free(mnode);
    return NULL;
  }
  bitarray *reservemask = make_bitarray(size);
  if (reservemask == NULL){
    free(mnode);
    free(reservemask);
    return NULL;
  }
  memnode_init(mnodeseq, size, reservemask, nextnode, mnode);
  return mnode;
}

void free_memnode (memnode *mnode){
  memnode *nd = mnode;
  while (nd != NULL){
    memnode *nextnd = nd->nextnode;
    free_bitarray(nd->reservemask);
    free(nd->sequence);
    free(nd);
    nd = nextnd;
  }
}

static void clear (uint8_t *sequence, size_t size){
  for (size_t index = 0; index < size; index++){
    sequence[index] = 0;
  }
}

static void *memnode_allocate_at (size_t size, memnode *mnode){
  if (0 < size){
    size_t foundoffset;
    if (bitarray_find(0, size, mnode->reservemask, &foundoffset) != 0){
      return NULL;
    }
    if (bitarray_fill(1, foundoffset, foundoffset + size, mnode->reservemask) != 0){
      return NULL;
    }
    clear(mnode->sequence + foundoffset, size);
    return mnode->sequence + foundoffset;
  }
  else {
    return NULL;
  }
}

void *memnode_allocate (size_t size, memnode *mnode){
  for (memnode *nd = mnode; nd != NULL; nd = nd->nextnode){
    void *allocated = memnode_allocate_at(size, nd);
    if (allocated != NULL){
      return allocated;
    }
  }
  return NULL;
}

memnode *memnode_next (memnode *mnode){
  return mnode->nextnode;
}

bool memnode_addressp (void *address, memnode *mnode){
  return mnode->sequence <= address && address < mnode->sequence + mnode->size;
}

bool memnode_allocatep (void *address, size_t size, memnode *mnode){
  memnode *foundmnode = memnode_find(address, mnode);
  if (foundmnode != NULL){
    bool fillp;
    size_t offset = address - foundmnode->sequence;
    if (bitarray_fillp(1, offset, offset + size, mnode->reservemask, &fillp) != 0){ return false; }
    return fillp;
  }
  else {
    return false;
  }
}

memnode *memnode_find (void *address, memnode *mnode){
  for (memnode *nd = mnode; nd != NULL; nd = nd->nextnode){
    if (memnode_addressp(address, nd)){
      return nd;
    }
  }
  return NULL;
}

int memnode_free (void *address, size_t size, memnode *mnode){
  memnode *nd = memnode_find(address, mnode);
  if (nd == NULL){
    return 1; 
  }
  size_t offset = address - mnode->sequence;
  bool reservep;
  if (bitarray_fillp(1, offset, offset + size, mnode->reservemask, &reservep) != 0){
    return 1;
  }
  if (bitarray_fill(0, offset, offset + size, mnode->reservemask) != 0){
    return 1;
  }
  return 0;
}

bool memnode_emptyp (memnode *mnode){
  bool freep;
  if (bitarray_fillp(0, 0, bitarray_length(mnode->reservemask), mnode->reservemask, &freep) != 0){
    return false; 
  }
  return freep;
}
