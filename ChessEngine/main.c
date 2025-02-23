#include <stdio.h>
#include "board.h"
#include "attacks.h"
#include "movegen.h"
#include "ui.h"


// ================================
// ============= MAIN =============
// ================================




int main() {
	printf("RT Engine\n");
	Board board;
	reset_board(&board);
	AttackTables* attacks = init_attack_tables();
	uci_main(&board, attacks);

	return 0;
}
