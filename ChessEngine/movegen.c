#include <stdio.h>
#include <stdlib.h>
#include "movegen.h"
#include "randoms.h"
#include "utils.h"
#include "globals.h"

void add_move(Moves* moves_list, int move) {
	moves_list->moves[moves_list->count++] = move;
}

void print_move(int move) {
	printf("%s%s%c\n",
		square_to_coords[get_move_source(move)],
		square_to_coords[get_move_target(move)],
		promoted_pieces[get_move_promoted(move)]);
}

void print_move_list(Moves* move_list) {
	if (!move_list->count) {
		printf("\nNo moves in the list\n");
		return;
	}
	printf("\nmove     piece     capture    double_push    enpassant    castling\n");
	int _count = 0;
	for (; _count < move_list->count; _count++) {
		int move = move_list->moves[_count];
		printf("%s%s%c    %c         %d          %d              %d             %d\n",
			square_to_coords[get_move_source(move)],
			square_to_coords[get_move_target(move)],
			get_move_promoted(move) ? promoted_pieces[get_move_promoted(move)] : ' ',
			ascii_pieces[get_move_piece(move)],
			get_move_capture(move) ? 1 : 0,
			get_move_double_push(move) ? 1 : 0,
			get_move_enpassant(move) ? 1 : 0,
			get_move_castling(move) ? 1 : 0
		);
	}
	printf("\n\nTotal number of moves: %d\n\n", _count);
}





void handle_piece_moves(Board* board, AttackTables* attack_tables, Moves* move_list, int piece, const char* piece_name) {
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
			is_capture = is_capture ? 1 : 0;
			add_move(move_list, encode_move(source_square, target_square, piece, 0, is_capture, 0, 0, 0));
			clear_bit(attacks, target_square);
		}
		clear_bit(current_bb, source_square);
	}
}

