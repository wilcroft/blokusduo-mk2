#include "pieceutils.h"
#include "boardutils.h"
#include "gameutils.h"
#include <stdio.h>

#define MODE 7

int main(void){
	piecelist_t l [ NUMPLAYERS];
	//piecelist_t ltwo;
	board_t b;
	move_t m;
	tilelistgroup_t liberties;
	int i,j,k;

	bool p1,p2;
	char code [5];

	time_t start;
	time_t end;

	initializePiecelist(&l[0]);
	initializePiecelist(&l[1]);
	initializeBoard(&b);

	for (i=0; i<NUMPLAYERS; i++) liberties.l[i]=NULL;

	updateLiberties(&b,&liberties);
#if (MODE==0) //PIECE LIST TEST
	for (i=0; i<NUMPIECES; i++){
		for (j=0; j<5; j++){
			for (k=0; k<5; k++){
				printf("%d ",l.p[i].map[j][k]);
			}
			printf("\n");
		}
		printf("---------\n");
	}
#elif (MODE == 1) //PIECE -> MOVE TEST
	for (i=0; i<8; i++){
		for (j=0; j<5; j++){
			for (k=0; k<5; k++){
				m=getMoveFromPiece(&(l.p[NUMPIECES-2]),i/2,i%2);
				printf("%d ",m.map[j][k]);
			}
			printf("\n");
		}
		printf("---------\n");
	}

#elif (MODE == 2)
	for (i=0;i<4;i++){
		m=getMoveFromPiece(&(l.p[NUMPIECES-2]),i,0);
		for (j=0;j<5;j++){
			for (k=0;k<5;k++){
				printf("%d ",m.map[j][k]);
			}
			printf("\n");
		}
		printf("---------\n");
		m=getMoveFromPiece(&(l.p[NUMPIECES-2]),i,1);
		for (j=0;j<5;j++){
			for (k=0;k<5;k++){
				printf("%d ",m.map[j][k]);
			}
			printf("\n");
		}
		printf("---------\n");
	}
#elif (MODE == 3) //BOARD TEST
	//placePiece(&b,&l,19,0,0,4,4,0,&liberties);
	placePieceFromCode("55t0",&b,&l,0,&liberties);
	drawBoard(&b);
	printf("Player X Moves: ");
	printLibertyList(liberties.l[0]);
	printf("Player O Moves: ");
	printLibertyList(liberties.l[1]);
	printf("\n--------------------------------------\n");
	//placePiece(&b,&l,20,0,0,9,9,1,&liberties);
	placePieceFromCode("aau0",&b,&l,1,&liberties);
	drawBoard(&b);
	printf("Player X Moves: ");
	printLibertyList(liberties.l[0]);
	printf("Player O Moves: ");
	printLibertyList(liberties.l[1]);
	printf("\n--------------------------------------\n");
	//placePiece(&b,&l,0,0,0,2,2,0,&liberties);
	placePieceFromCode("33a0",&b,&l,0,&liberties);
	drawBoard(&b);
	printf("Player X Moves: ");
	printLibertyList(liberties.l[0]);
	printf("Player O Moves: ");
	printLibertyList(liberties.l[1]);
	printf("\n--------------------------------------\n");
	//placePiece(&b,&l,20,0,0,11,11,1,&liberties);
	placePieceFromCode("ccu1",&b,&l,1,&liberties);
	drawBoard(&b);
	printf("Player X Moves: ");
	printLibertyList(liberties.l[0]);
	printf("Player O Moves: ");
	printLibertyList(liberties.l[1]);
#elif (MODE == 4) //SINGLE-PLAYER MOVE TEST
	do{
		getDumbMoveCode(&b,&l,1,&liberties,code);
		if (placePieceFromCode(code,&b,&l,1,&liberties)){
			drawMarkedBoard(&b,1);
			printf("Player X Moves: ");
			printLibertyList(liberties.l[1]);
			printf("--------------------------------------\n");
		}
		else printf("Code '%s' is invalid\n", code);
		getchar();
	} while (strcmp(code,"0000")!=0);
	printf("done\n");
#elif (MODE == 5) // SIMPLE VERSUS TEST
	i=1;
	j=1;
	do{
		if (i==1){
			getDumbMoveCode(&b,&l,0,&liberties,code);
			if (strcmp(code,"0000")==0){
				i=0;
				printf("P1 done.\n");
			}
			else if (placePieceFromCode(code, &b, &l, 0, &liberties)){
				printf("Player 1:\n");
				drawBoard(&b);
				printf("--------------------------------------\n");
			}
			else printf ("P1: Code '%s' is invalid\n", code);
			getchar();
		}
		if (j==1){
			getDumbMoveCode(&b,&ltwo,1,&liberties,code);
			if (strcmp(code,"0000")==0){
				j=0;
				printf("P2 done.\n");
			}
			else if (placePieceFromCode(code, &b, &ltwo, 1, &liberties)){
				printf("Player 2:\n");
				drawBoard(&b);
				printf("--------------------------------------\n");
			}
			else printf ("P2: Code '%s' is invalid\n", code);
			getchar();
		}


	}while (i!=0 || j !=0);

	printf("done!\n");
#elif (MODE == 6) // DUMB VERSUS RANDOM TEST
	i=1;
	j=1;
	do{
		if (i==1){
			//getBetterRandomMoveCode(&b,&l[0],0,&liberties,code);
			getLookAheadMoveCode(&b,l,0,&liberties,code);
			if (strcmp(code,"0000")==0){
				i=0;
				printf("P1 done.\n");
			}
			else if (placePieceFromCode(code, &b, &l[0], 0, &liberties)){
				printf("Player 1: %s\n",code);
				drawBoard(&b);
				printf("--------------------------------------\n");
			}
			else{
				printf ("P1: Code '%s' is invalid\n", code);
				printf("Player X Moves: ");
				printLibertyList(liberties.l[0]);
			}
			getchar();
		}
		if (j==1){
			getDumbMoveCode(&b,&l[1],1,&liberties,code);
			if (strcmp(code,"0000")==0){
				j=0;
				printf("P2 done.\n");
			}
			else if (placePieceFromCode(code, &b, &l[1], 1, &liberties)){
				printf("Player 2: %s\n",code);
				drawBoard(&b);
				printf("--------------------------------------\n");
			}
			else printf ("P2: Code '%s' is invalid\n", code);
			getchar();
		}


	}while (i!=0 || j !=0);

	printf("done!\n");
#elif (MODE == 7)  // MULTIGAME MODE
	j=0;
	k=0;
	start=time(NULL);
	for (i=0; i<1000; i++){
		if (i%50==0) printf("At %d (+%f)\n",i,difftime(time(NULL),start));
		p1=true;
		p2=true;
		do{
			if (p1){
				//getBetterRandomMoveCode(&b,&l[0],0,&liberties,code);
				getLookAheadMoveCode(&b,l,0,&liberties,code);
				if (strcmp(code,"0000")==0){
					p1=false;
//					printf("P1 done.\n");
				}
				else if (placePieceFromCode(code, &b, &l[0], 0, &liberties)){
//					printf("Player 1: %s\n",code);
//					drawBoard(&b);
//					printf("--------------------------------------\n");
				}
				else printf ("P1: Code '%s' is invalid\n", code);
//				getchar();
			}
			if (p2){
				//getDumbMoveCode(&b,&l[1],1,&liberties,code);
				getBetterRandomMoveCode(&b,&l[1],1,&liberties,code);
				if (strcmp(code,"0000")==0){
					p2=false;
//					printf("P2 done.\n");
				}
				else if (placePieceFromCode(code, &b, &l[1], 1, &liberties)){
//					printf("Player 2: %s\n",code);
//					drawBoard(&b);
//					printf("--------------------------------------\n");
				}
				else printf ("P2: Code '%s' is invalid\n", code);
//				getchar();
			}
		}while (p1 || p2);
//		drawBoard(&b);
//		printf("--------------------------------------\n");
		if (b.score[0] > b.score[1]) j++;
		else if (b.score[0] < b.score[1]) k++;
		
		initializeBoard(&b);
		updateLiberties(&b,&liberties);
		resetPieceList(&l[0]);
		resetPieceList(&l[1]);
	}
	end=time(NULL);
	printf("P1: %d (%f) \t P2: %d (%f)\n", j,j/i,k,k/i);
	printf("time: %f\n", difftime(end,start));
#endif

	getchar();
	return 0;
}