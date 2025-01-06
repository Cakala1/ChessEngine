#include "ui.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "movegen.h"
#include "globals.h"
#include "search.h"

int parse_move(Board* board, AttackTables* attacks, char* move_str) {
	Moves move_list[1];

	generate_moves(board, attacks, move_list);

	int source_square = move_str[0] - 'a' + (8 - (move_str[1] - '0')) * 8;
	int target_square = move_str[2] - 'a' + (8 - (move_str[3] - '0')) * 8;
	for (int count = 0; count < move_list->count; count++) {
		int move = move_list->moves[count];

		if (source_square == get_move_source(move) && target_square == get_move_target(move)) {
			int promoted_piece = get_move_promoted(move);

			
			if (promoted_piece) {
				printf("%d\n", promoted_piece);
				if ((promoted_piece == Q || promoted_piece == q) && move_str[4] == 'q') {
					return move;
				}
				else if ((promoted_piece == R || promoted_piece == r) && move_str[4] == 'r') {
					return move;
				}
				else if ((promoted_piece == B || promoted_piece == b) && move_str[4] == 'b') {
					return move;
				}
				else if ((promoted_piece == N || promoted_piece == n) && move_str[4] == 'n') {
					return move;
				}
				continue;
			}

			// legal move
			return move;
		}
	}

	// illegal move
	return 0;
}

void parse_position(Board* board, AttackTables* attacks, char* command) {
	// shift by 9 bits to skip "position" from command 
	command += 9;
	// current character of command
	char* curr = command;

	// parse start position
	if (!strncmp(curr, "startpos", 8)) {
		parse_FEN(board, start_position);
	}
	// parse command
	else {
		// check if "fen" exists in command
		curr = strstr(command, "fen");
		
		if (curr == NULL) {
			parse_FEN(board, start_position);
		}
		else{
			// shift by 4 bits to skip "fen" (and space)
			curr += 4;

			parse_FEN(board, curr);
		}
	}


	// check if "moves" exists in command
	curr = strstr(command, "moves");
	if (curr != NULL) {
		// shift pointer by 6 to skip "moves" (and space)
		curr += 6;
		while (curr != NULL) {
			printf("%s\n", curr);
			int move = parse_move(board, attacks, curr);

			if (!move) break;

			make_move(board, attacks, move, all_moves);

			// move pointer to the end of move
			while (*curr && *curr != ' ') curr++;

			// skip the space
			curr++;
		}
		
	}
	print_board(board);
}


void parse_go(Board* board, AttackTables* attacks, char* command){
	int depth = -1;

	char* curr_depth = NULL;

	if (curr_depth = strstr(command, "depth")) {
		depth = atoi(curr_depth + 6);
	}else{
		depth = 6;
	}

	printf("Depth: %d\n", depth);
	search_position(board, attacks, depth);
}

void uci_main(Board* board, AttackTables* attacks) {
	// reset stdin and stdout
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);

	char input[2000];

	printf("id name RT\nid name Cakala1\nuciok\n");

	while (1) {
		memset(input, 0, sizeof(input));

		// make sure output reaches GUI
		fflush(stdout);

		// get user / GUI input
		if (!fgets(input, 2000, stdin)) continue;

		// check if input is available
		if (input[0] == '\n') continue;

		if (!strncmp(input, "isready", 7)) {
			printf("readyok\n");
			continue;
		}

		else if (!strncmp(input, "position", 8)) {
			parse_position(board, attacks, input); 
		}

		else if (!strncmp(input, "ucinewgame", 10)) {
			parse_position(board, attacks, "position startpos");

		}
		
		else if (!strncmp(input, "go", 2)) {
			parse_go(board, attacks, input);
		}


		else if (!strncmp(input, "quit", 4)) {
			break;
		}

		else if (!strncmp(input, "uci", 3)) {
			printf("id name RT\nid name Cakala1\nuciok\n");
		}
	}
}
