#include "pieceutils.h"

piecelist_t * initializePiecelist (piecelist_t * list){
	FILE * f;
	int i, j;
	int err;
	if (fopen_s(&f,"piecemaps.txt", "r")!=0){
		printf("Error: %d\n", errno);
		getchar();
		return 0;
	}

	for (i=0; i<NUMPIECES; i++){
		list->p[i].isPlayed=false;
		for (j=0; j<5; j++){
			err= fscanf(f,"%hd%hd%hd%hd%hd", &(list->p[i].map[j][0]),
									&(list->p[i].map[j][1]),
									&(list->p[i].map[j][2]),
									&(list->p[i].map[j][3]),
									&(list->p[i].map[j][4]));
			if (errno!=0) 
				printf("%d\n",errno);
		}
		fscanf_s(f,"%hd %hhd", &(list->p[i].rotations), &(list->p[i].hasReflection));
		if (errno!=0) 
			printf("%d\n",errno);
		
	}

	fclose(f);
	return list;
}

piecelist_t * copyPieceList (piecelist_t * cpy, piecelist_t * src){
	short i,j,k;
	if (src == NULL) return NULL;
	if (cpy == NULL) cpy = (piecelist_t *) malloc (sizeof(piecelist_t));

	for (i=0; i<NUMPIECES; i++){
		cpy->p[i].isPlayed=src->p[i].isPlayed;
		cpy->p[i].hasReflection=src->p[i].hasReflection;
		cpy->p[i].rotations=src->p[i].rotations;
		for (j=0;j<5; j++){
			for (k=0; k<5; k++){
				cpy->p[i].map[j][k] = src->p[i].map[j][k];
			}
		}
	}
	return cpy;
}

piecelist_t * copyPieceListState (piecelist_t * cpy, piecelist_t * src){
	short i;
	if (src == NULL) return NULL;
	if (cpy == NULL) cpy = (piecelist_t *) malloc (sizeof(piecelist_t));

	for (i=0; i<NUMPIECES; i++){
		cpy->p[i].isPlayed=src->p[i].isPlayed;
	}
	return cpy;
}

void resetPieceList(piecelist_t * list){
	int i;
	for (i=0; i<NUMPIECES; i++) list->p[i].isPlayed=false;
}

move_t getMoveFromPiece(piece_t * p, short rotations, bool isReflected){
	move_t move;
	move_t refl;
	int i, j;
	
	if (rotations == 0){
		for (i=0; i<5; i++){
			for (j=0;j<5;j++){
				move.map[i][j] = p->map[i][j];
			}
		}
	}
	else if (rotations == 1){
		for (i=0; i<5; i++){
			for (j=0;j<5;j++){
				move.map[j][4-i] = p->map[i][j];
			}
		}
	}
	else if (rotations == 2){
		for (i=0; i<5; i++){
			for (j=0;j<5;j++){
				move.map[4-i][4-j] = p->map[i][j];
			}
		}
	}
	else if (rotations == 3){
		for (i=0; i<5; i++){
			for (j=0;j<5;j++){
				move.map[4-j][i] = p->map[i][j];
			}
		}
	}
	if (!isReflected)
		return move;
	
	for (i=0; i<5; i++){
		for (j=0; j<5; j++){
			refl.map[i][4-j] = move.map[i][j];
		}
	}
	return refl;
}


bool getMoveFromCode(char * code, short * x, short * y, short * pnum, short * rots, bool* refl){
	
	short rot=0;

	if (code == NULL) return false;
	if (rots == NULL) return false;
	if (x == NULL || y == NULL) return false;
	if (refl == NULL) return false;
	if (strlen(code) != 4) return false;

	if (strcmp(code,"0000")==0) return false;

	//sscanf(code,"%hx%hx%c%hd",x, y, &piece, &rot);

	*pnum = code[2] - 97;
	*x = ctoh(code[0]) - 1;
	*y = ctoh(code[1]) -1;

	*rots = (code[3]-48)/2;
	*refl = (code[3]-48)%2;

	return true;
}

bool getCode(char * code, short x, short y, short p, short rots, short refl){

	if (code == NULL) return false;

	sprintf(code,"%x%x%c%d",x+1,y+1,p+97,2*rots+refl);
//	printf("Movecode:%s\n",code);
	return true;
}


short ctoh(short c){
	if (c>96) return c-87;
	if (c>64) return c-55;
	return c-48;
}