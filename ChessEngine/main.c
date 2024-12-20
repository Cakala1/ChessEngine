#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	no_sq
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

#define set_bit(bb, square) ((bb) |= (1ULL << (square)))
#define get_bit(bb, square) ((bb) & (1ULL << (square)))
#define clear_bit(bb, square) ((bb) &= ~(1ULL << (square)))

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
enum {white, black, both};

// flags to detect if it is a bishop
enum {rook, bishop};

// helpers for castling rights using binary  
// wk - White king can castle to king side (0001)
// wq - White king cant castle to queen side (0010)
// bk - Black king can castle to king side (0100)
// bq - Black king cant castle to queen side (1000)
enum {wk = 1, wq = 2, bk =  4, bq = 8};

// encode pieces 
// big letter - white, small letter - black
// P = white pawn, p = black pawn, etc.
enum {P, N, B, R, Q, K, p, n, b, r, q, k};

char ascii_pieces[12] = "PNBRQKpnbrqk";

// convert ASCII to encoded constants
int char_pieces[] = {
	['P'] = P,
	['N'] = N,
	['B'] = B,
	['R'] = R,
	['Q'] = Q,
	['K'] = K,
	['p'] = p,
	['n'] = n,
	['b'] = b,
	['r'] = r,
	['q'] = q,
	['k'] = k,
};




// bitboards for all types of pieces (white pawn, black pawn, white knight, black knight, etc...)
U64 bitboards[12];

U64 occupancies[3];
int side = white;
int enpassant = no_sq;
int castle = 0;



// ==========================================
// ================= OUTPUT =================
// ==========================================

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

void print_board() {
	printf("\n\n");
	for (int rank = 0; rank < 8; rank++) {
		printf(" %d", 8 - rank);
		for (int file = 0; file < 8; file++) {
			int square = rank * 8 + file;
			int piece = -1;

			// checking all bitboards of specific pieces (starting from white pawn and ending at black king) and assinging correct piece
			for (int bb_idx = P; bb_idx <= k; bb_idx++) {
				if (get_bit(bitboards[bb_idx], square)) piece = bb_idx;
			}
			

			printf(" %c", (piece == -1) ? '.' : ascii_pieces[piece]);
		}
		printf("\n");
	}
	printf("\n   a b c d e f g h\n\n");
	printf("Enpassant: %s\n", (enpassant != no_sq) ? square_to_coords[enpassant] : "none");
	printf("Side: %s\n", !side ? "white" : "black");
	printf("Castling: %c%c%c%c\n\n",
		(castle & wk) ? 'K' : '-',
		(castle & wq) ? 'Q' : '-',
		(castle & bk) ? 'k' : '-',
		(castle & bq) ? 'q' : '-'
	);
}


// ============================
// ====== RANDOM NUMBERS ======
// ============================

//unsigned int state = 1017233273;
unsigned int random_state = 1804289383;

// getting random numbers by XOR shift algorithm

