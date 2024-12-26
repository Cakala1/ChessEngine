#include <stdio.h>
#include <stdlib.h>
#include "attacks.h"
#include "utils.h"



// ============================================================
// === Pregenerated bitboards to help with offboard attacks ===
// ============================================================

// files that are not "a" files (range b - h)
const U64 not_a_file = 18374403900871474942ULL;

// files that are not "h" files (range a - g)
const U64 not_h_file = 9187201950435737471ULL;

// files that are not "h" or "g" (range a - f)
const U64 not_hg_file = 4557430888798830399ULL;

// files that are not "a" or "b" (range c - h)
const U64 not_ab_file = 18229723555195321596ULL;

//	=======================================
//  ================ PAWN =================
//	=======================================

U64 generate_pawn_attacks(int side, int square) {
	U64 attacks = 0ULL;
	U64 bitboard = 0ULL;

	// setting piece on our board
	set_bit(bitboard, square);
	if (!side) {
		// for white
		attacks |= ((bitboard >> 7) & not_a_file);
		attacks |= ((bitboard >> 9) & not_h_file);
	}
	else {
		// for black
		attacks |= ((bitboard << 7) & not_h_file);
		attacks |= ((bitboard << 9) & not_a_file);
	}


	return attacks;
}

//	========================================
//  ================ KNIGHT ================
//	========================================

U64 generate_knight_attacks(int square) {
	U64 attacks = 0ULL;
	U64 bitboard = 0ULL;

	// setting piece on our board
	set_bit(bitboard, square);
	attacks |= ((bitboard >> 17) & not_h_file);
	attacks |= ((bitboard << 17) & not_a_file);
	attacks |= ((bitboard << 15) & not_h_file);
	attacks |= ((bitboard >> 15) & not_a_file);
	attacks |= ((bitboard >> 10) & not_hg_file);
	attacks |= ((bitboard << 10) & not_ab_file);
	attacks |= ((bitboard >> 6) & not_ab_file);
	attacks |= ((bitboard << 6) & not_hg_file);


	return attacks;
}


//	======================================
//  ================ KING ================
//	======================================

U64 generate_king_attacks(int square) {
	U64 attacks = 0ULL;
	U64 bitboard = 0ULL;

	// setting piece on our board
	set_bit(bitboard, square);
	attacks |= (bitboard << 8);
	attacks |= (bitboard >> 8);
	attacks |= ((bitboard << 9) & not_a_file);
	attacks |= ((bitboard >> 9) & not_h_file);
	attacks |= ((bitboard << 7) & not_h_file);
	attacks |= ((bitboard >> 7) & not_a_file);
	attacks |= ((bitboard << 1) & not_a_file);
	attacks |= ((bitboard >> 1) & not_h_file);


	return attacks;
}

//	========================================
//  ================ BISHOP ================
//	========================================


U64 generate_bishop_attacks(int square) {
	U64 attacks = 0ULL;
	int rank, file;
	int target_rank = square / 8;
	int target_file = square % 8;

	for (rank = target_rank + 1, file = target_file + 1; rank <= 6 && file <= 6; rank++, file++) attacks |= (1ULL << (8 * rank + file));
	for (rank = target_rank - 1, file = target_file - 1; rank >= 1 && file >= 1; rank--, file--) attacks |= (1ULL << (8 * rank + file));
	for (rank = target_rank - 1, file = target_file + 1; rank >= 1 && file <= 6; rank--, file++) attacks |= (1ULL << (8 * rank + file));
	for (rank = target_rank + 1, file = target_file - 1; rank <= 6 && file >= 1; rank++, file--) attacks |= (1ULL << (8 * rank + file));

	return attacks;
}


// blocks bishop vision with the "block_bb" given bitboard

U64 block_bishop(int square, U64 block_bb) {
	U64 attacks = 0ULL;
	int rank, file;
	int target_rank = square / 8;
	int target_file = square % 8;


	for (rank = target_rank + 1, file = target_file + 1; rank <= 7 && file <= 7; rank++, file++) {
		attacks |= (1ULL << (8 * rank + file));
		if ((1ULL << (8 * rank + file)) & block_bb) break;
	}
	for (rank = target_rank - 1, file = target_file - 1; rank >= 0 && file >= 0; rank--, file--) {
		attacks |= (1ULL << (8 * rank + file));
		if ((1ULL << (8 * rank + file)) & block_bb) break;
	}
	for (rank = target_rank - 1, file = target_file + 1; rank >= 0 && file <= 7; rank--, file++) {
		attacks |= (1ULL << (8 * rank + file));
		if ((1ULL << (8 * rank + file)) & block_bb) break;
	}
	for (rank = target_rank + 1, file = target_file - 1; rank <= 7 && file >= 0; rank++, file--) {
		attacks |= (1ULL << (8 * rank + file));
		if ((1ULL << (8 * rank + file)) & block_bb) break;
	}

	return attacks;
}


