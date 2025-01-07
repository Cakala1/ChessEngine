#ifndef PERFT_H
#define PERFT_H
#include "globals.h"
#include "board.h"
#include "attacks.h"

static inline void perft_driver(Board* board, AttackTables* attack_tables, int depth);
void perft_test(Board* board, AttackTables* attack_tables, int depth);

#endif 