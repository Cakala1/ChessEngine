#ifndef MOVEGEN_H
#define MOVEGEN_H
#include "globals.h"
#include "board.h"
#include "attacks.h"

// ==========================================================
// ===================== MOVE GENERATOR =====================
// ==========================================================
/*
	binary move bits				 flag name			 hexidecimal constants

	0000 0000 0000 0000 0011 1111    source square       0x3f
	0000 0000 0000 1111 1100 0000    target square       0xfc0
	0000 0000 1111 0000 0000 0000    piece               0xf000
	0000 1111 0000 0000 0000 0000    promoted piece      0xf0000
	0001 0000 0000 0000 0000 0000    capture flag        0x100000
	0010 0000 0000 0000 0000 0000    double push flag    0x200000
	0100 0000 0000 0000 0000 0000    enpassant flag      0x400000
	1000 0000 0000 0000 0000 0000    castling flag       0x800000
*/


enum { all_moves, only_captures };


#define encode_move(source, target, piece, promoted, capture, double_push, enpassant, castling) \
	(source) | (target << 6) | (piece << 12) | (promoted << 16) | (capture << 20) | (double_push << 21) | (enpassant << 22) | (castling << 23)\

#define get_move_source(move) (move & 0x3f)
#define get_move_target(move) ((move & 0xfc0) >> 6)
#define get_move_piece(move) ((move & 0xf000) >> 12)
#define get_move_promoted(move) ((move & 0xf0000) >> 16)
#define get_move_capture(move) (move & 0x100000)
#define get_move_double_push(move) (move & 0x200000)
#define get_move_enpassant(move) (move & 0x400000)
#define get_move_castling(move) (move & 0x800000)


typedef struct {
	int moves[256];
	int count;
} Moves;

void add_move(Moves* moves_list, int move);
void print_move(int move);
void print_move_list(Moves* move_list);
void handle_pawn_moves(Board* board, AttackTables* attack_tables, Moves* move_list, int side);
void handle_piece_moves(Board* board, AttackTables* attack_tables, Moves* move_list, int piece, const char* piece_name);
void handle_castle_moves(Board* board, AttackTables* attack_tables, Moves* move_list, int side);
void generate_moves(Board* board, AttackTables* attack_tables, Moves* move_list);
int make_move(Board* board, AttackTables* attack_tables, int move, int move_flag);

#endif 
