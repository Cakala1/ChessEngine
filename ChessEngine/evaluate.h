#ifndef EVALUATE_H
#define EVALUATE_H
#include "board.h"
#include "attacks.h"

int material_score[12];

// evaluates a score of current board state
int evaluate(Board* board, AttackTables* attacks);

#endif 