#ifndef UI_H
#define UI_H
#define _CRT_SECURE_NO_WARNINGS
#include "board.h"
#include "attacks.h"

// Parse move passed by user (e. g. e2e4)
int parse_move(Board* board, AttackTables* attacks, char* move_str);

// Parse position passed by user
void parse_position(Board* board, AttackTables* attacks, char* command);

// Parse "go" command																	
void parse_go(Board* board, AttackTables* attacks, char* command);


// Main function for UCI commands
// 
// COMMANDS:
// 
//	"position startpos"															- sets start position
//	"position startpos moves <move1_white> <move1_black> <move2_white>"			- sets start position with aditional moves made 
//																					(e. g. "position startpos moves e2e4 e7e5 d2d4")
//	"position fen <your_fen_position>"											- sets given FEN position
//																					(e. g. "position fen rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
//	"position fen <your_fen_position> moves <move1_white> <move1_black>"		- sets given FEN position with aditional moves made  
//																					(e. g. "position fen rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 moves e2e4 e7e5 d2d4")
//	"go depth <number>"															- searches for best move in <number> depth
// 
//  "isready"																	- returns status of engine
//	"ucinewgame"																- starts new game with start position
//	"uci"																		- displays info of engine
//  "quit"																		- quits engine
//

void uci_main(Board* board, AttackTables* attacks);

#endif 