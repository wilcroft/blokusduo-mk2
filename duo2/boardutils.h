#ifndef __DUO_BOARDUTILS_H__
#define __DUO_BOARDUTILS_H__

#include "boardtypes.h"
#include "stdio.h"


/* initializeBoard - create a blank board ready for a new game.
 * Sets the values of all tiles of board to the start state.
 * If b == NULL, initalizeBoard will malloc a new board for its
 * use. Returns a ptr to b, or the malloc'd board if b == NULL.
 */
board_t * initializeBoard(board_t * b);

board_t * copyBoard(board_t * b, const board_t * src);
/* updateLiberties - updates board marking of tiles that are 
 * liberties/available for play.
 *
 */
void updateLiberties(board_t * b, tilelistgroup_t * libList);//tilelist_t * libList [NUMPLAYERS]);

void emptyList(tilelist_t ** t);
tilelist_t * addListItemFront(tilelist_t * list, tilelist_t * item);
tilelist_t * addListTileFront(tilelist_t * list, tile_t * tile);
tilelist_t * addListItemBack(tilelist_t * list, tilelist_t * item);
tilelist_t * addListTileBack(tilelist_t * list, tile_t * tile);
void drawBoard(board_t * b);
void drawMarkedBoard(board_t * b,short player);

void printLibertyList(tilelist_t * list);

tilelistgroup_t * copyTileListGroup(tilelistgroup_t * cpy, tilelistgroup_t * src);

#endif;