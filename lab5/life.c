/*****************************************************************************
 * life.c
 * Parallelized and optimized implementation of the game of life resides here
 ****************************************************************************/
#include "life.h"
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*****************************************************************************
 * Helper function definitions
 ****************************************************************************/
#define BOARD( __board, __i, __j )  (__board[(__j) + LDA*(__i)])
#define NUMTHREADS 4
/*****************************************************************************
 * Game of life implementation
 ****************************************************************************/

typedef struct row{
    struct row* prev;
    struct row* next;
    int* x_tail;
    int x_cords[10010];
} row;

typedef enum {
		DEAD, LIVE
} state;
state stateList[1 << 9]; //global state table

// Thread argument
typedef struct {
  int thread_id;
  row* rowIn;
  row* rowOut;
  int ncols;
  int nrows;
} thread_arg_t;

void* processNextRows(void* arg){
    int bitmap;
    int startRow, rowPerthread;
    int *prev, *this, *next, *x_tail_this, *x_tail_prev, *x_tail_next;
    int *out;
    int x;

    int nrows = ((thread_arg_t*)arg)->nrows;
    int ncols = ((thread_arg_t*)arg)->ncols;
    int threadId = ((thread_arg_t*)arg)->thread_id;
    row* rowIn = ((thread_arg_t*)arg)->rowIn;
    row* rowOut = ((thread_arg_t*)arg)->rowOut;
    rowPerthread = nrows / NUMTHREADS;
    startRow = threadId * rowPerthread;

    for (int row = startRow; row < startRow + rowPerthread; row++){
        // printf("row %d: \n", row);
        bitmap = 0;
        //set up x and x_poiters
        this = rowIn[row].x_cords;
        prev = rowIn[row].prev->x_cords;
        next = rowIn[row].next->x_cords;
        out = rowOut[row].x_cords;
        x = 50000;
        *out = 50000;
        rowOut[row].x_tail = out;
        x = *prev;
        //find the left most cell
		if(x > *this)
			x = *this;
		if(x > *next)
			x = *next;
        if (x == 50000) break; //scan line empty
        
        //fecth tail x cords for the current scan line (largest x in scan line)
        x_tail_this = rowIn[row].x_tail;
        x_tail_next = rowIn[row].next->x_tail;
        x_tail_prev = rowIn[row].prev->x_tail;

        //check for wrap and handle x = 0 properly by initializing bitmap with wrapped column
        //insert column at x = 0 into bitmap[3-5]
        if(*prev == 0) {
			bitmap |= 010;
			prev++;
		}
		if(*this == 0) {
			bitmap |= 020;
			this++;
		}
		if(*next == 0) {
			bitmap |= 040;
			next++;
		}
        //insert column at x = ncols - 1 into bitmap[0-2]
        if(*x_tail_prev == ncols - 1) {
			bitmap |= 01;
		}
		if(*x_tail_this == ncols - 1) {
			bitmap |= 02;
		}
		if(*x_tail_next == ncols - 1) {
			bitmap |= 04;
		}
        // printf("    head wrap bitmap @ x = %d is: %d\n", x, bitmap);
        if (bitmap != 0) x = 1;

        //Enter main loop to scan along x cords
        for(;;) {
			for(;;) {
				//insert column at x >= 1 into bitmap[6-8]
				if(*prev == x) {
					bitmap |= 0100;
					prev++;
				}
				if(*this == x) {
					bitmap |= 0200;
					this++;
				}
				if(*next == x) {
					bitmap |= 0400;
					next++;
				}
				/* what does this bitmap indicate? */
                // printf("    bitmap @ x = %d is: %d\n", x, bitmap);
				if(stateList[bitmap] == LIVE)
					*out++ = x - 1;
				else if(bitmap == 000)
					break;
				/* move right */
				bitmap >>= 3;
				x += 1;
                //just written the cell at ncol - 2
                if (x >= ncols) break;
			}
            x = *prev;
            //find the left most cell
		    if(x > *this)
		    	x = *this;
		    if(x > *next)
		    	x = *next;
            if (x == 50000){
                //handle wrapping at x = ncol -1
                // printf("    tail wrap entry bitmap @ x = %d is: %d\n", x, bitmap);
                //insert column at x = 0 into bitmap[6-8]
                if(*(rowIn[row].prev->x_cords) == 0) {
		        	bitmap |= 0100;
		        }
		        if(*(rowIn[row].x_cords) == 0) {
		        	bitmap |= 0200;
		        }
		        if(*(rowIn[row].next->x_cords) == 0) {
		        	bitmap |= 0400;
		        }
                // printf("    tail wrap bitmap @ x = %d is: %d\n", x, bitmap);
                if(stateList[bitmap] == LIVE){
                    *out++ = ncols - 1;
                }
                break; //entire scan line is empty
            }
		}
        rowOut[row].x_tail = --out;
        out++;
        //terminate xcord array
        *out++ = 50000;
        *out++ = 50000;
        *out++ = 50000;
        *out++ = 50000;
        *out++ = 50000;
        for (int j = 0; rowOut[row].x_cords[j] != 50000; j++){
            // printf("%d ", rowOut[row].x_cords[j]);
        }
        // printf("\n");
    }
    pthread_exit(NULL);
}

