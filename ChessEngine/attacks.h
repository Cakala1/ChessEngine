#ifndef ATTACKS_H
#define ATTACKS_H
#include "globals.h"
#include "board.h"

typedef struct {
	U64 pawn_attacks[2][64];
	U64 knight_attacks[64];
	U64 king_attacks[64];
	U64 bishop_masks[64];
	U64 bishop_attacks[64][512];
	U64 rook_masks[64];
	U64 rook_attacks[64][4096];
} AttackTables;



U64 generate_pawn_attacks(int side, int square);
U64 generate_knight_attacks(int square);
U64 generate_king_attacks(int square);
U64 generate_bishop_attacks(int square);
U64 block_bishop(int square, U64 block_bb);
U64 generate_rook_attacks(int square);
U64 block_rook(int square, U64 block_bb);
U64 set_occupancy(int idx, int bits_in_mask, U64 attacks);
U64 find_magic_number(int square, int relevant_bits, int is_bishop);
void generate_magic_numbers();
void generate_sliders_attacks(AttackTables* attack_tables, int is_bishop);
inline U64 get_bishop_attacks(AttackTables* attack_tables, int square, U64 occupancy);
inline U64 get_rook_attacks(AttackTables* attack_tables, int square, U64 occupancy);
inline U64 get_queen_attacks(AttackTables* attack_tables, int square, U64 occupancy);
AttackTables* init_attack_tables();
void print_attacked_squares(Board* board, AttackTables* attack_tables, int side);
int is_square_attacked(Board* board, AttackTables* attack_tables, int square, int side);

#endif 