//	=======================================
//  ================ ROOK =================
//	=======================================


U64 generate_rook_attacks(int square) {
	U64 attacks = 0ULL;
	int rank, file;
	int target_rank = square / 8;
	int target_file = square % 8;


	for (rank = target_rank + 1; rank <= 6; rank++) attacks |= (1ULL << (8 * rank + target_file));
	for (rank = target_rank - 1; rank >= 1; rank--) attacks |= (1ULL << (8 * rank + target_file));
	for (file = target_file - 1; file >= 1; file--) attacks |= (1ULL << (8 * target_rank + file));
	for (file = target_file + 1; file <= 6; file++) attacks |= (1ULL << (8 * target_rank + file));

	return attacks;
}

// blocks rook vision with the "block_bb" given bitboard

U64 block_rook(int square, U64 block_bb) {
	U64 attacks = 0ULL;
	int rank, file;
	int target_rank = square / 8;
	int target_file = square % 8;


	for (rank = target_rank + 1; rank <= 7; rank++) {
		attacks |= (1ULL << (8 * rank + target_file));
		if ((1ULL << (8 * rank + target_file)) & block_bb) break;
	}
	for (rank = target_rank - 1; rank >= 0; rank--) {
		attacks |= (1ULL << (8 * rank + target_file));
		if ((1ULL << (8 * rank + target_file)) & block_bb) break;
	}
	for (file = target_file - 1; file >= 0; file--) {
		attacks |= (1ULL << (8 * target_rank + file));
		if ((1ULL << (8 * target_rank + file)) & block_bb) break;
	}
	for (file = target_file + 1; file <= 7; file++) {
		attacks |= (1ULL << (8 * target_rank + file));
		if ((1ULL << (8 * target_rank + file)) & block_bb) break;
	}

	return attacks;
}


// =========================================================
// ======================= OCCUPANCY  ======================
// =========================================================


U64 set_occupancy(int idx, int bits_in_mask, U64 attacks) {
	U64 occupancy = 0ULL;

	for (int i = 0; i < bits_in_mask; i++) {
		int square = get_ls1b_index(attacks);
		clear_bit(attacks, square);
		if (idx & (1 << i)) {
			occupancy |= (1ULL << square);
		}
	}

	return occupancy;
}

U64 find_magic_number(int square, int relevant_bits, int is_bishop) {
	U64 occupancies[4096];
	U64 attacks[4096];
	U64 used_attacks[4096];
	U64 attack_mask = is_bishop ? generate_bishop_attacks(square) : generate_rook_attacks(square);


	int occupancy_indicies = 1 << relevant_bits;

	for (int index = 0; index < occupancy_indicies; index++)
	{
		occupancies[index] = set_occupancy(index, relevant_bits, attack_mask);

		attacks[index] = is_bishop ? block_bishop(square, occupancies[index]) :
			block_rook(square, occupancies[index]);
	}

	// test magic numbers loop
	for (int random_count = 0; random_count < 100000000; random_count++)
	{
		U64 magic_number = generate_magic_number();
		if (count_bits((attack_mask * magic_number) & 0xFF00000000000000) < 6) continue;
		memset(used_attacks, 0ULL, sizeof(used_attacks));
		int index, fail;
		for (index = 0, fail = 0; !fail && index < occupancy_indicies; index++)
		{
			int magic_index = (int)((occupancies[index] * magic_number) >> (64 - relevant_bits));

			if (used_attacks[magic_index] == 0ULL)
				used_attacks[magic_index] = attacks[index];

			else if (used_attacks[magic_index] != attacks[index])
				fail = 1;
		}

		if (!fail)
			return magic_number;
	}

	printf("  Magic number fails!\n");
	return 0ULL;
}



// for testing magic numbers
void generate_magic_numbers() {
	for (int square = 0; square < 64; square++) {
		//printf("0x%llxULL,\n", find_magic_number(square, rook_relevant_bits[square], rook));
		//rook_magic_numbers[square] = find_magic_number(square, rook_relevant_bits[square], rook);
	}

	printf("\n\n\n\n");
	for (int square = 0; square < 64; square++) {
		//printf("0x%llxULL,\n", find_magic_number(square, bishop_relevant_bits[square], bishop));
		//bishop_magic_numbers[square] = find_magic_number(square, bishop_relevant_bits[square], bishop);
	}

}

