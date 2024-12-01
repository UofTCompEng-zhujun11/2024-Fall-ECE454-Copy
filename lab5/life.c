/*****************************************************************************
 * life.c
 * Parallelized and optimized implementation of the game of life resides here
 ****************************************************************************/
#include "life.h"
#include "util.h"

/*****************************************************************************
 * Helper function definitions
 ****************************************************************************/
/**
 * Swapping the two boards only involves swapping pointers, not
 * copying values.
 */
#define SWAP_BOARDS( b1, b2 )  do { \
  char* temp = b1; \
  b1 = b2; \
  b2 = temp; \
} while(0)

#define BOARD( __board, __i, __j )  (__board[(__i) + LDA*(__j)])
/*****************************************************************************
 * Game of life implementation
 ****************************************************************************/
char*
game_of_life (char* outboard, 
	      char* inboard,
	      const int nrows,
	      const int ncols,
	      const int gens_max)
{
    const int LDA = nrows;  // Leading dimension of the array
    int curgen;

    for (curgen = 0; curgen < gens_max; curgen++) {
        for (int i = 0; i < nrows; i++) {
            for (int j = 0; j < ncols; j += 16) { // Process 16 cells at a time
                // Compute neighbors for each group of 16 cells
                for (int k = 0; k < 16; k++) {
                    const int col = j + k; // Column offset for current group
                    const int inorth = mod(i - 1, nrows);
                    const int isouth = mod(i + 1, nrows);
                    const int jwest = mod(col - 1, ncols);
                    const int jeast = mod(col + 1, ncols);

                    const char neighbor_count =
                        BOARD(inboard, inorth, jwest) +
                        BOARD(inboard, inorth, col) +
                        BOARD(inboard, inorth, jeast) +
                        BOARD(inboard, i, jwest) +
                        BOARD(inboard, i, jeast) +
                        BOARD(inboard, isouth, jwest) +
                        BOARD(inboard, isouth, col) +
                        BOARD(inboard, isouth, jeast);

                    // Update the output board
                    BOARD(outboard, i, col) = alivep(neighbor_count, BOARD(inboard, i, col));
                }
            }
        }
        SWAP_BOARDS(outboard, inboard);
    }
    return inboard;
}
