#ifndef __GAMEUTILS_H__
#define __GAMEUTILS_H__

#include "pieceutils.h"
#include "boardutils.h"
#include <time.h>

#define MAX_RAND_ATEMPTS 10000

/* isMoveValid - check if a given move is a valid play on the board
 *
 */
bool isMoveValid(board_t * b, move_t * m, int x, int y, int player);

/* placePiece - update the board with the desired piece
 *
 */
bool placePiece(board_t * b, piecelist_t * plist, short pnum,short rotations, bool reflected, short x, short y, uint8_t player, tilelistgroup_t * tlist);
bool placePieceFromCode(char * code, board_t * b, piecelist_t * plist, uint8_t player, tilelistgroup_t * tlist);
void getDumbMoveCode(board_t * b, piecelist_t * plist, short player,tilelistgroup_t * tlist, char * code);
void getRandomMoveCode(board_t * b, piecelist_t * plist, short player,tilelistgroup_t * tlist, char * code);
void getBetterRandomMoveCode(board_t * b, piecelist_t * plist, short player,tilelistgroup_t * tlist, char * code);
void getLookAheadMoveCode(board_t * b, piecelist_t * plist, short player,tilelistgroup_t * tlist, char * code);

#endif