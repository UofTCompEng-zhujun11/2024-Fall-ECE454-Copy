
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "assert.h"

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

// Global lock for synchronization
pthread_mutex_t global_lock;

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

// process stream
void* process_streams(void* threadID_ptr) {
  int thread_id = *((int*)threadID_ptr);
  int i, j, k;
  int rnum;
  unsigned key;
  int streams_per_thread, start_stream;
  sample *s;

  // Divide streams among threads
  streams_per_thread = NUM_SEED_STREAMS / num_threads;
  start_stream = thread_id * streams_per_thread;

  // Process assigned streams
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
      if (!(s = h.lookup_threaded(key))){
	      // insert a new element for it into the hash table
	      s = new sample(key);
	      h.insert_threaded(s);
      }

      // lockj
      pthread_mutex_lock(&global_lock);

      // increment the count for the sample
      s->count++;

      // Unlock the global lock
      pthread_mutex_unlock(&global_lock);
    }
  }
  pthread_exit(NULL);
}

int 
main(int argc, char* argv[]) {
  pthread_t threads[4];
  int thread_id[4] = {0, 1, 2, 3};
  int t_ret;

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

  //global mutext init
  pthread_mutex_init(&global_lock, NULL);

  // Create threads
  for (int i = 0; i < num_threads; i++) {
    t_ret = (pthread_create(&threads[i], NULL, process_streams, (void*) &thread_id[i]));
    assert(!t_ret);
  }

  // Wait for threads to complete
  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }

  // Destroy the global lock
  pthread_mutex_destroy(&global_lock);

  // print a list of the frequency of all samples
  h.print();

  return 0;
}
