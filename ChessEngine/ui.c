#include "ui.h"
#include "movegen.h"
#include "globals.h"
int parse_move(Board* board, AttackTables* attacks, char* move_str) {
	Moves move_list[1];

	generate_moves(board, attacks, move_list);

	int source_square = move_str[0] - 'a' + (8 - (move_str[1] - '0')) * 8;
	int target_square = move_str[2] - 'a' + (8 - (move_str[3] - '0')) * 8;
	printf("Source %s Target %s", square_to_coords[source_square], square_to_coords[target_square]);
	for (int count = 0; count < move_list->count; count++) {
		int move = move_list->moves[count];

		if (source_square == get_move_source(move) && target_square == get_move_target(move)) {
			int promoted_piece = get_move_promoted(move);

			
			if (promoted_piece) {
				printf("%d\n", promoted_piece);
				if ((promoted_piece == Q || promoted_piece == q) && move_str[4] == 'q') {
					return move;
				}
				else if ((promoted_piece == R || promoted_piece == r) && move_str[4] == 'r') {
					return move;
				}
				else if ((promoted_piece == B || promoted_piece == b) && move_str[4] == 'b') {
					return move;
				}
				else if ((promoted_piece == N || promoted_piece == n) && move_str[4] == 'n') {
					return move;
				}
				continue;
			}

			// legal move
			return move;
		}
	}

	// illegal move
	return 0;
}
