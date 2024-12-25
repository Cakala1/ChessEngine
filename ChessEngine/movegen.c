#include <stdio.h>
#include <stdlib.h>
#include "movegen.h"
#include "randoms.h"
#include "utils.h"

Moves* create_moves() {
	Moves* moves = (Moves*)malloc(sizeof(Moves));
	if (!moves) {
		fprintf(stderr, "Failed to allocate memory for moves.\n");
		exit(1);
	}
	moves->count = 0;
	return moves;
}

void add_move(Moves* moves_list, int move) {
	moves_list->moves[moves_list->count++] = move;
}

void print_move(int move) {
	printf("%s%s%c\n",
		square_to_coords[get_move_source(move)],
		square_to_coords[get_move_target(move)],
		ascii_pieces[get_move_promoted(move)]);
}

void print_move_list(Moves* move_list) {
	printf("\nmove     piece     capture    double_push    enpassant    castling\n");
	int _count = 0;
	for (; _count < move_list->count; _count++) {
		int move = move_list->moves[_count];
		printf("%s%s%c    %c         %d          %d              %d             %d\n",
			square_to_coords[get_move_source(move)],
			square_to_coords[get_move_target(move)],
			ascii_pieces[get_move_promoted(move)],
			ascii_pieces[get_move_piece(move)],
			get_move_capture(move) ? 1 : 0,
			get_move_double_push(move) ? 1 : 0,
			get_move_enpassant(move) ? 1 : 0,
			get_move_castling(move) ? 1 : 0
		);
	}
	printf("\n\nTotal number of moves: %d\n\n", _count);
}





void handle_piece_moves(Board* board, AttackTables* attack_tables, int piece, const char* piece_name) {
	U64 current_bb = board->bitboards[piece];
	while (current_bb) {
		int source_square = get_ls1b_index(current_bb);
		U64 attacks = 0ULL;

		// Get appropriate attacks based on piece type
		if (piece == N || piece == n)
			attacks = attack_tables->knight_attacks[source_square];
		else if (piece == B || piece == b)
			attacks = get_bishop_attacks(attack_tables, source_square, board->occupancies[both]);
		else if (piece == R || piece == r)
			attacks = get_rook_attacks(attack_tables, source_square, board->occupancies[both]);
		else if (piece == Q || piece == q)
			attacks = get_queen_attacks(attack_tables, source_square, board->occupancies[both]);
		else if (piece == K || piece == k)
			attacks = attack_tables->king_attacks[source_square];

		// Filter attacks by side
		attacks &= (board->side == white) ? ~board->occupancies[white] : ~board->occupancies[black];

		while (attacks) {
			int target_square = get_ls1b_index(attacks);
			int is_capture = get_bit((board->side == white) ? board->occupancies[black] : board->occupancies[white], target_square);

			printf("%s %s: %s to %s\n",
				piece_name,
				is_capture ? "capture" : "move",
				square_to_coords[source_square],
				square_to_coords[target_square]);

			clear_bit(attacks, target_square);
		}
		clear_bit(current_bb, source_square);
	}
}

