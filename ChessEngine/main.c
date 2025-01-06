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

// ================================
// ============= MAIN =============
// ================================


int main() {
	printf("RT Engine\n");
	Board board;
	reset_board(&board);
	AttackTables* attacks = init_attack_tables();
	//parse_position(&board, attacks, "position fen rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 moves e2e4 e7e5 d2d4");
	////parse_position(&board, attacks, "position startpos moves e2e4 e7e5 d2d4");
	//parse_go(&board, attacks, "go depth 6");
	//print_board(&board);

	uci_main(&board, attacks);
	return 0;
}
