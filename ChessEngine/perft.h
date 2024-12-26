#ifndef PERFT_H
#define PERFT_H
#include "globals.h"
#include "board.h"
#include "attacks.h"

long nodes;
void perft_driver(Board* board, AttackTables* attack_tables, int depth);

#endif 