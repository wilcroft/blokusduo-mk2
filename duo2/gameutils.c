#include "gameutils.h"

bool isMoveValid(board_t * b, move_t * m, int x, int y, int player){
	bool onLiberty = false;
	int i, j;
	if (x < 0 || y < 0 || x >= BOARDWIDTH || y >= BOARDHEIGHT || player < 0 || player >= NUMPLAYERS)
		return false;

/*	for (i=0; i<5; i++){
		if ((x-2+i)>=0 && (x-2+i)<BOARDWIDTH){
			for (j=0; j<5; j++){
				if ((y-2+j)>=0 && (y-2+j)<BOARDHEIGHT){
					if (m->map[i][j] && !(b->tile[y-2+j][x-2+i].isAvailable[player]))
						return false;
					if (m->map[i][j] && (b->tile[y-2+j][x-2+i].isLiberty[player]))
						onLiberty = true;
				}
			}
		}
	}*/

	/* POTENTIAL DEPRECATION FOR BAD RESULTS
	//check tiles within r=2 for validity
	for (i=0; i<3; i++){
		//top half
		if ((y-i)>=0){
			for (j=-i;j<=i; j++){
				if ((x+j)>=0 && (x+j) < BOARDWIDTH){
					if (m->map[2-i][j+2] && !(b->tile[y-i][x+j].isAvailable[player]))
						return false;
					if (m->map[2-i][j+2] && (b->tile[y-i][x+j].isLiberty[player]))
						onLiberty = true;
				}
			}
		}
		//bottom half (if i!= 0)
		if (i!=0 && (y+i)<BOARDHEIGHT){
			for (j=-i;j<=i; j++){
				if ((x+j)>=0 && (x+j) < BOARDWIDTH){
					if (m->map[2+i][j+2] && !(b->tile[y+i][x+j].isAvailable[player]))
						return false;
					if (m->map[2+i][j+2] && (b->tile[y+i][x+j].isLiberty[player]))
						onLiberty = true;
				}
			}
		}
	}
	*/
	//check tiles within r=2 for validity
	for (i=0; i<3; i++){
		//for (j=-i;j<=i; j++){
		for (j=i; j<5-i; j++){
		//top half
			if ((y-i)>=0 && (x+j-2)>=0 && (x+j-2) < BOARDWIDTH){
					if (m->map[2-i][j] && !(b->tile[y-i][x+j-2].isAvailable[player]))
						return false;
					if (m->map[2-i][j] && (b->tile[y-i][x+j-2].isLiberty[player]))
						onLiberty = true;
			}
			else if (m->map[2-i][j]) return false;
		//bottom half (if i!=0)
			if (i!=0){
				if ((y+i) < BOARDHEIGHT && (x+j-2)>=0 && (x+j-2) < BOARDWIDTH){
					if (m->map[2+i][j] && !(b->tile[y+i][x+j-2].isAvailable[player]))
						return false;
					if (m->map[2+i][j] && (b->tile[y+i][x+j-2].isLiberty[player]))
						onLiberty = true;
				}
				else if (m->map[2+i][j]) return false;
			}
		}
	}

/*	for (i=0; i<3; i++){
		if ((x-2+i)>=0){
			for (j=-i;j<=i;j++){
				if((y+j)>=0 && (y+j)<BOARDHEIGHT){
					if (m->map[i][j+2] && !(b->tile[y+j][x-2+i].isAvailable[player]))
						return false;
					if (m->map[i][j+2] && (b->tile[y+j][x-2+i].isLiberty[player]))
						onLiberty = true;
				}
			}
		}
		if ((x-2+i)>=BOARDWIDTH){
			for (j=-i;j<=i;j++){
				if((y+j)>=0 && (y+j)<BOARDHEIGHT){
					if (m->map[i][j+2] && !(b->tile[y+j][x-2+i].isAvailable[player]))
						return false;
					if (m->map[i][j+2] && (b->tile[y+j][x-2+i].isLiberty[player]))
						onLiberty = true;
				}
			}
		}
	}
	*/
	return onLiberty;
}



bool placePiece(board_t * b, piecelist_t * plist, short pnum, short rotations, bool reflected, short x, short y, uint8_t player, tilelistgroup_t * tlist){
	move_t m;
	int i, j;
//	tilelist_t * list [NUMPLAYERS];

	if (plist->p[pnum].isPlayed) return false;
	
//	for (i=0; i<NUMPLAYERS; i++) list[i] = NULL;

	m = getMoveFromPiece(&(plist->p[pnum]), rotations,reflected);

	if (!isMoveValid(b,&m,x,y,player)) return false;

	for (j=0; j<5; j++){
		if ((y-2+j)>=0 && (y-2+j)<BOARDHEIGHT){
			for (i=0; i<5; i++){
				if ((x-2+i)>=0 && (x-2+i)<BOARDWIDTH){
					if(m.map[j][i]){
						b->tile[y-2+j][x-2+i].isOccupied = true;
						b->tile[y-2+j][x-2+i].owningPlayer = player;
						b->score[player] ++;
					}
				}
			}
		}
	}

	updateLiberties(b,tlist);
	plist->p[pnum].isPlayed = true;

	//getCode(code,x,y,pnum,rotations,reflected);
	
	
	return true;
}

