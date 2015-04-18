#include "boardutils.h"

board_t * initializeBoard(board_t * b){
	int i, j, k;
	if (b==NULL)
		b= (board_t *) malloc (sizeof(board_t));
	
	for (i=0; i<BOARDHEIGHT; i++){
		for (j=0; j<BOARDWIDTH; j++){
			b->tile[i][j].isOccupied = false;
			b->tile[i][j].owningPlayer = NUMPLAYERS;
			b->tile[i][j].x = j;
			b->tile[i][j].y = i;
			for (k=0; k<NUMPLAYERS; k++){
				b->tile[i][j].isLiberty[k] = false;
				b->tile[i][j].isAvailable[k] = true;
			}
		}
	}
	
	for (k=0; k<NUMPLAYERS; k++){
		b->score[k]=0;
	}

	if(ISDUO){
		b->tile[4][4].isLiberty[0] = true;
		b->tile[9][9].isLiberty[1] = true;
	}
	
	return b;
}

board_t * copyBoard(board_t * b, const board_t * src){
	int i, j;
	if (b==NULL)
		b= (board_t *) malloc (sizeof(board_t));
	
	for (i=0; i<BOARDHEIGHT; i++){
		for (j=0; j<BOARDWIDTH; j++){
			b->tile[i][j] = src->tile[i][j];
		}
	}
	
	for (i=0; i<NUMPLAYERS; i++){
		b->score[i] = src->score[i];
	}
	return b;
}

void updateLiberties(board_t * b, tilelistgroup_t * libList){//tilelist_t * libList [NUMPLAYERS]){
	int i, j, k;
	for (k=0; k<NUMPLAYERS; k++){
		emptyList(&(libList->l[k]));
	}
	for (i=0; i<BOARDHEIGHT; i++){
		for (j=0; j<BOARDWIDTH; j++){
			//Tile is Occupied - not a liberty!
			if (b->tile[i][j].isOccupied){
				for (k=0; k<NUMPLAYERS; k++){
					b->tile[i][j].isLiberty[k] = false;
					b->tile[i][j].isAvailable[k] = false;
				}
			}
			else {
				for (k=0; k<NUMPLAYERS; k++){
					//Tile is adjacent to an occupied tile of the same player - not a liberty!
					if ( (i>0 && b->tile[i-1][j].owningPlayer==k) ||
							(j>0 && b->tile[i][j-1].owningPlayer==k)  ||
							(i<BOARDHEIGHT-1 && b->tile[i+1][j].owningPlayer==k) ||
							(j<BOARDWIDTH-1 && b->tile[i][j+1].owningPlayer==k) ){
						b->tile[i][j].isLiberty[k] = false;
						b->tile[i][j].isAvailable[k] = false;
					}

					//Tile is diagonally-adjacent to one of the same player - a liberty!
					else if ( (i>0 && j>0 && b->tile[i-1][j-1].owningPlayer==k) ||
								(i>0 && j<BOARDWIDTH-1 && b->tile[i-1][j+1].owningPlayer==k) ||
								(i<BOARDHEIGHT-1 && j>0 && b->tile[i+1][j-1].owningPlayer==k) ||
								(i<BOARDHEIGHT-1 && j<BOARDWIDTH-1 && b->tile[i+1][j+1].owningPlayer==k)) {
						b->tile[i][j].isLiberty[k] = true;
						b->tile[i][j].isAvailable[k] = true;
						if (k==0)
							libList->l[k] = addListTileFront (libList->l[k], &(b->tile[i][j]));
						else
							libList->l[k] = addListTileBack (libList->l[k], &(b->tile[i][j]));
					}

					//Tile is not near an occupied tile - not a liberty! (but available)
					else{
						b->tile[i][j].isLiberty[k] = false;
						b->tile[i][j].isAvailable[k] = true;
					}
				}
			}
		}
	}
	
	if(ISDUO){
		if (b->tile[4][4].isAvailable[0]){
			b->tile[4][4].isLiberty[0] = true;
			libList->l[0] = addListTileFront (libList->l[0], &(b->tile[4][4]));
		}
		if (b->tile[9][9].isAvailable[1]){
			b->tile[9][9].isLiberty[1] = true;
			libList->l[1] = addListTileFront (libList->l[1], &(b->tile[9][9]));
		}
	}

}	