// Helper function for pawn moves
void handle_pawn_moves(Board* board, AttackTables* attack_tables, Moves* move_list, int side) {
	U64 current_bb = board->bitboards[side == white ? P : p];
	while (current_bb) {
		int source_square = get_ls1b_index(current_bb);
		int target_square = side == white ? source_square - 8 : source_square + 8;
		int piece = side == white ? P : p;
		// Single push
		if (!(target_square < a8 || target_square > h1) && !get_bit(board->occupancies[both], target_square)) {
			if ((side == white && source_square >= a7 && source_square <= h7) || 
				(side == black && source_square >= a2 && source_square <= h2)) 
			{
				add_move(move_list, encode_move(source_square, target_square, piece, Q, 0, 0, 0, 0));
				add_move(move_list, encode_move(source_square, target_square, piece, R, 0, 0, 0, 0));
				add_move(move_list, encode_move(source_square, target_square, piece, N, 0, 0, 0, 0));
				add_move(move_list, encode_move(source_square, target_square, piece, B, 0, 0, 0, 0));
			}
			else {
				add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

				// Double push
				int double_push = side == white ? target_square - 8 : target_square + 8;
				if (((side == white && source_square >= a2 && source_square <= h2) ||
					(side == black && source_square >= a7 && source_square <= h7)) &&
					!get_bit(board->occupancies[both], double_push)) {
					add_move(move_list, encode_move(source_square, double_push, piece, 0, 0, 1, 0, 0));
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
				add_move(move_list, encode_move(source_square, target_square, piece, Q, 1, 0, 0, 0));
				add_move(move_list, encode_move(source_square, target_square, piece, R, 1, 0, 0, 0));
				add_move(move_list, encode_move(source_square, target_square, piece, N, 1, 0, 0, 0));
				add_move(move_list, encode_move(source_square, target_square, piece, B, 1, 0, 0, 0));
			}
			else {
				add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

			}
			clear_bit(attacks, target_square);
		}

		// En passant
		if (board->enpassant != no_sq) {
			U64 enpassant_attacks = attack_tables->pawn_attacks[side][source_square] & (1ULL << board->enpassant);
			if (enpassant_attacks) {
				int target_enpassant = get_ls1b_index(enpassant_attacks);
				printf("Enpassant capture: %s to %s\n",
					square_to_coords[source_square],
					square_to_coords[target_enpassant]);
				add_move(move_list, encode_move(source_square, target_enpassant, piece, 0, 0, 0, 1, 0));
			}
		}

		clear_bit(current_bb, source_square);
	}
}

// Helper function for castle moves
void handle_castle_moves(Board* board, AttackTables* attack_tables, Moves* move_list, int side) {
	if (side == white) {
		if (board->castle & wk) {
			if (!get_bit(board->occupancies[both], f1) && !get_bit(board->occupancies[both], g1)) {
				if (!is_square_attacked(board, attack_tables, e1, black) &&
					!is_square_attacked(board, attack_tables, f1, black)) {
					printf("Castle move: e1 to g1\n");
					add_move(move_list, encode_move(e1, g1, K, 0, 0, 0, 0, 1));
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
					add_move(move_list, encode_move(e1, c1, K, 0, 0, 0, 0, 1));
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
					add_move(move_list, encode_move(e8, g8, k, 0, 0, 0, 0, 1));
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
					add_move(move_list, encode_move(e8, c8, k, 0, 0, 0, 0, 1));
				}
			}
		}
	}
}

void generate_moves(Board* board, AttackTables* attack_tables, Moves* move_list) {
	move_list->count = 0;
	// Handle pawns
	handle_pawn_moves(board, attack_tables, move_list, board->side);


	// Handle pieces
	if (board->side == white) {
		handle_piece_moves(board, attack_tables, move_list, N, "Knight");
		handle_piece_moves(board, attack_tables, move_list, B, "Bishop");
		handle_piece_moves(board, attack_tables, move_list, R, "Rook");
		handle_piece_moves(board, attack_tables, move_list, Q, "Queen");
		handle_piece_moves(board, attack_tables, move_list, K, "King");
	}
	else {
		handle_piece_moves(board, attack_tables, move_list, n, "Knight");
		handle_piece_moves(board, attack_tables, move_list, b, "Bishop");
		handle_piece_moves(board, attack_tables, move_list, r, "Rook");
		handle_piece_moves(board, attack_tables, move_list, q, "Queen");
		handle_piece_moves(board, attack_tables, move_list, k, "King");
	}

	// Handle castling
	handle_castle_moves(board, attack_tables, move_list, board->side);
}

int make_move(Board* board, AttackTables* attack_tables, int move, int move_flag) {
	if (move_flag == all_moves) {
		Board* copied = copy_board(board);

		int source_square = get_move_source(move);
		int target_square = get_move_target(move);
		int piece = get_move_piece(move);
		int promoted = get_move_promoted(move);
		int capture = get_move_capture(move) ? 1 : 0;
		int double_push = get_move_double_push(move);
		int enpassant = get_move_enpassant(move);
		int castling = get_move_castling(move);
		print_move(move);
		// moving piece from source to target
		clear_bit(board->bitboards[piece], source_square);
		set_bit(board->bitboards[piece], target_square);

		if (get_move_capture(move)) {
			// range for our loop
			int start_piece, end_piece;

			// white to move
			if (board->side == white)
			{
				start_piece = p;
				end_piece = k;
			}

			// black to move
			else
			{
				start_piece = P;
				end_piece = K;
			}

			for (int _piece = start_piece; _piece <= end_piece; _piece++) {
				if (get_bit(board->bitboards[_piece], target_square)) {
					clear_bit(board->bitboards[_piece], target_square);
					break;
				}
			}
		}
	}
	else {
		// make sure its a capture
		if (get_move_capture(move)) {
			make_move(board, attack_tables, move, all_moves);
		}
		return 0;
	}
	return 0;
}
