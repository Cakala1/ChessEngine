#ifndef SEARCH_H
#define SEARCH_H
#include "board.h"
#include "attacks.h"

void search_position(Board* board, AttackTables* attacks, int depth);

#endif 