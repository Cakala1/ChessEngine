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
	Board* copied = copy_board(board);

	for (int i = 0; i < move_list->count; i++) {
		copied = copy_board(board);
		if (!make_move(board, attack_tables, move_list->moves[i], all_moves)) {
			board = copy_board(copied);
			free(copied);
			continue;
		}
		perft_driver(board, attack_tables, depth - 1);
		
		board = copy_board(copied);
		free(copied);
	}
}
