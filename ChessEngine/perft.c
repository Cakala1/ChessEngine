#include "perft.h"
#include "movegen.h"
#include "board.h"
#include "attacks.h"


long nodes = 0;


void perft_driver(Board* board, AttackTables* attack_tables, int depth) {
	// base condition
	if (depth == 0) {
		nodes++;
		return;
	}
	Moves move_list[1];
	generate_moves(board, attack_tables, move_list);
	Board copied;
	copy_board(board, &copied);

	for (int i = 0; i < move_list->count; i++) {
		copy_board(board, &copied);
		if (!make_move(board, attack_tables, move_list->moves[i], all_moves)) {
			copy_board(&copied, board);
			
			continue;
		}
		perft_driver(board, attack_tables, depth - 1);

		copy_board(&copied, board);
	
	}
}
