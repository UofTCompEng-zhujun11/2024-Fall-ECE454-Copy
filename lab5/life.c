/*****************************************************************************
 * life.c
 * Parallelized and optimized implementation of the game of life resides here
 ****************************************************************************/
#include "life.h"
#include "util.h"
#include <omp.h>

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
    const int LDA = nrows; // Leading dimension of the array
    const int TILE_SIZE = 32; // Tile size (minimum board size)

    int curgen;

    for (curgen = 0; curgen < gens_max; curgen++) {
        #pragma omp parallel for collapse(2) schedule(static)
        for (int ti = 0; ti < nrows; ti += TILE_SIZE) {
            for (int tj = 0; tj < ncols; tj += TILE_SIZE) {
                // Process each tile
                for (int i = ti; i < ti + TILE_SIZE && i < nrows; i++) {
                    for (int j = tj; j < tj + TILE_SIZE && j < ncols; j++) {
                        // Calculate neighbors with toroidal wrapping
                        const int inorth = mod(i - 1, nrows);
                        const int isouth = mod(i + 1, nrows);
                        const int jwest = mod(j - 1, ncols);
                        const int jeast = mod(j + 1, ncols);

                        const char neighbor_count =
                            BOARD(inboard, inorth, jwest) +
                            BOARD(inboard, inorth, j) +
                            BOARD(inboard, inorth, jeast) +
                            BOARD(inboard, i, jwest) +
                            BOARD(inboard, i, jeast) +
                            BOARD(inboard, isouth, jwest) +
                            BOARD(inboard, isouth, j) +
                            BOARD(inboard, isouth, jeast);

                        // Update the state in the output board
                        BOARD(outboard, i, j) = alivep(neighbor_count, BOARD(inboard, i, j));
                    }
                }
            }
        }
        SWAP_BOARDS(outboard, inboard);
    }
    return inboard;
}
