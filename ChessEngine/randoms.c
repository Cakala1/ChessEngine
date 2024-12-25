#include "randoms.h"

unsigned int random_state = 1804289383;

// getting random numbers by XOR shift algorithm

// U32
unsigned int get_random_U32_number() {
	unsigned int num = random_state;
	num ^= num << 13;
	num ^= num >> 17;
	num ^= num << 5;
	random_state = num;

	return num;
}

// U64
U64 get_random_U64_number() {
	U64 n1, n2, n3, n4;

	// init random numbers slicing 16 bits from MS1B side
	n1 = (U64)(get_random_U32_number()) & 0xFFFF;
	n2 = (U64)(get_random_U32_number()) & 0xFFFF;
	n3 = (U64)(get_random_U32_number()) & 0xFFFF;
	n4 = (U64)(get_random_U32_number()) & 0xFFFF;

	return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

U64 generate_magic_number() {
	return (get_random_U64_number() & get_random_U64_number() & get_random_U64_number());
}