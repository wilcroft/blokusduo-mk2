#ifndef __PIECETYPES_H__
#define __PIECETYPES_H__

#include <stdint.h>
#include <stdlib.h>
#include "bool.h"

#define MOVELISTLENGTH 100

#define NUMPIECES 21

typedef struct {
	short rotations;
	bool hasReflection;
	bool isPlayed;
	short map [5][5];
} piece_t;

typedef struct {
	piece_t p[NUMPIECES];
} piecelist_t;

typedef struct {
	short map [5][5];
} move_t;

typedef struct {
	char code [MOVELISTLENGTH][5];
	short count;
} movecodelist_t;
#endif