bool placePieceFromCode(char * code, board_t * b, piecelist_t * plist, uint8_t player, tilelistgroup_t * tlist){
//	move_t move;
	short x;
	short y;
	short pnum;
	short rots;
	bool refl;

	if (!getMoveFromCode(code,&x,&y,&pnum,&rots,&refl)) return false;
	return placePiece(b,plist,pnum,rots,refl,x,y,player,tlist);
}

void getDumbMoveCode(board_t * b, piecelist_t * plist, short player,tilelistgroup_t * tlist, char * code){
	move_t move;
	short x, y;
	short pnum,refl,rots;
	short pnum_max;

//	short i,j,k,a;
	tilelist_t * ptr;
	if (b == NULL || plist == NULL || tlist == NULL || code == NULL) return;
	if (tlist->l[player] == NULL) {
		strcpy(code,"0000");
		return;
	}

	ptr = tlist->l[player];
	pnum_max=NUMPIECES-1;
	refl=0;
	//find the biggest piece not yet played.
	while (plist->p[pnum_max].isPlayed) pnum_max--;
	// try
	do{
		for(pnum=pnum_max; pnum >=0; pnum--){
			if (!plist->p[pnum].isPlayed){
				for(rots=0;rots < plist->p[pnum].rotations; rots++){
					for(refl=0; refl <= plist->p[pnum].hasReflection; refl++){
						move = getMoveFromPiece(&(plist->p[pnum]),rots,refl);
/////////////////////////////////////////////////////////////////////////////////////////
///////////// NEEDS OPTIMIZING!!! ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
						for (x=-2; x<3; x++){
							for (y=-2; y<3; y++){
								if (isMoveValid(b,&move,x+(ptr->t->x),y+(ptr->t->y),player)){
									getCode(code,x+(ptr->t->x),y+(ptr->t->y),pnum,rots,refl);
									return;
								}
							}
						}
/////////////////////////////////////////////////////////////////////////////////////////
					}
				}
			}
		}
		ptr=ptr->next;
	} while(ptr !=NULL);

	if (ptr==NULL){
		strcpy(code,"0000");
		return;
	}
	//getCode(code,x,y,pnum,rots,refl);
}

void getRandomMoveCode(board_t * b, piecelist_t * plist, short player,tilelistgroup_t * tlist, char * code){

	short x, y;
	short pnum;
	short rots;
	bool refl;
	short libnum;
	short i;
	short attempts;
	move_t move;

	tilelist_t * ptr;

	if (b == NULL || plist == NULL || tlist == NULL || code == NULL) return;
	if (tlist->l[player] == NULL) {
		strcpy(code,"0000");
		return;
	}

	srand(time(NULL));

	for (attempts=0; attempts<MAX_RAND_ATEMPTS; attempts++){
		ptr = tlist->l[player];
		x = rand()%5-2;
		y = rand()%5-2;
		do{
			pnum = rand()%NUMPIECES;
		} while (plist->p[pnum].isPlayed);
		rots = rand()%(plist->p[pnum].rotations);
		if (plist->p[pnum].hasReflection) refl = rand()%2;
		else refl = 0;
		libnum = rand()%20;
		if (tlist->l[player]->next!=NULL){
			for (i=0; i<libnum; i++){
				ptr = ptr->next;
				if (ptr == NULL) ptr = tlist->l[player];
			}
		}
		
		move = getMoveFromPiece(&(plist->p[pnum]),rots,refl);
		if (isMoveValid(b,&move,x+(ptr->t->x),y+(ptr->t->y),player)){
			getCode(code,x+(ptr->t->x),y+(ptr->t->y),pnum,rots,refl);
			return;
		}
	}

	strcpy(code,"0000");

}

void getBetterRandomMoveCode(board_t * b, piecelist_t * plist, short player,tilelistgroup_t * tlist, char * code){
	move_t move;
	short x, y;
	short pnum,refl,rots;
	short pnum_max;

	short i;
	tilelist_t * ptr;
	movecodelist_t movelist;
	movelist.count = 0;

	if (b == NULL || plist == NULL || tlist == NULL || code == NULL) return;
	if (tlist->l[player] == NULL) {
		strcpy(code,"0000");
		return;
	}

	ptr = tlist->l[player];
	pnum_max=NUMPIECES-1;
	refl=0;
	srand(time(NULL));

	//find the biggest piece not yet played.
	while (plist->p[pnum_max].isPlayed) pnum_max--;
	// try
	do{
		for(pnum=pnum_max; pnum >=0; pnum--){
			if (!plist->p[pnum].isPlayed){
				for(rots=0;rots < plist->p[pnum].rotations; rots++){
					for(refl=0; refl <= plist->p[pnum].hasReflection; refl++){
						move = getMoveFromPiece(&(plist->p[pnum]),rots,refl);
/////////////////////////////////////////////////////////////////////////////////////////
///////////// NEEDS OPTIMIZING!!! ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
						for (x=-2; x<3; x++){
							for (y=-2; y<3; y++){
								if (isMoveValid(b,&move,x+(ptr->t->x),y+(ptr->t->y),player)){
									getCode(movelist.code[movelist.count],x+(ptr->t->x),y+(ptr->t->y),pnum,rots,refl);
									movelist.count++;
									if (movelist.count==MOVELISTLENGTH){
//										printf("filled\n");
										i=rand()%MOVELISTLENGTH;
										strcpy(code,movelist.code[i]);
										return;
									}
								}
							}
						}
/////////////////////////////////////////////////////////////////////////////////////////
					}
				}
			}
		}
		ptr=ptr->next;
	} while(ptr !=NULL);

	if (movelist.count==0){
		strcpy(code,"0000");
		return;
	}
	
	i=rand()%movelist.count;
	strcpy(code,movelist.code[i]);
	//getCode(code,x,y,pnum,rots,refl);

}

