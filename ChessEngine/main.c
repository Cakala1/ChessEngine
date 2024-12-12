#include <stdio.h>

// bitboard data type
#define U64 unsigned long long

// enum board squares
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

//bit macros  
#define set_bit(bb, square) (bb |= (1ULL << square))
#define get_bit(bb, square) (bb & (1ULL << square))
#define clear_bit(bb, square) (get_bit(bb, square) ? (bb ^= (1ULL << square)) : 0);


// sides to move (colors)
enum {white, black};

// print bitboard to console

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

//
// Pregenerated bitboards to help with offboard attacks
//

// files that are not "a" files (range b - h)
const U64 not_a_file = 18374403900871474942ULL;

// files that are not "h" files (range a - g)
const U64 not_h_file = 9187201950435737471ULL;

// files that are not "h" or "g" (range a - f)
const U64 not_hg_file = 4557430888798830399ULL;

// files that are not "a" or "b" (range c - h)
const U64 not_ab_file = 18229723555195321596ULL;

// PAWN ATTACKS [side][square]
U64 pawn_attacks[2][64];

// KNIGHT ATTACKS (same for black and white)
U64 knight_attacks[64];


// generate pawn attacks
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


// generate knight attacks 

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

//
// Generating aviable attacks for all spots
//

void generate_attacks() {
	// loop over board squares and generate a move
	for (int square = 0; square < 64; square++) {
		// generate for both black and white pawns
		pawn_attacks[white][square] = generate_pawn_attacks(white, square);
		pawn_attacks[black][square] = generate_pawn_attacks(black, square);

		// generate for knights
		knight_attacks[square] = generate_knight_attacks(square);
	}
}



int main() {
	printf("RT Engine\n");
	// generate all possible moves
	generate_attacks();
	for (int square = 0; square < 64; square++) {
		printf("\n%d\n", square);
		print_bitboard(knight_attacks[square]);
	}



	return 0;
}
