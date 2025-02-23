# Chess Engine

## Project Description
This is a developing chess engine written in **C**, supporting the **UCI (Universal Chess Interface) protocol**. The project is still in progress, and its functionality will be gradually expanded. It is created for **educational purposes**, to learn programming and improve algorithmic thinking.

## Features
- Supports the **starting position** and positions defined using **FEN**.
- Allows adding moves to a given position.
- Searches for the best moves at a specified depth.
- Basic **UCI commands** to interact with chess GUIs.

## UCI Commands
The engine supports the following UCI commands:

### Setting the Position
- `position startpos`  
  Sets the starting position.

- `position startpos moves <move1_white> <move1_black> <move2_white>`  
  Sets the starting position with additional moves. 
  **Example:** `position startpos moves e2e4 e7e5 d2d4`

- `position fen <your_fen_position>`  
  Sets a given **FEN position**.
  **Example:** `position fen rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1`

- `position fen <your_fen_position> moves <move1_white> <move1_black>`  
  Sets a given **FEN position** with additional moves.
  **Example:** `position fen rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 moves e2e4 e7e5 d2d4`

### Searching for the Best Move
- `go depth <number>`  
  Searches for the best move up to the specified depth.
  **Example:** `go depth 5`

### Other UCI Commands
- `isready`  
  Checks the engine's status, returns "readyok" if ready.

- `ucinewgame`  
  Starts a new game with the starting position.

- `uci`  
  Displays information about the engine.

- `quit`  
  Exits the engine.

## How to Use
1. Clone the repository and compile the code in **C**.
2. Run the engine in a terminal or through a UCI-compatible **GUI** (e.g., **Arena**, **Cute Chess**).
3. Send UCI commands to interact with the engine.

## Planned Features
- Optimization of the move search algorithm.
- Implementation of advanced position evaluation heuristics.
- Integration with popular chess interfaces.
- Ability to analyze games and suggest moves.

## Project Information
This project is created **for educational purposes**. Any suggestions and contributions are welcome!

---
Initial version of the documentation - Work in progress 🚀
