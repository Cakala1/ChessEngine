#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include "board.h"
#include "attacks.h"
#include "movegen.h"
#include "globals.h"
#include "randoms.h"
#include "utils.h"
#include "perft.h"
#include "ui.h"
#include "evaluate.h"
#include "search.h"

// ================================
// ============= MAIN =============
// ================================




int main() {
	printf("RT Engine\n");
	Board board;
	reset_board(&board);
	AttackTables* attacks = init_attack_tables();

	/*int debug = 0;
	if (debug) {
		parse_FEN(&board, start_position);
		print_board(&board);
		search_position(&board, attacks, 4);
	}
	else uci_main(&board, attacks);*/

	uci_main(&board, attacks);

	return 0;
}
