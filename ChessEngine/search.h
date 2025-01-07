#ifndef SEARCH_H
#define SEARCH_H
#include "board.h"
#include "attacks.h"

static int negamax(Board* board, AttackTables* attacks, int alpha, int beta, int depth);
void search_position(Board* board, AttackTables* attacks, int depth);

#endif 