// Helper function for pawn moves
void handle_pawn_moves(Board* board, AttackTables* attack_tables, int side) {
	U64 current_bb = board->bitboards[side == white ? P : p];
	while (current_bb) {
		int source_square = get_ls1b_index(current_bb);
		int target_square = side == white ? source_square - 8 : source_square + 8;

		// Single push
		if (!(target_square < a8 || target_square > h1) && !get_bit(board->occupancies[both], target_square)) {
			if ((side == white && source_square >= a7 && source_square <= h7) ||
				(side == black && source_square >= a2 && source_square <= h2)) {
				printf("Pawn promote: %s to %s\n", square_to_coords[source_square], square_to_coords[target_square]);
			}
			else {
				printf("Pawn push: %s to %s\n", square_to_coords[source_square], square_to_coords[target_square]);

				// Double push
				int double_push = side == white ? target_square - 8 : target_square + 8;
				if (((side == white && source_square >= a2 && source_square <= h2) ||
					(side == black && source_square >= a7 && source_square <= h7)) &&
					!get_bit(board->occupancies[both], double_push)) {
					printf("Double Pawn push: %s to %s\n",
						square_to_coords[source_square],
						square_to_coords[double_push]);
				}
			}
		}

		// Captures
		U64 attacks = attack_tables->pawn_attacks[side][source_square] &
			(side == white ? board->occupancies[black] : board->occupancies[white]);
		while (attacks) {
			target_square = get_ls1b_index(attacks);
			if ((side == white && source_square >= a7 && source_square <= h7) ||
				(side == black && source_square >= a2 && source_square <= h2)) {
				printf("Pawn capture and promote: %s to %s\n",
					square_to_coords[source_square],
					square_to_coords[target_square]);
			}
			else {
				printf("Pawn capture: %s to %s\n",
					square_to_coords[source_square],
					square_to_coords[target_square]);
			}
			clear_bit(attacks, target_square);
		}

		// En passant
		if (board->enpassant != no_sq) {
			U64 enpassant_attacks = attack_tables->pawn_attacks[side][source_square] & (1ULL << board->enpassant);
			if (enpassant_attacks) {
				printf("Enpassant capture: %s to %s\n",
					square_to_coords[source_square],
					square_to_coords[board->enpassant]);
			}
		}

		clear_bit(current_bb, source_square);
	}
}

// Helper function for castle moves
void handle_castle_moves(Board* board, AttackTables* attack_tables, int side) {
	if (side == white) {
		if (board->castle & wk) {
			if (!get_bit(board->occupancies[both], f1) && !get_bit(board->occupancies[both], g1)) {
				if (!is_square_attacked(board, attack_tables, e1, black) &&
					!is_square_attacked(board, attack_tables, f1, black)) {
					printf("Castle move: e1 to g1\n");
				}
			}
		}
		if (board->castle & wq) {
			if (!get_bit(board->occupancies[both], b1) &&
				!get_bit(board->occupancies[both], c1) &&
				!get_bit(board->occupancies[both], d1)) {
				if (!is_square_attacked(board, attack_tables, e1, black) &&
					!is_square_attacked(board, attack_tables, d1, black)) {
					printf("Castle move: e1 to c1\n");
				}
			}
		}
	}
	else {
		if (board->castle & bk) {
			if (!get_bit(board->occupancies[both], f8) && !get_bit(board->occupancies[both], g8)) {
				if (!is_square_attacked(board, attack_tables, e8, white) &&
					!is_square_attacked(board, attack_tables, f8, white)) {
					printf("Castle move: e8 to g8\n");
				}
			}
		}
		if (board->castle & bq) {
			if (!get_bit(board->occupancies[both], b8) &&
				!get_bit(board->occupancies[both], c8) &&
				!get_bit(board->occupancies[both], d8)) {
				if (!is_square_attacked(board, attack_tables, e8, white) &&
					!is_square_attacked(board, attack_tables, d8, white)) {
					printf("Castle move: e8 to c8\n");
				}
			}
		}
	}
}

void generate_moves(Board* board, AttackTables* attack_tables) {
	// Handle pawns
	handle_pawn_moves(board, attack_tables, board->side);

	// Handle pieces
	if (board->side == white) {
		handle_piece_moves(board, attack_tables, N, "Knight");
		handle_piece_moves(board, attack_tables, B, "Bishop");
		handle_piece_moves(board, attack_tables, R, "Rook");
		handle_piece_moves(board, attack_tables, Q, "Queen");
		handle_piece_moves(board, attack_tables, K, "King");
	}
	else {
		handle_piece_moves(board, attack_tables, n, "Knight");
		handle_piece_moves(board, attack_tables, b, "Bishop");
		handle_piece_moves(board, attack_tables, r, "Rook");
		handle_piece_moves(board, attack_tables, q, "Queen");
		handle_piece_moves(board, attack_tables, k, "King");
	}

	// Handle castling
	handle_castle_moves(board, attack_tables, board->side);
}