char*
game_of_life (char* outboard, 
	      char* inboard,
	      const int nrows,
	      const int ncols,
	      const int gens_max)
{
    if (nrows > 10000) return inboard;
    const int LDA = nrows;
    row *rowIn = (row*) malloc(10000 * sizeof(*rowIn));
    row *rowOut = (row*) malloc(10000 * sizeof(*rowOut));
    row* temp;
    int x_cords_idx;
    int liveCnt, bmpIdx;
    pthread_t threads[4];
    thread_arg_t thread_args[4];

    //init bitmap arr
    for(int bitmap_state_idx = 0; bitmap_state_idx < 1<<9; bitmap_state_idx++) {
		for(liveCnt = bmpIdx = 0; bmpIdx < 9; bmpIdx++)
			if(bitmap_state_idx & 1<<bmpIdx)
				liveCnt += 1;
		if(bitmap_state_idx & 020) {
			if(liveCnt - 1 == 2 || liveCnt - 1 == 3) //remove the center live cell
				stateList[bitmap_state_idx] = LIVE;
			else
				stateList[bitmap_state_idx] = DEAD;
		} else {
			if(liveCnt == 3)
				stateList[bitmap_state_idx] = LIVE;
			else
				stateList[bitmap_state_idx] = DEAD;
		}
	}

    //init live cords into rowIn
    for (int i = 0; i < nrows; i++){
        x_cords_idx = 0;
        rowIn[i].prev = &rowIn[i - 1];
        rowIn[i].next = &rowIn[i + 1];
        rowOut[i].prev = &rowOut[i - 1];
        rowOut[i].next = &rowOut[i + 1];
        // printf("row %d:  ", i);
        for (int j = 0; j < ncols; j++){
            if (BOARD(inboard, i, j) == 1){
                rowIn[i].x_cords[x_cords_idx] = j;
                // printf("%d ", rowIn[i].x_cords[x_cords_idx]);
                x_cords_idx++;
            }
        }
        //if there are live cells in this row, set tail ptr to the last life cell, else point to [0], which is -1
        //buffer enought -1 to make sure garbege wont be picked up
        rowIn[i].x_cords[x_cords_idx] = 50000;
        // printf("%d ", rowIn[i].x_cords[x_cords_idx]);
        rowIn[i].x_cords[x_cords_idx + 1] = 50000;
        // printf("%d ", rowIn[i].x_cords[x_cords_idx + 1]);
        rowIn[i].x_cords[x_cords_idx + 2] = 50000;
        // printf("%d ", rowIn[i].x_cords[x_cords_idx + 2]);
        rowIn[i].x_cords[x_cords_idx + 3] = 50000;
        // printf("%d ", rowIn[i].x_cords[x_cords_idx + 3]);
        rowIn[i].x_cords[x_cords_idx + 4] = 50000;
        // printf("%d ", rowIn[i].x_cords[x_cords_idx + 4]);
        if (x_cords_idx != 0)
            rowIn[i].x_tail = &rowIn[i].x_cords[x_cords_idx - 1];
        else
            rowIn[i].x_tail = &rowIn[i].x_cords[x_cords_idx];
        // printf("x_tail = %d \n", *rowIn[i].x_tail);
    }
    // printf("***********init done********\n");
    //Wrap rows properly
    rowIn[0].prev = &rowIn[nrows - 1];
    rowIn[nrows - 1].next = &rowIn[0];
    rowOut[0].prev = &rowOut[nrows - 1];
    rowOut[nrows - 1].next = &rowOut[0];

    //Main iteration processing loop
    for (int it = 0; it < gens_max; it++){
        //for all rows
        for (int threadId = 0; threadId < NUMTHREADS; threadId++){
            thread_args[threadId].thread_id = threadId;
            thread_args[threadId].ncols = ncols;
            thread_args[threadId].nrows = nrows;
            thread_args[threadId].rowIn = rowIn;
            thread_args[threadId].rowOut = rowOut;
            pthread_create(&threads[threadId], NULL, processNextRows, &thread_args[threadId]);
        }
        // Wait for threads to complete
        for (int i = 0; i < NUMTHREADS; i++) {
          pthread_join(threads[i], NULL);
        }
        temp = rowIn;
        rowIn = rowOut;
        rowOut = temp;
    }

    //Construct output
    memset(outboard, 0, sizeof(char) * ncols * nrows * 2);
    for (int i = 0; i < nrows; i++){
        for (int j = 0; rowIn[i].x_cords[j] != 50000; j++){
            BOARD(outboard, i, rowIn[i].x_cords[j]) = 1;
        }
    }
    free(rowIn);
    free(rowOut);

    return outboard;
}
