#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "globals.h"
#include "utils.h"

void reset_board(Board* board) {
	memset(board->bitboards, 0, sizeof(board->bitboards));
	memset(board->occupancies, 0, sizeof(board->occupancies));
	board->side = white;
	board->enpassant = no_sq;
	board->castle = 0;
}

Board* create_board() {
	Board* board = (Board*)malloc(sizeof(Board));
	if (!board) {
		fprintf(stderr, "Failed to allocate memory for board.\n");
		exit(1);
	}
	reset_board(board);

	return board;
}

Board* copy_board(Board* bb) {
	Board* board = (Board*)malloc(sizeof(Board));
	if (!board) {
		fprintf(stderr, "Failed to allocate memory for board.\n");
		exit(1);
	}
	memcpy(board->bitboards, bb->bitboards, sizeof(bb->bitboards));
	memcpy(board->occupancies, bb->occupancies, sizeof(bb->occupancies));
	board->side = bb->side;
	board->enpassant = bb->enpassant;
	board->castle = bb->castle;

	return board;
}



void print_board(Board* board) {
	printf("\n\n");
	for (int rank = 0; rank < 8; rank++) {
		printf(" %d", 8 - rank);
		for (int file = 0; file < 8; file++) {
			int square = rank * 8 + file;
			int piece = -1;

			// checking all bitboards of specific pieces (starting from white pawn and ending at black king) and assinging correct piece
			for (int bb_idx = P; bb_idx <= k; bb_idx++) {
				if (get_bit(board->bitboards[bb_idx], square)) piece = bb_idx;
			}


			printf(" %c", (piece == -1) ? '.' : ascii_pieces[piece]);
		}
		printf("\n");
	}
	printf("\n   a b c d e f g h\n\n");
	printf("Enpassant: %s\n", (board->enpassant != no_sq) ? square_to_coords[board->enpassant] : "none");
	printf("Side: %s\n", !board->side ? "white" : "black");
	printf("Castling: %c%c%c%c\n\n",
		(board->castle & wk) ? 'K' : '-',
		(board->castle & wq) ? 'Q' : '-',
		(board->castle & bk) ? 'k' : '-',
		(board->castle & bq) ? 'q' : '-'
	);
}

void parse_FEN(Board* board, char* fen) {
	reset_board(board);
	for (int square = 0; square < 64 && *fen && *fen != ' '; )
	{
		// match ascii pieces within FEN string
		if ((*fen >= 'b' && *fen <= 'r') || (*fen >= 'B' && *fen <= 'R'))
		{
			int piece = char_pieces[*fen];
			set_bit(board->bitboards[piece], square);
			square++;
			fen++;
		}

		// match empty square numbers within FEN string
		else if (*fen >= '1' && *fen <= '8')
		{
			int offset = *fen - '0';
			square += offset;
			fen++;
		}

		// rank separator
		else if (*fen == '/')
		{
			fen++;
		}
		else
		{
			fen++; // wrong character
		}
	}

	// setting side
	fen++;
	board->side = *fen == 'w' ? white : black;

	// setting castle rights
	fen += 2;
	while (*fen != ' ') {
		switch (*fen) {
		case 'K':
			board->castle |= wk;
			break;
		case 'Q':
			board->castle |= wq;
			break;
		case 'k':
			board->castle |= bk;
			break;
		case 'q':
			board->castle |= bq;
			break;
		default:
			break;
		}
		fen++;
	}
	fen++;

	// setting enpassant square
	if (*fen != '-') {
		int file = fen[0] - 'a';
		int rank = 8 - (fen[1] - '0');
		board->enpassant = rank * 8 + file;
	}

	// setting occupancies
	for (int piece = P; piece <= K; piece++) {
		board->occupancies[white] |= board->bitboards[piece];
	}
	for (int piece = p; piece <= k; piece++) {
		board->occupancies[black] |= board->bitboards[piece];
	}
	board->occupancies[both] |= board->occupancies[white] | board->occupancies[black];
}