void getLookAheadMoveCode(board_t * b, piecelist_t * plist, short player,tilelistgroup_t * tlist, char * code){
	move_t move;
	short x, y;
	short pnum,refl,rots;
	short pnum_max;

	short i,j,k;
	tilelist_t * ptr;
	movecodelist_t movelist;
	board_t bcopy;
	piecelist_t plistcopy [NUMPLAYERS];
	bool pdone[NUMPLAYERS];
	bool alldone;
	char pcode[5];
	short pts;
	tilelistgroup_t tlistcopy;

	movelist.count = 0;

	if (b == NULL || plist == NULL || tlist == NULL || code == NULL) return;
	if (tlist->l[player] == NULL) {
		strcpy(code,"0000");
		return;
	}
	
	for (j=0; j< NUMPLAYERS; j++){
			copyPieceList(&(plistcopy[j]), &plist[j]);
			tlistcopy.l[j] = NULL;
	}
	ptr = tlist->l[player];
	pnum_max=NUMPIECES-1;
	refl=0;

	//find the biggest piece not yet played.
	while (plist[player].p[pnum_max].isPlayed) pnum_max--;
	// try
	do{
		for(pnum=pnum_max; pnum >=0; pnum--){
			if (!plist[player].p[pnum].isPlayed){
				for(rots=0;rots < plist[player].p[pnum].rotations; rots++){
					for(refl=0; refl <= plist[player].p[pnum].hasReflection; refl++){
						move = getMoveFromPiece(&(plist[player].p[pnum]),rots,refl);
/////////////////////////////////////////////////////////////////////////////////////////
///////////// NEEDS OPTIMIZING!!! ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
						for (x=-2; x<3; x++){
							for (y=-2; y<3; y++){
								if (movelist.count<MOVELISTLENGTH && isMoveValid(b,&move,x+(ptr->t->x),y+(ptr->t->y),player)){
									getCode(movelist.code[movelist.count],x+(ptr->t->x),y+(ptr->t->y),pnum,rots,refl);
									movelist.count++;
								}
							}
							if (movelist.count>= MOVELISTLENGTH) break;
						}
						if (movelist.count>= MOVELISTLENGTH) break;
/////////////////////////////////////////////////////////////////////////////////////////
					}
					if (movelist.count>= MOVELISTLENGTH) break;
				}
			}
			if (movelist.count>= MOVELISTLENGTH) break;
		}
		ptr=ptr->next;
	} while(ptr !=NULL && movelist.count<MOVELISTLENGTH);

	if (movelist.count==0){
		strcpy(code,"0000");
		return;
	}

	pts = -50;
	//Do look-aheads
	for (i=0; i<movelist.count; i++){
		//Make copies of current board state to work with
		copyBoard(&bcopy,b);
		//copyTileListGroup(&tlistcopy,tlist);
		for (j=0; j< NUMPLAYERS; j++){
			copyPieceListState(&(plistcopy[j]), &plist[j]);
			pdone[j]=false;
		}
		alldone=false;

		//place test move i;
		placePieceFromCode(movelist.code[i],&bcopy,&(plistcopy[player]),player,&tlistcopy);
		j = (player+1)%NUMPLAYERS;
		while (!alldone){
			getDumbMoveCode(&bcopy,&plistcopy[j],j,&tlistcopy,pcode);
			if (strcmp(pcode,"0000")==0) 
				pdone[j] = true;
			else
				placePieceFromCode(pcode,&bcopy,&plistcopy[j],j,&tlistcopy);
			j= (j+1)%NUMPLAYERS;
			alldone=true;
			for(k=0; k<NUMPLAYERS; k++) alldone = pdone[k] & alldone;
		}
		if((bcopy.score[player]-bcopy.score[(player+1)%NUMPLAYERS])>pts){
			strcpy(code,movelist.code[i]);
			pts = bcopy.score[player]-bcopy.score[(player+1)%NUMPLAYERS];
		}
	}
	//getCode(code,x,y,pnum,rots,refl);
}