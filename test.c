
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "memnode.h"

#define TEST(form)\
if (form){\
printf("success: %s at %s:%d\n", #form, __FILE__, __LINE__);\
}\
else {\
printf("error: %s at %s:%d\n", #form, __FILE__, __LINE__); abort();\
}

static void test_memnode (){
  // allocate, free and allocate.
  {
    MAKE_LOCAL_MEMNODE(mnode, 1024, NULL);
    TEST(memnode_emptyp(mnode) == true);
    char *value1 = memnode_allocate(3, mnode);
    TEST(value1 != NULL);
    TEST(memnode_find(value1, mnode) == mnode); //test linked memnode.
    TEST(memnode_addressp(value1, mnode) == true); //test linked memnode.
    TEST(memnode_allocatep(value1, 3, mnode) == true); //test linked memnode.
    TEST(memnode_emptyp(mnode) == false);
    TEST(value1[0] == 0);
    TEST(value1[1] == 0);
    TEST(value1[2] == 0);
    value1[0] = 'a';
    value1[1] = 'b';
    value1[2] = 'c';
    TEST(memnode_free(value1, 3, mnode) == 0);
    TEST(memnode_emptyp(mnode) == true);
    char *value2 = memnode_allocate(3, mnode);
    TEST(value2 != NULL);
    TEST(memnode_find(value2, mnode) == mnode); //test linked memnode.
    TEST(memnode_addressp(value2, mnode) == true); //test linked memnode.
    TEST(memnode_allocatep(value2, 3, mnode) == true); //test linked memnode.
    TEST(memnode_emptyp(mnode) == false);
    TEST(value2 == value1);
    TEST(value2[0] == 0);
    TEST(value2[1] == 0);
    TEST(value2[2] == 0);
    value2[0] = 'a';
    value2[1] = 'b';
    value2[2] = 'c';
  }
  // allocate many times.
  {
    MAKE_LOCAL_MEMNODE(mnode, 1024, NULL);
    TEST(memnode_emptyp(mnode) == true);
    char *value1 = memnode_allocate(3, mnode);
    char *value2 = memnode_allocate(3, mnode);
    char *value3 = memnode_allocate(3, mnode);
    TEST(value1 != NULL);
    TEST(value2 != NULL);
    TEST(value3 != NULL);
    TEST(memnode_find(value1, mnode) == mnode); //test linked memnode.
    TEST(memnode_find(value2, mnode) == mnode); //test linked memnode.
    TEST(memnode_find(value3, mnode) == mnode); //test linked memnode.
    TEST(memnode_addressp(value1, mnode) == true); //test linked memnode.
    TEST(memnode_addressp(value2, mnode) == true); //test linked memnode.
    TEST(memnode_addressp(value3, mnode) == true); //test linked memnode.
    TEST(memnode_allocatep(value1, 3, mnode) == true); //test linked memnode.
    TEST(memnode_allocatep(value2, 3, mnode) == true); //test linked memnode.
    TEST(memnode_allocatep(value3, 3, mnode) == true); //test linked memnode.
    TEST(memnode_emptyp(mnode) == false);
    value1[0] = 'a';
    value1[1] = 'b';
    value1[2] = 'c';
    value2[0] = '1';
    value2[1] = '2';
    value2[2] = '3';
    value3[0] = 'A';
    value3[1] = 'B';
    value3[2] = 'C';
    TEST(value1[0] == 'a');
    TEST(value1[1] == 'b');
    TEST(value1[2] == 'c');
    TEST(value2[0] == '1');
    TEST(value2[1] == '2');
    TEST(value2[2] == '3');
    TEST(value3[0] == 'A');
    TEST(value3[1] == 'B');
    TEST(value3[2] == 'C');
    TEST(memnode_free(value1, 3, mnode) == 0);
    TEST(memnode_free(value2, 3, mnode) == 0);
    TEST(memnode_free(value3, 3, mnode) == 0);
  }
  // allocate from linked memnodes.
  {
    MAKE_LOCAL_MEMNODE(mnode, 1024, NULL);
    MAKE_LOCAL_MEMNODE(mnoderoot, 0, mnode);
    TEST(memnode_emptyp(mnode) == true);
    char *value = memnode_allocate(3, mnoderoot);
    TEST(value != NULL);
    TEST(memnode_find(value, mnoderoot) == mnode);
    TEST(memnode_addressp(value, mnoderoot) == false);
    TEST(memnode_addressp(value, mnode) == true);
    TEST(memnode_allocatep(value, 3, mnoderoot) == false);
    TEST(memnode_allocatep(value, 3, mnode) == true);
    TEST(memnode_emptyp(mnode) == false);
    TEST(value[0] == 0);
    TEST(value[1] == 0);
    TEST(value[2] == 0);
    value[0] = 'a';
    value[1] = 'b';
    value[2] = 'c';
  }
  // allocte from small memnode.
  {
    MAKE_LOCAL_MEMNODE(mnode, sizeof(int) * 3, NULL);
    int *value1 = memnode_allocate(sizeof(int), mnode);
    int *value2 = memnode_allocate(sizeof(int), mnode);
    int *value3 = memnode_allocate(sizeof(int), mnode);
    int *value4 = memnode_allocate(sizeof(int), mnode);
    TEST(value1 != NULL);
    TEST(value2 != NULL);
    TEST(value3 != NULL);
    TEST(value4 == NULL);
    *value1 = 1;
    *value2 = 2;
    *value3 = 3;
    TEST(*value1 == 1);
    TEST(*value2 == 2);
    TEST(*value3 == 3);
    TEST(memnode_free(value1, sizeof(int), mnode) == 0);
    TEST(memnode_free(value2, sizeof(int), mnode) == 0);
    TEST(memnode_free(value3, sizeof(int), mnode) == 0);
    int *value1a = memnode_allocate(sizeof(int), mnode);
    int *value2a = memnode_allocate(sizeof(int), mnode);
    int *value3a = memnode_allocate(sizeof(int), mnode);
    int *value4a = memnode_allocate(sizeof(int), mnode);
    TEST(value1a != NULL);
    TEST(value2a != NULL);
    TEST(value3a != NULL);
    TEST(value4a == NULL);
    TEST(*value1a == *value1);
    TEST(*value2a == *value2);
    TEST(*value3a == *value3);
  }
}

static void test_make_memnode (){
  {
    memnode *mnode = make_memnode(1024, NULL);
    TEST(mnode != NULL); 
    TEST(memnode_emptyp(mnode) == true);
    char *value1 = memnode_allocate(3, mnode);
    TEST(value1 != NULL);
    TEST(memnode_find(value1, mnode) == mnode); //test linked memnode.
    TEST(memnode_addressp(value1, mnode) == true); //test linked memnode.
    TEST(memnode_allocatep(value1, 3, mnode) == true); //test linked memnode.
    TEST(memnode_emptyp(mnode) == false);
    TEST(value1[0] == 0);
    TEST(value1[1] == 0);
    TEST(value1[2] == 0);
    value1[0] = 'a';
    value1[1] = 'b';
    value1[2] = 'c';
    TEST(memnode_free(value1, 3, mnode) == 0);
    TEST(memnode_emptyp(mnode) == true);
    char *value2 = memnode_allocate(3, mnode);
    TEST(value2 != NULL);
    TEST(memnode_find(value2, mnode) == mnode); //test linked memnode.
    TEST(memnode_addressp(value2, mnode) == true); //test linked memnode.
    TEST(memnode_allocatep(value2, 3, mnode) == true); //test linked memnode.
    TEST(memnode_emptyp(mnode) == false);
    TEST(value2 == value1);
    TEST(value2[0] == 0);
    TEST(value2[1] == 0);
    TEST(value2[2] == 0);
    value2[0] = 'a';
    value2[1] = 'b';
    value2[2] = 'c';
    free_memnode(mnode);
  }
}

static void test_make_memnode2 (){
  // allocate 40 bytes.
  {
    memnode *mnode = make_memnode(1024, NULL);
    TEST(mnode != NULL);
    void *value1 = memnode_allocate(40, mnode);
    TEST(value1 != NULL);
    void *value2 = memnode_allocate(40, mnode);
    TEST(value2 - value1 == 40);
    TEST(value2 != NULL);
    void *value3 = memnode_allocate(40, mnode);
    TEST(value3 != NULL);
    TEST(value3 - value2 == 40);
    TEST(memnode_free(value1, 40, mnode) == 0);
    TEST(memnode_free(value2, 40, mnode) == 0);
    TEST(memnode_free(value3, 40, mnode) == 0);
    free_memnode(mnode);
  }
  // allocate 40 bytes many times.
  {
    memnode *mnode = make_memnode(1024, NULL);
    void *value1 = memnode_allocate(40, mnode);
    void *value2 = memnode_allocate(40, mnode);
    void *value3 = memnode_allocate(40, mnode);
    void *value4 = memnode_allocate(40, mnode);
    void *value5 = memnode_allocate(40, mnode);
    TEST(value1 != NULL);
    TEST(value2 != NULL);
    TEST(value3 != NULL);
    TEST(value4 != NULL);
    TEST(value5 != NULL);
    TEST(value1 != value2);
    TEST(value2 != value3);
    TEST(value3 != value4);
    TEST(value4 != value5);
    TEST(value2 - value1 == 40);
    TEST(value3 - value2 == 40);
    TEST(value4 - value3 == 40);
    TEST(value5 - value4 == 40);
    TEST(memnode_free(value1, 40, mnode) == 0);
    TEST(memnode_free(value2, 40, mnode) == 0);
    TEST(memnode_free(value3, 40, mnode) == 0);
    TEST(memnode_free(value4, 40, mnode) == 0);
    TEST(memnode_free(value5, 40, mnode) == 0);
  }
}

int main (){
  test_memnode();
  test_make_memnode();
  test_make_memnode2();
  return 0;
}