// U32
unsigned int get_random_U32_number() {
	unsigned int num =  random_state;
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

// =============================
// ======= MAGIC NUMBERS =======
// =============================

const U64 bishop_magic_numbers[64] = {
	0x40040822862081ULL,
	0x40810a4108000ULL,
	0x2008008400920040ULL,
	0x61050104000008ULL,
	0x8282021010016100ULL,
	0x41008210400a0001ULL,
	0x3004202104050c0ULL,
	0x22010108410402ULL,
	0x60400862888605ULL,
	0x6311401040228ULL,
	0x80801082000ULL,
	0x802a082080240100ULL,
	0x1860061210016800ULL,
	0x401016010a810ULL,
	0x1000060545201005ULL,
	0x21000c2098280819ULL,
	0x2020004242020200ULL,
	0x4102100490040101ULL,
	0x114012208001500ULL,
	0x108000682004460ULL,
	0x7809000490401000ULL,
	0x420b001601052912ULL,
	0x408c8206100300ULL,
	0x2231001041180110ULL,
	0x8010102008a02100ULL,
	0x204201004080084ULL,
	0x410500058008811ULL,
	0x480a040008010820ULL,
	0x2194082044002002ULL,
	0x2008a20001004200ULL,
	0x40908041041004ULL,
	0x881002200540404ULL,
	0x4001082002082101ULL,
	0x8110408880880ULL,
	0x8000404040080200ULL,
	0x200020082180080ULL,
	0x1184440400114100ULL,
	0xc220008020110412ULL,
	0x4088084040090100ULL,
	0x8822104100121080ULL,
	0x100111884008200aULL,
	0x2844040288820200ULL,
	0x90901088003010ULL,
	0x1000a218000400ULL,
	0x1102010420204ULL,
	0x8414a3483000200ULL,
	0x6410849901420400ULL,
	0x201080200901040ULL,
	0x204880808050002ULL,
	0x1001008201210000ULL,
	0x16a6300a890040aULL,
	0x8049000441108600ULL,
	0x2212002060410044ULL,
	0x100086308020020ULL,
	0x484241408020421ULL,
	0x105084028429c085ULL,
	0x4282480801080cULL,
	0x81c098488088240ULL,
	0x1400000090480820ULL,
	0x4444000030208810ULL,
	0x1020142010820200ULL,
	0x2234802004018200ULL,
	0xc2040450820a00ULL,
	0x2101021090020ULL,
};

const U64 rook_magic_numbers[64] = {
	0x8a80104000800020ULL,
	0x140002000100040ULL,
	0x2801880a0017001ULL,
	0x100081001000420ULL,
	0x200020010080420ULL,
	0x3001c0002010008ULL,
	0x8480008002000100ULL,
	0x2080088004402900ULL,
	0x800098204000ULL,
	0x2024401000200040ULL,
	0x100802000801000ULL,
	0x120800800801000ULL,
	0x208808088000400ULL,
	0x2802200800400ULL,
	0x2200800100020080ULL,
	0x801000060821100ULL,
	0x80044006422000ULL,
	0x100808020004000ULL,
	0x12108a0010204200ULL,
	0x140848010000802ULL,
	0x481828014002800ULL,
	0x8094004002004100ULL,
	0x4010040010010802ULL,
	0x20008806104ULL,
	0x100400080208000ULL,
	0x2040002120081000ULL,
	0x21200680100081ULL,
	0x20100080080080ULL,
	0x2000a00200410ULL,
	0x20080800400ULL,
	0x80088400100102ULL,
	0x80004600042881ULL,
	0x4040008040800020ULL,
	0x440003000200801ULL,
	0x4200011004500ULL,
	0x188020010100100ULL,
	0x14800401802800ULL,
	0x2080040080800200ULL,
	0x124080204001001ULL,
	0x200046502000484ULL,
	0x480400080088020ULL,
	0x1000422010034000ULL,
	0x30200100110040ULL,
	0x100021010009ULL,
	0x2002080100110004ULL,
	0x202008004008002ULL,
	0x20020004010100ULL,
	0x2048440040820001ULL,
	0x101002200408200ULL,
	0x40802000401080ULL,
	0x4008142004410100ULL,
	0x2060820c0120200ULL,
	0x1001004080100ULL,
	0x20c020080040080ULL,
	0x2935610830022400ULL,
	0x44440041009200ULL,
	0x280001040802101ULL,
	0x2100190040002085ULL,
	0x80c0084100102001ULL,
	0x4024081001000421ULL,
	0x20030a0244872ULL,
	0x12001008414402ULL,
	0x2006104900a0804ULL,
	0x1004081002402ULL,
};


// =============================
// ========== ATTACKS ==========
// =============================

// PAWN ATTACKS [side][square]
U64 pawn_attacks[2][64];

// KNIGHT ATTACKS (same for black and white)
U64 knight_attacks[64];

// KING ATTACKS (same for black and white)
U64 king_attacks[64];

// BISHOP ATTACKS
U64 bishop_masks[64];
// [square][occupancy]	
U64 bishop_attacks[64][512];

// ROOK ATTACKS
U64 rook_masks[64];
// [square][occupancy]	
U64 rook_attacks[64][4096];

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

// ======================================
// ============ MAGIC NUMBERS ===========
// ======================================

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

 void generate_sliders_attacks(int is_bishop) {
	 for (int square = 0; square < 64; square++) {
		 bishop_masks[square] = generate_bishop_attacks(square);
		 rook_masks[square] = generate_rook_attacks(square);

		 U64 attack_mask = is_bishop ? bishop_masks[square] : rook_masks[square];
		 int relevant_bits = count_bits(attack_mask);
		 int occupancy_indicies = 1 << relevant_bits;

		 for (int i = 0; i < occupancy_indicies; i++) {
			 U64 occupancy = set_occupancy(i, relevant_bits, attack_mask);
			 if (is_bishop) {
				 int magic = (occupancy * bishop_magic_numbers[square]) >> (64 - bishop_relevant_bits[square]);
				 bishop_attacks[square][magic] = block_bishop(square, occupancy);
			 }
			 else {
				 int magic = (occupancy * rook_magic_numbers[square]) >> (64 - rook_relevant_bits[square]);
				 rook_attacks[square][magic] = block_rook(square, occupancy);
			 }
		 }
	 }
 }

 
 static inline U64 get_bishop_attacks(int square, U64 occupancy) {
	 occupancy &= bishop_masks[square];
	 occupancy *= bishop_magic_numbers[square];
	 occupancy >>= 64 - bishop_relevant_bits[square];

	 return bishop_attacks[square][occupancy];
 }

 static inline U64 get_rook_attacks(int square, U64 occupancy) {
	 occupancy &= rook_masks[square];
	 occupancy *= rook_magic_numbers[square];
	 occupancy >>= 64 - rook_relevant_bits[square];
	 return rook_attacks[square][occupancy];
 }


// ================================
// ============= Init =============
// ================================

void init_all() {
	 generate_attacks();
	 //generate_magic_numbers();
	 generate_sliders_attacks(bishop);
	 generate_sliders_attacks(rook);
}


// ================================
// ============= MAIN =============
// ================================


int main() {
	printf("RT Engine\n");
	init_all();
	set_bit(bitboards[P], a2);
	set_bit(bitboards[P], b2);
	set_bit(bitboards[P], c2);
	set_bit(bitboards[P], d2);
	set_bit(bitboards[P], e2);
	set_bit(bitboards[P], f2);
	set_bit(bitboards[P], g2);
	set_bit(bitboards[P], h2);
	set_bit(bitboards[N], b1);
	set_bit(bitboards[N], g1);
	set_bit(bitboards[B], c1);
	set_bit(bitboards[B], f1);
	set_bit(bitboards[R], a1);
	set_bit(bitboards[R], h1);
	set_bit(bitboards[Q], d1);
	set_bit(bitboards[K], e1);

	set_bit(bitboards[p], a7);
	set_bit(bitboards[p], b7);
	set_bit(bitboards[p], c7);
	set_bit(bitboards[p], d7);
	set_bit(bitboards[p], e7);
	set_bit(bitboards[p], f7);
	set_bit(bitboards[p], g7);
	set_bit(bitboards[p], h7);
	set_bit(bitboards[n], b8);
	set_bit(bitboards[n], g8);
	set_bit(bitboards[b], c8);
	set_bit(bitboards[b], f8);
	set_bit(bitboards[r], a8);
	set_bit(bitboards[r], h8);
	set_bit(bitboards[q], d8);
	set_bit(bitboards[k], e8);

	enpassant = e3;
	castle = 15;

	print_board();

	for (int piece = 0; piece <= k; piece++) {
		print_bitboard(bitboards[piece]);
	}
	return 0;
}
