#include "utils.h"

count_bits(U64 bb) {
	int bit_count = 0;
	while (bb) {
		bit_count++;
		bb &= bb - 1;
	}
	return bit_count;
}

get_ls1b_index(U64 bb) {
	if (!bb) return -1;
	return count_bits(((bb & (~bb + 1)) - 1));
}

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