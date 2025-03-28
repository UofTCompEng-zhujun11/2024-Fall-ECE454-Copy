
#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include "list.h"
#include <pthread.h>

#define HASH_INDEX(_addr,_size_mask) (((_addr) >> 2) & (_size_mask))

template<class Ele, class Keytype> class hash;

template<class Ele, class Keytype> class hash {
 private:
  unsigned my_size_log;
  unsigned my_size;
  unsigned my_size_mask;
  list<Ele,Keytype> *entries;
  pthread_mutex_t* list_locks; // Array of locks for each list

 public:
  void setup(unsigned the_size_log=5);
  void insert(Ele *e);
  Ele *lookup(Keytype the_key);
  void print(FILE *f=stdout);
  void reset();
  void cleanup();
  list<Ele,Keytype> *get_list(unsigned the_idx);
  // list-lock versions
  Ele* lookup_threaded(Keytype the_key);
  void insert_threaded(Ele* e);
};

template<class Ele, class Keytype> 
void 
hash<Ele,Keytype>::setup(unsigned the_size_log){
  my_size_log = the_size_log;
  my_size = 1 << my_size_log;
  my_size_mask = (1 << my_size_log) - 1;
  entries = new list<Ele,Keytype>[my_size];

  // Initialize locks for each lsit
  list_locks = new pthread_mutex_t[my_size];
  for (unsigned i = 0; i < my_size; i++) {
    pthread_mutex_init(&list_locks[i], NULL);
  }
}

template<class Ele, class Keytype> 
list<Ele,Keytype> *
hash<Ele,Keytype>::get_list(unsigned the_idx){
  if (the_idx >= my_size){
    fprintf(stderr,"hash<Ele,Keytype>::list() public idx out of range!\n");
    exit (1);
  }
  return &entries[the_idx];
}

template<class Ele, class Keytype> 
Ele *
hash<Ele,Keytype>::lookup(Keytype the_key){
  list<Ele,Keytype> *l;

  l = &entries[HASH_INDEX(the_key,my_size_mask)];
  return l->lookup(the_key);
}  

// lookup with list-level locking
template<class Ele, class Keytype> 
Ele *
hash<Ele,Keytype>::lookup_threaded(Keytype the_key) {
  int bucket_idx = HASH_INDEX(the_key, my_size_mask);

  pthread_mutex_lock(&list_locks[bucket_idx]);
  Ele* result = entries[bucket_idx].lookup(the_key);
  pthread_mutex_unlock(&list_locks[bucket_idx]);

  return result;
}

template<class Ele, class Keytype> 
void 
hash<Ele,Keytype>::print(FILE *f){
  unsigned i;

  for (i=0;i<my_size;i++){
    entries[i].print(f);
  }
}

template<class Ele, class Keytype> 
void 
hash<Ele,Keytype>::reset(){
  unsigned i;
  for (i=0;i<my_size;i++){
    entries[i].cleanup();
  }
}

template<class Ele, class Keytype> 
void 
hash<Ele,Keytype>::cleanup(){
  unsigned i;
  reset();

  // Destroy locks
  for (i = 0; i < my_size; i++) {
    pthread_mutex_destroy(&list_locks[i]);
  }

  delete [] entries;
  delete [] list_locks;
}

template<class Ele, class Keytype> 
void 
hash<Ele,Keytype>::insert(Ele *e){
  entries[HASH_INDEX(e->key(),my_size_mask)].push(e);
}

// insert with list-level locking
template<class Ele, class Keytype>
void 
hash<Ele, Keytype>::insert_threaded(Ele* e) {
  int bucket_idx = HASH_INDEX(e->key(), my_size_mask);

  pthread_mutex_lock(&list_locks[bucket_idx]);
  entries[bucket_idx].push(e);
  pthread_mutex_unlock(&list_locks[bucket_idx]);

  return;
}

#endif
