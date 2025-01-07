#ifndef EVALUATE_H
#define EVALUATE_H
#include "board.h"
#include "attacks.h"

// evaluates a score of current board state
int evaluate(Board* board, AttackTables* attacks);

#endif 