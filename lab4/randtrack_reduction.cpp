
#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "defs.h"
#include "hash.h"

#define SAMPLES_TO_COLLECT   10000000
#define RAND_NUM_UPPER_BOUND   100000
#define NUM_SEED_STREAMS            4

/* 
 * ECE454 Students: 
 * Please fill in the following team struct 
 */
team_t team = {
    "Team Name",                  /* Team name */

    "AAA BBB",                    /* Member full name */
    "9999999999",                 /* Member student number */
    "AAABBB@CCC",                 /* Member email address */
};

unsigned num_threads;
unsigned samples_to_skip;

class sample;

class sample {
  unsigned my_key;
 public:
  sample *next;
  unsigned count;

  sample(unsigned the_key){my_key = the_key; count = 0;};
  unsigned key(){return my_key;}
  void print(FILE *f){printf("%d %d\n",my_key,count);}
};

// This instantiates an empty hash table
// it is a C++ template, which means we define the types for
// the element and key value here: element is "class sample" and
// key value is "unsigned".  
hash<sample,unsigned> h;

// Thread argumen
typedef struct {
  int thread_id;
  hash<sample, unsigned>* private_hash;
} thread_arg_t;

// process stream
void* process_streams(void* arg) {
  int thread_id = ((thread_arg_t*)arg)->thread_id;
  hash<sample, unsigned>* p_h = ((thread_arg_t*)arg)->private_hash;
  int i, j, k, rnum;
  unsigned key;
  int streams_per_thread, start_stream;
  sample *s;

  streams_per_thread = NUM_SEED_STREAMS / num_threads;
  start_stream = thread_id * streams_per_thread;

  // process streams starting with different initial numbers
  for (i = start_stream; i < start_stream + streams_per_thread; i++) {
    rnum = i;

    // collect a number of samples
    for (j=0; j<SAMPLES_TO_COLLECT; j++){

      // skip a number of samples
      for (k=0; k<samples_to_skip; k++){
	      rnum = rand_r((unsigned int*)&rnum);
      }

      // force the sample to be within the range of 0..RAND_NUM_UPPER_BOUND-1
      key = rnum % RAND_NUM_UPPER_BOUND;

      // if this sample has not been counted before
      if (!(s = p_h->lookup(key))){
	      // insert a new element for it into the hash table
	      s = new sample(key);
	      p_h->insert(s);
      }

      // increment the count for the sample
      s->count++;
    }
  }
  pthread_exit(NULL);
}

// Function to merge hash
void merge_private_to_global(hash<sample, unsigned>* private_hash) {
  for (int  i = 0; i < 16384; i++) {
    list<sample, unsigned>* bucket = private_hash->get_list(i);
    sample* current = bucket->head();

    while (current != NULL) {
      sample* global_sample = h.lookup(current->key());
      if (global_sample == NULL) {
        global_sample = new sample(current->key());
        h.insert(global_sample);
      }
      global_sample->count += current->count;

      current = current->next;
    }
  }
}

int  
main (int argc, char* argv[]){
  int t_ret;
  pthread_t threads[4];
  thread_arg_t thread_args[4];
  hash<sample, unsigned> private_hashes[4];

  // Print out team information
  printf( "Team Name: %s\n", team.team );
  printf( "\n" );
  printf( "Student 1 Name: %s\n", team.name1 );
  printf( "Student 1 Student Number: %s\n", team.number1 );
  printf( "Student 1 Email: %s\n", team.email1 );
  printf( "\n" );

  // Parse program arguments
  if (argc != 3){
    printf("Usage: %s <num_threads> <samples_to_skip>\n", argv[0]);
    exit(1);  
  }
  sscanf(argv[1], " %d", &num_threads); // not used in this single-threaded version
  sscanf(argv[2], " %d", &samples_to_skip);

  // initialize a 16K-entry (2**14) hash of empty lists
  h.setup(14);

  for (int i = 0; i < num_threads; i++) {
    private_hashes[i].setup(14);
    thread_args[i].thread_id = i;
    thread_args[i].private_hash = &private_hashes[i];
    t_ret = pthread_create(&threads[i], NULL, process_streams, &thread_args[i]);

    assert(!t_ret);
  }

  // Wait for threads to complete
  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }

  // Merge private hash tables into the global hash table
  for (int i = 0; i < num_threads; i++) {
    merge_private_to_global(&private_hashes[i]);
  }

  // print a list of the frequency of all samples
  h.print();
}