void generate_sliders_attacks(AttackTables* attack_tables, int is_bishop) {
	for (int square = 0; square < 64; square++) {
		attack_tables->bishop_masks[square] = generate_bishop_attacks(square);
		attack_tables->rook_masks[square] = generate_rook_attacks(square);

		U64 attack_mask = is_bishop ? attack_tables->bishop_masks[square] : attack_tables->rook_masks[square];
		int relevant_bits = count_bits(attack_mask);
		int occupancy_indicies = 1 << relevant_bits;

		for (int i = 0; i < occupancy_indicies; i++) {
			U64 occupancy = set_occupancy(i, relevant_bits, attack_mask);
			if (is_bishop) {
				int magic = (occupancy * bishop_magic_numbers[square]) >> (64 - bishop_relevant_bits[square]);
				attack_tables->bishop_attacks[square][magic] = block_bishop(square, occupancy);
			}
			else {
				int magic = (occupancy * rook_magic_numbers[square]) >> (64 - rook_relevant_bits[square]);
				attack_tables->rook_attacks[square][magic] = block_rook(square, occupancy);
			}
		}
	}
}


inline U64 get_bishop_attacks(AttackTables* attack_tables, int square, U64 occupancy) {
	occupancy &= attack_tables->bishop_masks[square];
	occupancy *= bishop_magic_numbers[square];
	occupancy >>= 64 - bishop_relevant_bits[square];

	return attack_tables->bishop_attacks[square][occupancy];
}

inline U64 get_rook_attacks(AttackTables* attack_tables, int square, U64 occupancy) {
	occupancy &= attack_tables->rook_masks[square];
	occupancy *= rook_magic_numbers[square];
	occupancy >>= 64 - rook_relevant_bits[square];
	return attack_tables->rook_attacks[square][occupancy];
}

inline U64 get_queen_attacks(AttackTables* attack_tables, int square, U64 occupancy) {
	return (get_bishop_attacks(attack_tables, square, occupancy) | get_rook_attacks(attack_tables, square, occupancy));
}


AttackTables* init_attack_tables() {
	AttackTables* tables = (AttackTables*)malloc(sizeof(AttackTables));
	if (!tables) {
		fprintf(stderr, "Failed to allocate memory for attack tables\n");
		exit(1);
	}

	for (int square = 0; square < 64; square++) {
		tables->pawn_attacks[white][square] = generate_pawn_attacks(white, square);
		tables->pawn_attacks[black][square] = generate_pawn_attacks(black, square);
		tables->knight_attacks[square] = generate_knight_attacks(square);
		tables->king_attacks[square] = generate_king_attacks(square);
	}

	generate_sliders_attacks(tables, bishop);
	generate_sliders_attacks(tables, rook);

	return tables;
}

void print_attacked_squares(Board* board, AttackTables* attack_tables, int side) {
	printf("\n");
	for (int r = 0; r < 8; r++) {
		printf(" %d ", 8 - r);
		for (int f = 0; f < 8; f++) {
			int square = 8 * r + f;
			printf(" %c", is_square_attacked(board, attack_tables, square, side) ? '1' : '.');
		}
		printf("\n");
	}

	printf("\n    a b c d e f g h\n\n");
}

int is_square_attacked(Board* board, AttackTables* attack_tables, int square, int side) {
	// attacked by white pawns
	if ((side == white) && (attack_tables->pawn_attacks[black][square] & board->bitboards[P])) return 1;

	// attacked by black pawns
	if ((side == black) && (attack_tables->pawn_attacks[white][square] & board->bitboards[p])) return 1;

	// attacked by knights
	if (attack_tables->knight_attacks[square] & ((side == white) ? board->bitboards[N] : board->bitboards[n])) return 1;

	// attacked by bishops
	if (get_bishop_attacks(attack_tables, square, board->occupancies[both]) & ((side == white) ? board->bitboards[B] : board->bitboards[b])) return 1;

	// attacked by rooks
	if (get_rook_attacks(attack_tables, square, board->occupancies[both]) & ((side == white) ? board->bitboards[R] : board->bitboards[r])) return 1;

	// attacked by bishops
	if (get_queen_attacks(attack_tables, square, board->occupancies[both]) & ((side == white) ? board->bitboards[Q] : board->bitboards[q])) return 1;

	// attacked by kings
	if (attack_tables->king_attacks[square] & ((side == white) ? board->bitboards[K] : board->bitboards[k])) return 1;

	// by default return false
	return 0;
	

	return 0;
}