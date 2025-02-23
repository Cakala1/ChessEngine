#include "search.h"
#include "evaluate.h"
#include "globals.h"
#include "movegen.h"


static int best_move;
static int ply;

static int negamax(Board* board, AttackTables* attacks, int alpha, int beta, int depth) {
    if (depth == 0) {
        return evaluate(board, attacks);
    }

    nodes++;

    int best_sofar = best_move;
    int old_alpha = alpha;

    Moves move_list[1];
    generate_moves(board, attacks, move_list);


    for (int i = 0; i < move_list->count; i++) {
        Board copy;
        copy_board(board, &copy);
        ply++;

        // illegal move
        if (!make_move(board, attacks, move_list->moves[i], all_moves)) {
            ply--;
            continue;
        }

        int score = -negamax(board, attacks, -beta, -alpha, depth - 1);

        copy_board(&copy, board);
        ply--;

        // fail-hard beta cutoff
        if (score >= beta) {
            // move fails high
            return beta;
        }

        // found better move
        if (score > alpha) {
            alpha = score;
            if (ply == 0) {
                best_sofar = move_list->moves[i];
            }
        }
    }

    if (old_alpha != alpha) {
        best_move = best_sofar;
    }

    // move fails low
    return alpha;
}

void search_position(Board* board, AttackTables* attacks, int depth) {
    int score = negamax(board, attacks, -50000, 50000, depth);
	printf("bestmove ");
    print_move(best_move);
    printf("\n");
   // printf("bestmove d2d4\n");
}
