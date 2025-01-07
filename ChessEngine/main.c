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

// ================================
// ============= MAIN =============
// ================================




int main() {
	printf("RT Engine\n");
	Board board;
	reset_board(&board);
	AttackTables* attacks = init_attack_tables();
	
	parse_FEN(&board, "1nbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ");
	print_board(&board);
	printf("score: %d\n", evaluate(&board, attacks));

	//uci_main(&board, attacks);
	return 0;
}
