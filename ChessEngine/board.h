#ifndef BOARD_H
#define BOARD_H
#include "globals.h"

typedef struct {
	U64 bitboards[12];
	U64 occupancies[3];
	int side;
	int enpassant;
	int castle;
} Board;

void reset_board(Board* board);
Board* create_board();
Board* copy_board(Board* bb);
void print_board(Board* board);
void parse_FEN(Board* board, char* fen);


#endif 