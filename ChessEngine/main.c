#include <stdio.h>
#include <stdlib.h>

// bitboard data type
#define U64 unsigned long long

// =======================
// ==== BOARD SQUARES ====
// =======================

enum {
	a8, b8, c8, d8, e8, f8, g8, h8,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a1, b1, c1, d1, e1, f1, g1, h1, 
};

const char* square_to_coords[] = {
	"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
	"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
	"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
	"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
	"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
	"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
	"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
	"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};




// =====================
// ==== BIT HELPERS ====
// =====================

#define set_bit(bb, square) (bb |= (1ULL << square))
#define get_bit(bb, square) (bb & (1ULL << square))
#define clear_bit(bb, square) (get_bit(bb, square) ? (bb ^= (1ULL << square)) : 0)

// count bits in bitboard 
static inline int count_bits(U64 bb) {
	int bit_count = 0;
	while (bb) {
		bit_count++;
		bb &= bb - 1;
	}
	return bit_count;
}

// get index of the least significant first bit
static inline int get_ls1b_index(U64 bb) {
	if (!bb) return -1;
	return count_bits(((bb & (~bb + 1)) - 1));
}


// sides to move (colors)
enum {white, black};

void print_bitboard(U64 bb) {
	printf("\n\n");
	// loop over ranks of bitboard
	for (int rank = 0; rank < 8; rank++) {
		//loop over board files
		for (int file = 0; file < 8; file++) {
			int square = rank * 8 + file;
			// print ranks
			if (file == 0) printf(" %d ", 8 - rank);
			// print bit at square (1 or 0)
			printf(" %c", get_bit(bb, square) ? '1' : '.');
		}

		printf("\n");
	}
	//print files
	printf("\n    a b c d e f g h\n\n");

	// print bitboard as 64 bit unsigned integer
	printf("Bitboard value: %llu\n", bb);
}



// ============================
// ====== RANDOM NUMBERS ======
// ============================

//unsigned int state = 1017233273;
unsigned int state = 1804289383;

// getting random numbers by XOR shift algorithm

// U32
unsigned int get_random_U32_number() {
	unsigned int num = state;
	num ^= num << 13;
	num ^= num >> 17;
	num ^= num << 5;
	state = num;

	return num;
}

// U64
U64 get_random_U64_number() {
	U64 n1, n2, n3, n4;
	// getting random num (U32) and slicing 16 bits from most significant bit side
	n1 = ((U64)(get_random_U32_number() & 0xFFFFF));
	n2 = ((U64)(get_random_U32_number() & 0xFFFFF));
	n3 = ((U64)(get_random_U32_number() & 0xFFFFF));
	n4 = ((U64)(get_random_U32_number() & 0xFFFFF));

	return (n1 | (n2 << 16) | (n3 << 32) | (n4 << 48));
}

U64 generate_magic_number() {
	return (get_random_U64_number() & get_random_U64_number() & get_random_U64_number());
}




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

// relevant occupancy bit count for every square on board
const int bishop_relevant_bits[64] = {
	6, 5, 5, 5, 5, 5, 5, 6,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 7, 7, 7, 7, 5, 5,
	5, 5, 7, 9, 9, 7, 5, 5,
	5, 5, 7, 9, 9, 7, 5, 5,
	5, 5, 7, 7, 7, 7, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	6, 5, 5, 5, 5, 5, 5, 6,
};

const int rook_relevant_bits[64] = {
	12, 11, 11, 11, 11, 11, 11, 12,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	12, 11, 11, 11, 11, 11, 11, 12,
};


// PAWN ATTACKS [side][square]
U64 pawn_attacks[2][64];

// KNIGHT ATTACKS (same for black and white)
U64 knight_attacks[64];

// KING ATTACKS (same for black and white)
U64 king_attacks[64];


// ===================================================================
// ==== FUNCTIONS TO HELP WITH GENERATING ATTACKS FOR ALL FIGURES ====
// ===================================================================


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

U64 block_bishop (int square, U64 block_bb) {
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



// ====================================================
// ==== Generating available attacks for all spots ====
// ====================================================

void generate_attacks() {
	// loop over board squares and generate a move
	for (int square = 0; square < 64; square++) {
		// generate for both black and white pawns
		pawn_attacks[white][square] = generate_pawn_attacks(white, square);
		pawn_attacks[black][square] = generate_pawn_attacks(black, square);

		// generate for knights
		knight_attacks[square] = generate_knight_attacks(square);

		// generate for king
		king_attacks[square] = generate_king_attacks(square);
	}
}


// set occupancies

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

int main() {
	printf("RT Engine\n");
	generate_attacks();
	print_bitboard(generate_magic_number());
	return 0;
}
