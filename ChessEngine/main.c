#include <stdio.h>
#include "board.h"
#include "attacks.h"
#include "movegen.h"

// FEN dedug positions
#define empty_board "8/8/8/8/8/8/8/8 w - - "
#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define killer_position "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define cmk_position "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "


// ================================
// ============= MAIN =============
// ================================


int main() {
	Board* board = create_board();
	AttackTables* tables = init_attack_tables();
	printf("RT Engine\n");
	parse_FEN(board, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ");
	print_board(board);
	Moves* move_list = create_moves();
	int move1 = encode_move(e4, e5, P, N, 0, 0, 0, 0);
	int move2 = encode_move(d5, d6, P, Q, 0, 0, 0, 0);
	add_move(move_list, move1);
	add_move(move_list, move2);
	print_move_list(move_list);
	//generate_moves(board, tables);
	return 0;
}
