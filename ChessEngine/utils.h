#ifndef UTILS_H
#define UTILS_H
#include "globals.h"

#define set_bit(bb, square) ((bb) |= (1ULL << (square)))
#define get_bit(bb, square) ((bb) & (1ULL << (square)))
#define clear_bit(bb, square) ((bb) &= ~(1ULL << (square)))

// count bits in bitboard 
int count_bits(U64 bb);

// get index of the least significant first bit
int get_ls1b_index(U64 bb);

void print_bitboard(U64 bb);

int get_time_ms();

#endif 