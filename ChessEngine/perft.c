#include "perft.h"
#include "movegen.h"
#include "board.h"
#include "attacks.h"
#include "utils.h"


long nodes = 0;


static inline void perft_driver(Board* board, AttackTables* attack_tables, int depth) {
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

void perft_test(Board* board, AttackTables* attack_tables, int depth){
	printf("Performence Test\n\n");
	// base condition
	if (depth == 0) {
		nodes++;
		return;
	}
	Moves move_list[1];
	generate_moves(board, attack_tables, move_list);
	Board copied;
	copy_board(board, &copied);
	
	long start = get_time_ms();

	for (int i = 0; i < move_list->count; i++) {
		copy_board(board, &copied);
		if (!make_move(board, attack_tables, move_list->moves[i], all_moves)) {
			copy_board(&copied, board);

			continue;
		}
		long cummulative_nodes = nodes;

		perft_driver(board, attack_tables, depth - 1);
		copy_board(&copied, board);

		long old_nodes = nodes - cummulative_nodes;
		printf("move: %s%s%c   nodes: %ld\n",
			square_to_coords[get_move_source(move_list->moves[i])],
			square_to_coords[get_move_target(move_list->moves[i])],
			promoted_pieces[get_move_promoted(move_list->moves[i])],
			old_nodes
		);
	};
	printf("\n\nDepth: %d", depth);
	printf("\nNodes: %ld", nodes);
	printf("\nTime: %ld\n", get_time_ms() - start);
	
}

