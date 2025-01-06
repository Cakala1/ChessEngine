#ifndef GLOBALS_H
#define GLOBALS_H

#define U64 unsigned long long

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

// sides to move (colors)
enum { white, black, both };

// flags to detect if it is a bishop
enum { rook, bishop };

// helpers for castling rights using binary  
// wk - White king can castle to king side (0001)
// wq - White king cant castle to queen side (0010)
// bk - Black king can castle to king side (0100)
// bq - Black king cant castle to queen side (1000)
enum { wk = 1, wq = 2, bk = 4, bq = 8 };

// encode pieces 
// big letter - white, small letter - black
// P = white pawn, p = black pawn, etc.
enum { P, N, B, R, Q, K, p, n, b, r, q, k };


extern const char* square_to_coords[];
extern char ascii_pieces[12];
extern int char_pieces[];
extern char promoted_pieces[];

extern const int bishop_relevant_bits[64];

extern const int rook_relevant_bits[64];


// FEN dedug positions
#define empty_board "8/8/8/8/8/8/8/8 w - - "
#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define killer_position "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define cmk_position "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "



// =============================
// ======= MAGIC NUMBERS =======
// =============================

extern const U64 bishop_magic_numbers[64];

extern const U64 rook_magic_numbers[64];

#endif // GLOBALS_H


