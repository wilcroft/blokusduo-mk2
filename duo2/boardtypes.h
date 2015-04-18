#ifndef __BOARDTYPES_H__
#define __BOARDTYPES_H__

#include <stdlib.h>
#include <stdint.h>
#include "bool.h"

#define NUMPLAYERS 2
#define BOARDHEIGHT 14
#define BOARDWIDTH 14
#define ISDUO 1

typedef struct {
	bool isOccupied;
	uint8_t owningPlayer;
	uint8_t x;
	uint8_t y;
	bool isLiberty[NUMPLAYERS];
	bool isAvailable[NUMPLAYERS];
} tile_t;

typedef struct{
	tile_t tile[BOARDHEIGHT][BOARDWIDTH];
	short score[NUMPLAYERS];
} board_t;

typedef struct tilelist{
	tile_t * t;
	struct tilelist * next;
} tilelist_t;

typedef struct {
	tilelist_t * l[NUMPLAYERS];
} tilelistgroup_t;
#endif