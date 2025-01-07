#include "board.h"
#include "attacks.h"
#include "utils.h"

int material_score[12] = {
	100,      // white pawn score
	300,      // white knight scrore
	350,      // white bishop score
	500,      // white rook score
   1000,      // white queen score
  10000,      // white king score
   -100,      // black pawn score
   -300,      // black knight scrore
   -350,      // black bishop score
   -500,      // black rook score
  -1000,      // black queen score
 -10000,      // black king score
};

int evaluate(Board* board, AttackTables* attacks){
	int score = 0;
	
	U64 bitboard;

	int piece, square; 

	for (int curr_piece = P; curr_piece <= k; curr_piece++) {
		bitboard = board->bitboards[curr_piece];

		while (bitboard) {
			piece = curr_piece;
			square = get_ls1b_index(bitboard);
			score += material_score[piece];
			clear_bit(bitboard, square);
		}
	}

	return board->side == white ? score : score * -1;
}
