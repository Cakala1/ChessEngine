#ifndef UI_H
#define UI_H
#include "board.h"
#include "attacks.h"

// Parse move passed by user (e. g. e2e4)
int parse_move(Board* board, AttackTables* attacks, char* move_str);

#endif 