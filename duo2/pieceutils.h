#ifndef __PIECEUTILS_H__
#define __PIECEUTILS_H__

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include "piecetypes.h"

piecelist_t * initializePiecelist (piecelist_t * list);
piecelist_t * copyPieceList (piecelist_t * cpy, piecelist_t * src);
piecelist_t * copyPieceListState (piecelist_t * cpy, piecelist_t * src);
void resetPieceList(piecelist_t * list);
move_t getMoveFromPiece(piece_t * p, short rotations, bool isReflected);
bool getMoveFromCode(char * code, short * x, short * y, short * pnum, short * rots, bool* refl);
bool getCode(char * code, short x, short y, short p, short rots, short refl);
short ctoh(char c);


#endif