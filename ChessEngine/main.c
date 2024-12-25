#include <stdio.h>
#include <string.h>
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
	printf("RT Engine\n");
	Board* board = create_board();
	AttackTables* tables = init_attack_tables();

	parse_FEN(board, tricky_position);
	print_board(board);
	Moves move_list[1];
	Board* copied = copy_board(board);
	generate_moves(board, tables, move_list);
	for (int i = 0; i < move_list->count; i++) {
		int move = move_list->moves[i];
		copied = copy_board(board);
		make_move(board, tables, move, all_moves);
		print_board(board);
		board = copy_board(copied);
		getchar();
	}
	
	free(board);
	free(tables);
	return 0;
}