void emptyList(tilelist_t ** t){
	tilelist_t * p;
	while (*t !=NULL){
		p = (*t)->next;
		free(*t);
		(*t)=p;
	}
}

/* addListItemFront - Add a new tilelist (item) to the head of a tilelist
 *		then return a pointer to the head of the list
 */
tilelist_t * addListItemFront(tilelist_t * list, tilelist_t * item){
	item->next = list;
	list = item;
	return item;
}

/* addListTileFront - Create a new tilelist from a tile, then add it to
 *		the head of a list. Then return a pointer to the head of the list
 */
tilelist_t * addListTileFront(tilelist_t * list, tile_t * tile){
	tilelist_t * item = (tilelist_t *) malloc(sizeof(tilelist_t));
	item->next = list;
	item->t = tile;
	list = item;
	return item;
}

/* addListItemBack - Add a new tilelist (item) to the tail of a tilelist
 *		then return a pointer to the head of the list
 */
tilelist_t * addListItemBack(tilelist_t * list, tilelist_t * item){
	tilelist_t * p;
	if (list == NULL) return item;
	p = list;
	while(p->next!=NULL){
		p = p->next;
	}
	p->next = item;
	return list;
}

/* addListTileBack - Create a new tilelist from a tile, then add it to
 *		the tail of a list. Then return a pointer to the head of the list
 */
tilelist_t * addListTileBack(tilelist_t * list, tile_t * tile){
	tilelist_t * item = (tilelist_t *) malloc(sizeof(tilelist_t));
	item->next = NULL;
	item->t = tile;
	return addListItemBack(list,item);
}

void drawBoard(board_t * b){
	int i, j;
	if (b==NULL) return;
	printf("  123456789abcde\n");
	for (i=0; i<BOARDHEIGHT; i++){
		printf("%x ",i+1);
		for (j=0; j<BOARDWIDTH; j++){
			if (b->tile[i][j].isOccupied){
				if (b->tile[i][j].owningPlayer==0) printf("X");
				else if (b->tile[i][j].owningPlayer==1) printf("O");
			}
			else
				printf (".");
		}
		printf("\n");
	}
	printf("X: %d \tO: %d\n\n", b->score[0], b->score[1]);
}
void drawMarkedBoard(board_t * b,short player){
	int i, j;
	if (b==NULL) return;
	printf("  123456789abcde\n");
	for (i=0; i<BOARDHEIGHT; i++){
		printf("%x ",i+1);
		for (j=0; j<BOARDWIDTH; j++){
			if (b->tile[i][j].isOccupied){
				if (b->tile[i][j].owningPlayer==0) printf("X");
				else if (b->tile[i][j].owningPlayer==1) printf("O");
			}
			else if (b->tile[i][j].isLiberty[player]) printf("*");
			else if (b->tile[i][j].isAvailable[player]) printf("-");
			else printf (".");
		}
		printf("\n");
	}
	printf("X: %d \tO: %d\n\n", b->score[0], b->score[1]);
}

void printLibertyList(tilelist_t * list){

	while (list != NULL){
		printf("(%x,%x) ",list->t->x+1,list->t->y+1);
		list=list->next;
	}
	printf("\n");
}

tilelistgroup_t * copyTileListGroup(tilelistgroup_t * cpy, tilelistgroup_t * src){
	int i;
	tilelist_t * cpyptr;
	tilelist_t * srcptr;

	if (src == NULL) return NULL;
	if (cpy == NULL){
		cpy = (tilelistgroup_t *) malloc(sizeof(tilelistgroup_t));
		for (i=0; i<NUMPLAYERS; i++) cpy->l[i] = NULL;
	}

	for (i=0; i<NUMPLAYERS; i++){
		if (cpy->l[i] != NULL)  emptyList(&(cpy->l[i]));
		srcptr=src->l[i];
		cpyptr=cpy->l[i];
		while (srcptr !=NULL){
			if (cpyptr != NULL){
				cpyptr->next = (tilelist_t *) malloc (sizeof(tilelist_t));
				cpyptr = cpyptr->next;
			}
			else {
				cpy->l[i] = (tilelist_t *) malloc (sizeof(tilelist_t));
				cpyptr=cpy->l[i];
			}
			cpyptr->t = srcptr->t;
			srcptr = srcptr->next;
		}
	}
	
	return cpy;
}
