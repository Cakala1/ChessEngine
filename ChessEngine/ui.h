#ifndef UI_H
#define UI_H
#include "board.h"
#include "attacks.h"

// Parse move passed by user (e. g. e2e4)
int parse_move(Board* board, AttackTables* attacks, char* move_str);

// Parse position passed by user
//	"position startpos"																- sets start position
//	"position startpos moves <move1_white> <move1_black> <move2_white>"				- sets start position with aditional moves made 
//																					(e. g. "position startpos moves e2e4 e7e5 d2d4")
//	"position fen <your_fen_position>"												- sets given FEN position
//																					(e. g. "position fen rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
//	"position fen <your_fen_position> moves <move1_white> <move1_black>"			- sets given FEN position with aditional moves made  
//																					(e. g. "position fen rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 moves e2e4 e7e5 d2d4")
// 
void parse_position(Board* board, AttackTables* attacks, char* command);


#endif 