#ifndef IMPLEMENTATION_H
#define IMPLEMENTATION_H
#include "BoardGame_Classes.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <limits>
#include <set>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;


//#######################################################################################################################
//                                                   GAME 1

template <typename T>
class Pyramid_XO : public Board<T> {
public:
    // Constructor to initialize the board
    Pyramid_XO() {
        this->rows = 3;
        this->columns = 5;
        this->board = new T*[this->rows];
        for (int i = 0; i < this->rows; ++i) {
            this->board[i] = new T[this->columns];
            for (int j = 0; j < this->columns; ++j) {
                this->board[i][j] = " "; // Initialize all cells with a space
            }
        }

        // Set specific cells with their coordinates
        this->board[0][2] = "(0,2)";
        this->board[1][1] = "(1,1)";
        this->board[1][2] = "(1,2)";
        this->board[1][3] = "(1,3)";
        this->board[2][0] = "(2,0)";
        this->board[2][1] = "(2,1)";
        this->board[2][2] = "(2,2)";
        this->board[2][3] = "(2,3)";
        this->board[2][4] = "(2,4)";
    }

    // Destructor to clean up the board
    ~Pyramid_XO() {
        for (int i = 0; i < this->rows; ++i) {
            delete[] this->board[i];
        }
        delete[] this->board;
    }

    // Reset the board to its initial state
    void reset_board() {
        for (int i = 0; i < this->rows; ++i) {
            for (int j = 0; j < this->columns; ++j) {
                this->board[i][j] = " "; // Reset all cells to space
            }
        }

        // Reset specific cells with their coordinates
        this->board[0][2] = "(0,2)";
        this->board[1][1] = "(1,1)";
        this->board[1][2] = "(1,2)";
        this->board[1][3] = "(1,3)";
        this->board[2][0] = "(2,0)";
        this->board[2][1] = "(2,1)";
        this->board[2][2] = "(2,2)";
        this->board[2][3] = "(2,3)";
        this->board[2][4] = "(2,4)";
        this->n_moves = 0; // Reset move count
    }

    // Update the board with the player's move
    bool update_board(int x, int y, T symbol) override {
        if (x < 0 || x >= this->rows || y < 0 || y >= this->columns || this->board[x][y] == " " || this->board[x][y] != "(" + to_string(x) + "," + to_string(y) + ")") {
            // Check if the move is valid
            cout << "Invalid input" << endl;
            return false;
        } else {
            this->board[x][y] = symbol; // Place the symbol on the board
            this->n_moves++; // Increment move count
            return true;
        }
    }

    // Display the current state of the board
    void display_board() override {
        cout << endl << endl;
        cout << "       0       1       2       3      4\n";
        cout << "  -------------------------------------------\n";

        cout << "0 ";
        cout << "                    " << setw(5) <<  this->board[0][2] << " ";
        cout << "\n                   -----------\n";

        cout << "1             ";
        cout << setw(5) <<  this->board[1][1] << " | ";
        cout << setw(5) <<  this->board[1][2] << " | ";
        cout << setw(5) <<  this->board[1][3] << "\n";
        cout << "             -----------------------\n";

        cout << "2     ";
        cout << setw(5) << left << this->board[2][0] << " | ";
        cout << setw(5) << left << this->board[2][1] << " | ";
        cout << setw(5) << left << this->board[2][2] << " | ";
        cout << setw(5) << left << this->board[2][3] << " | ";
        cout << setw(5) << left << this->board[2][4] << "\n\n";
    }

    // Check if there is a winning condition
    bool is_win() override {
        // Check rows
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->columns - 2; j++) {
                // Check for three consecutive symbols in a row
                if (this->board[i][j] != " " && this->board[i][j] != "(" + to_string(i) + "," + to_string(j) + ")" && this->board[i][j] == this->board[i][j + 1] && this->board[i][j] == this->board[i][j + 2]) {
                    return true;
                }
            }
        }

        // Check columns
        for (int j = 0; j < this->columns; j++) {
            for (int i = 0; i < this->rows - 2; i++) {
                // Check for three consecutive symbols in a column
                if (this->board[i][j] != " " && this->board[i][j] != "(" + to_string(i) + "," + to_string(j) + ")" && this->board[i][j] == this->board[i + 1][j] && this->board[i][j] == this->board[i + 2][j]) {
                    return true;
                }
            }
        }

        // Check diagonals
        // Check for three consecutive symbols in the main diagonal
        if (this->board[0][2] != "(" + to_string(0) + "," + to_string(2) + ")" &&
            ((this->board[0][2] == this->board[1][1] && this->board[0][2] == this->board[2][0]) ||
             (this->board[0][2] == this->board[1][3] && this->board[0][2] == this->board[2][4]))) {
            return true;
        }

        // Check for three consecutive symbols in the anti-diagonal
        if (this->board[2][2] != "(" + to_string(2) + "," + to_string(2) + ")" &&
            ((this->board[2][2] == this->board[1][1] && this->board[2][2] == this->board[0][0]) ||
             (this->board[2][2] == this->board[1][3] && this->board[2][2] == this->board[0][4]))) {
            return true;
        }

        return false; // No win condition met
    }

    // Check if the game is a draw
    bool is_draw() override {
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->columns; j++) {
                // If there is any cell with coordinates and space, it's not a draw
                if (this->board[i][j] == "(" + to_string(i) + "," + to_string(j) + ")" && this->board[i][j] != " ") {
                    return false;
                }
            }
        }
        return true; // All cells are filled
    }

    // Check if the game is over (win or draw)
    bool game_is_over() override {
        return this->is_win() || this->is_draw();
    }
};

// Set to keep track of used moves
set<pair<int, int>> usedMoves_game1;

// Class for human player
class current_player_game1 : public Player<string> {
private:
    Pyramid_XO<string>* boardPtr2;
public:
    current_player_game1(string n, char s, Pyramid_XO<string>* b) : Player<string>(n, string(1, s)), boardPtr2(b) {}

    // Get move from human player
    void getmove(int &x, int &y) override {
        while (true) {
            cout << getname() << "'s turn. Enter your move (row and column): ";
            if (cin >> x >> y) {
                if (x >= 0 && x <= 2 && y >= 0 && y <= 4 && boardPtr2->board[x][y] != " " && usedMoves_game1.find({x, y}) == usedMoves_game1.end()) {
                    usedMoves_game1.insert({x, y}); // Insert the move into the set of used moves
                    break;
                } else {
                    cout << "Invalid input. Please enter valid integers within the range and ensure the cell is not occupied." << endl;
                }
            } else {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter valid integers." << endl;
            }
        }
    }
};

// Class for random player
class random_player_game1 : public Player<string> {
private:
    Pyramid_XO<string>* boardPtr2;
public:
    random_player_game1(string n, char s, Pyramid_XO<string>* b) : Player<string>(n, string(1, s)), boardPtr2(b) {}

    // Get move from random player
    void getmove(int &x, int &y) override {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        do {
            x = rand() % 3; // Random row
            y = rand() % 5; // Random column
        } while (boardPtr2->board[x][y] == " " || usedMoves_game1.find({x, y}) != usedMoves_game1.end());
        usedMoves_game1.insert({x, y}); // Insert the move into the set of used moves
        cout << "Player '" << getsymbol() << "' (random) chose: " << x << "," << y << endl;
    }
};


void menu_game_1() {
    cout << "Welcome to our pyramid tic tac toe game!" << endl;

    Pyramid_XO<string> board; // Initialize the game board

    Player<string>* player1;
    Player<string>* player2;

    usedMoves_game1.clear(); // Clear any used moves from previous games
    board.reset_board(); // Reset the board to its initial state

    // Setup Player 1
    while (true) {
        string player1Type;
        cout << "Is Player 1 (symbol 'X') a human or a random player? " << endl;
        cout << "1. Human player" << endl;
        cout << "2. Random player" << endl;
        cout << "Please enter a choice 1 or 2: ";
        cin >> player1Type;
        if (player1Type == "1") {
            player1 = new current_player_game1("Human 1 (X) ", 'X', &board);
            break;
        } else if (player1Type == "2") {
            player1 = new random_player_game1("Computer Random 1", 'X', &board);
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    // Setup Player 2
    while (true) {
        string player2Type;
        cout << "Is Player 2 (symbol 'O') a human or a random player? " << endl;
        cout << "1. Human player" << endl;
        cout << "2. Random player" << endl;
        cout << "Please enter a choice 1 or 2: ";
        cin >> player2Type;
        if (player2Type == "1") {
            player2 = new current_player_game1("Human 2 (O) ", 'O', &board);
            break;
        } else if (player2Type == "2") {
            player2 = new random_player_game1("Computer Random 2", 'O', &board);
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    // Assign the board to players
    player1->setBoard(&board);
    player2->setBoard(&board);

    Player<string>* players[2] = {player1, player2}; // Array of players

    // Initialize the game manager and start the game
    GameManager<string> gameManager(&board, players);
    gameManager.run();

    // Clean up dynamically allocated memory
    delete player1;
    delete player2;
}


//#######################################################################################################################
//                                           GAME 2

class ConnectFourBoard : public Board<char> {
public:
    // Constructor to initialize the board with given rows and columns
    ConnectFourBoard(int rows, int columns) {
        this->rows = rows;
        this->columns = columns;
        this->board = new char*[rows];
        for (int i = 0; i < rows; ++i) {
            this->board[i] = new char[columns];
            fill(this->board[i], this->board[i] + columns, ' '); // Fill the board with spaces
        }
    }

    // Destructor to delete the board
    ~ConnectFourBoard() {
        for (int i = 0; i < rows; ++i) {
            delete[] board[i];
        }
        delete[] board;
    }

    // Reset the board to its initial state
    void reset() {
        for (int i = 0; i < rows; ++i) {
            fill(board[i], board[i] + columns, ' '); // Reset all cells to spaces
        }
        n_moves = 0; // Reset move count
    }

    // Get the number of columns of the board
    int getBoardColumns() const {
        return this->columns;
    }

    // Check if a column is full
    bool is_column_full(int column) const {
        return this->board[0][column] != ' '; // Column is full if top cell is not empty
    }

    // Update the board with the player's move
    bool update_board(int x, int y, char symbol) override {
        if (y < 0 || y >= this->columns) return false; // Check if column is valid
        for (int i = this->rows - 1; i >= 0; --i) {
            if (this->board[i][y] == ' ') {
                this->board[i][y] = symbol; // Place the symbol in the lowest empty cell
                this->n_moves++; // Increment move count
                return true;
            }
        }
        return false; // Column is full
    }

    // Display the current state of the board
    void display_board() override {
        cout << "\nCurrent Board:\n";
        for (int i = 0; i < this->rows; ++i) {
            for (int j = 0; j < this->columns; ++j) {
                cout << "| " << this->board[i][j] << " ";
            }
            cout << "|\n";
        }
        for (int j = 0; j < this->columns; ++j) {
            cout << "----";
        }
        cout << "-\n";
        for (int j = 0; j < this->columns; ++j) {
            cout << "  " << j << " ";
        }
        cout << "\n";
    }

    // Check if there is a winning condition
    bool is_win() override {
        for (int i = 0; i < this->rows; ++i) {
            for (int j = 0; j < this->columns; ++j) {
                char symbol = this->board[i][j];
                if (symbol == ' ') continue;

                // Check horizontal line
                if (j + 3 < this->columns &&
                    this->board[i][j + 1] == symbol  &&
                    this->board[i][j + 2] == symbol &&
                    this->board[i][j + 3] == symbol)
                    return true;

                // Check vertical line
                if (i + 3 < this->rows &&
                    this->board[i + 1][j] == symbol &&
                    this->board[i + 2][j] == symbol &&
                    this->board[i + 3][j] == symbol)
                    return true;

                // Check diagonal (bottom-left to top-right)
                if (i + 3 < this->rows && j + 3 < this->columns &&
                    this->board[i + 1][j + 1] == symbol &&
                    this->board[i + 2][j + 2] == symbol &&
                    this->board[i + 3][j + 3] == symbol)
                    return true;

                // Check diagonal (top-left to bottom-right)
                if (i + 3 < this->rows && j - 3 >= 0 &&
                    this->board[i + 1][j - 1] == symbol &&
                    this->board[i + 2][j - 2] == symbol &&
                    this->board[i + 3][j - 3] == symbol)
                    return true;
            }
        }
        return false; // No win condition met
    }

    // Check if the game is a draw
    bool is_draw() override {
        return this->n_moves == this->rows * this->columns && !is_win(); // Draw if all cells are filled and no win
    }

    // Check if the game is over (win or draw)
    bool game_is_over() override {
        return is_win() || is_draw();
    }
};

class HumanPlayer_game2 : public Player<char> {
public:
    HumanPlayer_game2(string name, char symbol) : Player<char>(name, symbol) {}

    // Get move from human player
    void getmove(int& x, int& y) override {
        auto* connectFourBoard = dynamic_cast<ConnectFourBoard*>(this->boardPtr); // Cast the board pointer to ConnectFourBoard
        int columns = connectFourBoard->getBoardColumns(); // Get the number of columns
        while (true) {
            cout << this->getname()  << ", enter your move (column 0-" << columns - 1 << "): ";
            cin >> y;
            if (y < 0 || y >= columns) {
                cout << "Invalid column. Please choose a column between 0 and " << columns - 1 << ".\n";
                continue;
            }
            if (connectFourBoard->is_column_full(y)) {
                cout << "Column " << y << " is full. Please choose a different column.\n";
                continue;
            }
            x = 0; // Row is not used in Connect Four; set to 0
            break;
        }
    }
};


class randomPlayer_game2 : public Player<char> {
public:
    randomPlayer_game2(string name, char symbol)
            : Player<char>(name, symbol) {
        srand(static_cast<unsigned int>(time(0))); // Seed the random number generator
    }

    // Get move from random player
    void getmove(int& x, int& y) override {
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Add a delay
        auto* connectFourBoard = dynamic_cast<ConnectFourBoard*>(this->boardPtr); // Cast the board pointer to ConnectFourBoard
        int columns = connectFourBoard->getBoardColumns(); // Get the number of columns
        while (true) {
            x = 0; // Row is not used in Connect Four; set to 0
            y = rand() % columns; // Random column
            if (connectFourBoard->is_column_full(y)) {
                continue; // If the column is full, try again
            }

            cout << name << " placed at column " << y << "\n";
            break;
        }
    }
};

void menu_game_2() {
    ConnectFourBoard board(6, 7); // Create a Connect Four board with 6 rows and 7 columns
    Player<char>* players[2];

    // Setup Player 1
    while (true) {
        string player1Type;
        cout << "Is Player 1 (symbol 'X') a human or a random player?  " << endl;
        cout << "1. Human player " << endl;
        cout << "2. Random player" << endl;
        cout << "Please enter a choice 1 or 2: " << endl;
        cin >> player1Type;
        if (player1Type == "1") {
            players[0] = new HumanPlayer_game2("Human 1 (X) ", 'X');
            break;
        } else if (player1Type == "2") {
            players[0] = new randomPlayer_game2("Computer Random 1 (X) ", 'X');
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    // Setup Player 2
    while (true) {
        string player2Type;
        cout << "Is Player 2 (symbol 'O') a human or a random player?  " << endl;
        cout << "1. Human player " << endl;
        cout << "2. Random player" << endl;
        cout << "Please enter a choice 1 or 2: " << endl;
        cin >> player2Type;
        if (player2Type == "1") {
            players[1] = new HumanPlayer_game2("Human 2 (O) ", 'O');
            break;
        } else if (player2Type == "2") {
            players[1] = new randomPlayer_game2("Computer Random 2 (O) ", 'O');
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    // Assign the board to players
    players[0]->setBoard(&board);
    players[1]->setBoard(&board);

    // Initialize the game manager and start the game
    GameManager<char> game(&board, players);
    game.run();

    // Clean up dynamically allocated memory
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }

    // Reset the board for a new game
    board.reset();
}

// #######################################################################################################################################################################################################333
//                                          GAME 3

class _5x5_board : public Board<string> {
public:
    int total_moves; // Track total moves made in the game

    // Constructor to initialize the 5x5 board
    _5x5_board() {
        this->rows = 5;
        this->columns = 5;
        total_moves = 0;

        board = new string*[rows];
        for (int i = 0; i < rows; ++i) {
            board[i] = new string[columns];
            for (int j = 0; j < columns; ++j) {
                board[i][j] = "(" + to_string(i) + "," + to_string(j) + ")"; // Initialize each cell with its coordinates
            }
        }
    }

    // Destructor to delete the board
    ~_5x5_board() {
        for (int i = 0; i < rows; ++i) {
            delete[] board[i];
        }
        delete[] board;
    }

    // Update the board with the player's move
    bool update_board(int x, int y, string symbol) override {
        if (total_moves == 25) { // unreal move
            return true;
        }
        string cell_content = "(" + to_string(x) + "," + to_string(y) + ")";
        if (x >= 0 && x < rows && y >= 0 && y < columns && board[x][y] == cell_content) {
            board[x][y] = symbol; // Place the symbol on the board
            total_moves++; // Increment move count
            return true;
        }
        return false; // Invalid move
    }

    // Display the current state of the board
    void display_board() override {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                cout << setw(6) << board[i][j];
                if (j < columns - 1) {
                    cout << "|";
                }
            }
            cout << endl;
            if (i < rows - 1) {
                cout << string(columns * 7 - 1, '-') << endl;
            }
        }
        cout << endl;
    }

    // Check if there is a winning condition
    bool is_win() override {
        if (total_moves == 24) {
            return count_sequences("O") > count_sequences("X");
        }
        if (total_moves == 25) {
            return count_sequences("X") > count_sequences("O");
        }
        return false;
    }

    // Check if the game is a draw
    bool is_draw() override {
        return (total_moves == 24 && count_sequences("X") == count_sequences("O"));
    }

    // Check if the game is over (win or draw)
    bool game_is_over() override {
        return is_draw() || is_win();
    }

    // Count sequences of three consecutive symbols
    int count_sequences(string symbol) {
        int counter = 0;

        // Check rows for sequences
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col <= columns - 3; col++) {
                if (board[row][col] == symbol && board[row][col + 1] == symbol && board[row][col + 2] == symbol) {
                    counter++;
                }
            }
        }

        // Check columns for sequences
        for (int col = 0; col < columns; col++) {
            for (int row = 0; row <= rows - 3; row++) {
                if (board[row][col] == symbol && board[row + 1][col] == symbol && board[row + 2][col] == symbol) {
                    counter++;
                }
            }
        }

        // Check main diagonals for sequences
        for (int row = 0; row <= rows - 3; row++) {
            for (int col = 0; col <= columns - 3; col++) {
                if (board[row][col] == symbol && board[row + 1][col + 1] == symbol && board[row + 2][col + 2] == symbol) {
                    counter++;
                }
            }
        }

        // Check anti-diagonals for sequences
        for (int row = 0; row <= rows - 3; row++) {
            for (int col = 2; col < columns; col++) {
                if (board[row][col] == symbol && board[row + 1][col - 1] == symbol && board[row + 2][col - 2] == symbol) {
                    counter++;
                }
            }
        }
        return counter;
    }
};


set<pair<int, int>> usedMoves_game3;

// Class for human player in 5x5 tic-tac-toe game
class current_player_game3 : public Player<string> {
private:
    _5x5_board* boardPtr2; // Pointer to the game board
public:
    current_player_game3(string n, string s, _5x5_board* b) : Player(n, s), boardPtr2(b) {}

    // Get move from human player
    void getmove(int &x, int &y) override {
        if (boardPtr2->total_moves == 24) { // If 24 moves have been made, set the move to (0,0)
            x = 0;
            y = 0;
            boardPtr2->total_moves++;
            return;
        }
        while (true) {
            cout << getname() << "'s turn. Enter your move (row and column): ";
            if (cin >> x >> y) {
                if (x >= 0 && x < 5 && y >= 0 && y < 5) {
                    break; // Valid move entered
                } else {
                    cout << "Invalid input. Please enter valid integers within the range 0-4." << endl;
                }
            } else {
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
                cout << "Invalid input. Please enter valid integers within the range 0-4." << endl;
            }
        }
        usedMoves_game3.insert({x, y}); // Insert the move into the set of used moves
    }
};

// Class for random player in 5x5 tic-tac-toe game
class random_player_game3 : public Player<string> {
private:
    _5x5_board* boardPtr2; // Pointer to the game board
public:
    random_player_game3(string n, string s, _5x5_board* b) : Player(n, s), boardPtr2(b) {}

    // Get move from random player
    void getmove(int &x, int &y) override {
        if (boardPtr2->total_moves == 24) { // If 24 moves have been made, set the move to (0,0)
            x = 0;
            y = 0;
            boardPtr2->total_moves++;
            return;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Add a delay

        do {
            x = rand() % 5; // Random row
            y = rand() % 5; // Random column
        } while (usedMoves_game3.find({x, y}) != usedMoves_game3.end()); // Check if the move has already been made
        usedMoves_game3.insert({x, y}); // Insert the move into the set of used moves
        cout << "Player \'" << getsymbol() << "\' (random) chose: " << x << "," << y << endl << endl;
        cout << "----------------------------------" << endl;
    }
};

void menu_game_3() {
    cout << "Welcome to our 5x5 tic tac toe game!" << endl;

    usedMoves_game3.clear(); // Clear any used moves from previous games

    srand(time(0)); // Seed the random number generator
    _5x5_board* board = new _5x5_board(); // Initialize the game board

    Player<string>* player1;
    Player<string>* player2;

    // Setup Player 1
    while (true) {
        string player1Type;
        cout << "Is Player 1 (symbol 'X') a human or a random player?  " << endl;
        cout << "1. Human player " << endl;
        cout << "2. Random player" << endl;
        cout << "Please enter a choice 1 or 2 : " << endl;
        cin >> player1Type;
        if (player1Type == "1") {
            player1 = new current_player_game3("Human 1 (X) ", "X", board);
            break;
        } else if (player1Type == "2") {
            player1 = new random_player_game3("Computer Random 1 (X) ", "X", board);
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    // Setup Player 2
    while (true) {
        string player2Type;
        cout << "Is Player 2 (symbol 'O') a human or a random player?  " << endl;
        cout << "1. Human player " << endl;
        cout << "2. Random player" << endl;
        cout << "Please enter a choice 1 or 2 : " << endl;
        cin >> player2Type;
        if (player2Type == "1") {
            player2 = new current_player_game3("Human 2 (O) ", "O", board);
            break;
        } else if (player2Type == "2") {
            player2 = new random_player_game3("Computer Random 2 (O) ", "O", board);
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    Player<string>* players[2] = {player1, player2}; // Array of players

    // Initialize the game manager and start the game
    GameManager<string> gameManager(board, players);
    gameManager.run();

    // Display the results
    if (board->count_sequences("X") > board->count_sequences("O")) {
        cout << "Total : " << board->count_sequences("X") << " sequences of (X), and " << board->count_sequences("O") << " sequences of (O). ";
    }
    if (board->count_sequences("O") > board->count_sequences("X")) {
        cout << "Total : " << board->count_sequences("O") << " sequences of (O), and " << board->count_sequences("X") << " sequences of (X). ";
    }
    if (board->count_sequences("X") == board->count_sequences("O")) {
        cout << "It is a draw with sequences equal " << board->count_sequences("X") << " for each player!" << endl;
    }

    // Clean up dynamically allocated memory
    delete board;
    delete player1;
    delete player2;
}


//###################################################################################################################################################################################################################################
//                                                 GAME 5

// Class for Numeric Tic Tac Toe game board
class NumTicTacToeBoard : public Board<int> {
public:
    NumTicTacToeBoard() {
        rows = 3; // Set number of rows
        columns = 3; // Set number of columns
        board = new int*[rows]; // Allocate memory for the board
        for (int i = 0; i < rows; i++) {
            board[i] = new int[columns](); // Initialize the board with zeros
        }
        n_moves = 0; // Initialize move count
    }

    // Reset the board to its initial state
    void reset_board() {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                board[i][j] = 0; // Reset all cells to zero
            }
        }
        n_moves = 0; // Reset move count
    }

    // Update the board with the player's move
    bool update_board(int x, int y, int symbol) override {
        if (x >= 0 && x < rows && y >= 0 && y < columns && board[x][y] == 0) {
            board[x][y] = symbol; // Place the symbol on the board
            n_moves++; // Increment move count
            return false; // Move is valid
        }
        return true; // Move is invalid
    }

    // Display the current state of the board
    void display_board() override {
        cout << "   0   1   2\n";
        cout << "  ------------\n";
        for (int i = 0; i < rows; i++) {
            cout << i << " |";
            for (int j = 0; j < columns; j++) {
                if (board[i][j] == 0)
                    cout << " . |"; // Display empty cell
                else
                    cout << " " << board[i][j] << " |"; // Display cell with number
            }
            cout << endl;
            cout << "  ------------\n";
        }
    }

    // Check if there is a winning condition
    bool is_win() override {
        // Check rows for sum of 15
        for (int i = 0; i < rows; i++) {
            if (board[i][0] + board[i][1] + board[i][2] == 15 &&
                board[i][0] != 0 && board[i][1] != 0 && board[i][2] != 0) {
                return true;
            }
        }

        // Check columns for sum of 15
        for (int j = 0; j < columns; j++) {
            if (board[0][j] + board[1][j] + board[2][j] == 15 &&
                board[0][j] != 0 && board[1][j] != 0 && board[2][j] != 0) {
                return true;
            }
        }

        // Check diagonals for sum of 15
        if ((board[0][0] + board[1][1] + board[2][2] == 15 &&
             board[0][0] != 0 && board[1][1] != 0 && board[2][2] != 0) ||
            (board[0][2] + board[1][1] + board[2][0] == 15 &&
             board[0][2] != 0 && board[1][1] != 0 && board[2][0] != 0)) {
            return true;
        }

        return false; // No win condition met
    }

    // Check if the game is a draw
    bool is_draw() override {
        return n_moves == 9 && !is_win(); // Draw if all cells are filled and no win
    }

    // Check if the game is over (win or draw)
    bool game_is_over() override {
        return is_win() || is_draw();
    }
};
// Class for human player in Numeric Tic Tac Toe game
class HumanPlayer_game5 : public Player<int> {
private:
    vector<int> available_numbers; // Numbers available for the player to use

public:
    HumanPlayer_game5(string name, int symbol, const vector<int>& numbers)
            : Player<int>(name, symbol), available_numbers(numbers) {}

    // Get move from human player
    void getmove(int& x, int& y) override {
        if (this->boardPtr == nullptr || this->boardPtr->game_is_over()) {
            return; // Do nothing if the game is over or the board is not set
        }

        int num;
        cout << this->name << ", your available numbers: ";
        for (int n : available_numbers) cout << n << " "; // Display available numbers
        cout << endl;
        cout << "Enter the number you want to place: ";
        cin >> num;

        // Validate the chosen number
        while (find(available_numbers.begin(), available_numbers.end(), num) == available_numbers.end()) {
            cout << "Invalid number. Choose again: ";
            cin >> num;
        }

        // Remove the chosen number from available numbers
        available_numbers.erase(remove(available_numbers.begin(), available_numbers.end(), num), available_numbers.end());

        cout << "Enter the row and column to place your number (0-2): ";
        cin >> x >> y;

        // Validate the move and place the number
        while (this->boardPtr->update_board(x, y, num)) {
            cout << "Invalid move. Try again.\n";
            available_numbers.push_back(num); // Add the number back to available numbers
            cout << "Enter the number you want to place: ";
            cin >> num;

            while (find(available_numbers.begin(), available_numbers.end(), num) == available_numbers.end()) {
                cout << "Invalid number. Choose again: ";
                cin >> num;
            }

            available_numbers.erase(remove(available_numbers.begin(), available_numbers.end(), num), available_numbers.end());
            cout << "Enter the row and column to place your number (e.g., 0 1): ";
            cin >> x >> y;
        }
    }
};

// Class for random player in Numeric Tic Tac Toe game
class randomPlayer_game5 : public Player<int> {
private:
    vector<int> available_numbers; // Numbers available for the random player to use

public:
    randomPlayer_game5(string name, const vector<int>& numbers)
            : Player<int>(name, symbol), available_numbers(numbers) {
        srand(static_cast<unsigned int>(time(0)));
    }

    // Get move from random player
    void getmove(int& x, int& y) override {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (this->boardPtr == nullptr || this->boardPtr->game_is_over()) {
            return; // Do nothing if the game is over or the board is not set
        }

        int numIndex = rand() % available_numbers.size(); // Randomly select an available number
        int num = available_numbers[numIndex];
        bool validMove = false;

        // Try random positions until a valid move is found
        while (!validMove) {
            x = rand() % 3; // Random row
            y = rand() % 3; // Random column
            if (this->boardPtr->update_board(x, y, num) == false) { // If the move is valid
                validMove = true;
                available_numbers.erase(available_numbers.begin() + numIndex); // Remove the number from available numbers
                cout << name << " placed " << num << " at (" << x << ", " << y << ")\n";
            }
        }
    }
};

void menu_game_5() {
    NumTicTacToeBoard board; // Create the Numeric Tic Tac Toe board
    vector<int> player1_numbers = {1, 3, 5, 7, 9}; // Numbers available to Player 1
    vector<int> player2_numbers = {2, 4, 6, 8}; // Numbers available to Player 2

    Player<int>* players[2];

    // Setup Player 1
    while (true) {
        string player1Type;
        cout << "Is Player 1  a human or a random player?  " << endl;
        cout << "1. Human player " << endl;
        cout << "2. Random player" << endl;
        cout << "Please enter a choice 1 or 2: " << endl;
        cin >> player1Type;
        if (player1Type == "1") {
            players[0] = new HumanPlayer_game5("Human 1 ", 1, player1_numbers);
            break;
        } else if (player1Type == "2") {
            players[0] = new randomPlayer_game5("Computer Random 1 ", player1_numbers);
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    // Setup Player 2
    while (true) {
        string player2Type;
        cout << "Is Player 2  a human or a random player?  " << endl;
        cout << "1. Human player " << endl;
        cout << "2. Random player" << endl;
        cout << "Please enter a choice 1 or 2: " << endl;
        cin >> player2Type;
        if (player2Type == "1") {
            players[1] = new HumanPlayer_game5("Human 2 ", 2, player2_numbers);
            break;
        } else if (player2Type == "2") {
            players[1] = new randomPlayer_game5("Computer Random 2 ", player2_numbers);
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    // Assign the board to players
    players[0]->setBoard(&board);
    players[1]->setBoard(&board);

    // Initialize the game manager and start the game
    GameManager<int> game(&board, players);
    game.run();

    // Clean up dynamically allocated memory
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }

    // Reset the board for a new game
    board.reset_board();
}


//####################################################################################################################################################################################################################
//                                                GAME  6

// Class for Misere Tic Tac Toe game board
class misere_board : public Board<string> {
public:
    static string current_symbol; // Static variable to track the current symbol

    // Set the current symbol
    static void set_current_symbol(const string& s) {
        current_symbol = s;
    }

    // Get the current symbol
    string get_current_symbol() {
        return current_symbol;
    }

    // Constructor to initialize the board
    misere_board() {
        this->rows = 3;
        this->columns = 3;

        board = new string*[rows];
        for (int i = 0; i < rows; ++i) {
            board[i] = new string[columns];
            for (int j = 0; j < columns; ++j) {
                board[i][j] = "(" + to_string(i) + "," + to_string(j) + ")"; // Initialize each cell with its coordinates
            }
        }
    }

    // Destructor to delete the board
    ~misere_board() {
        for (int i = 0; i < rows; ++i) {
            delete[] board[i];
        }
        delete[] board;
    }

    // Update the board with the player's move
    bool update_board(int x, int y, string symbol) override {
        string opponentSymbol = (symbol == "X") ? "O" : "X";

        if (count_sequences(opponentSymbol) > 0) { // Check if the opponent has already won
            cout << endl << endl;
            return true;
        }

        string cell_content = "(" + to_string(x) + "," + to_string(y) + ")";
        if (x >= 0 && x < rows && y >= 0 && y < columns && board[x][y] == cell_content) {
            board[x][y] = symbol; // Place the symbol on the board
            n_moves++; // Increment move count
            return true;
        }
        return false; // Invalid move
    }

    // Display the current state of the board
    void display_board() override {
        cout << endl;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                cout << setw(6) << board[i][j];
                if (j < columns - 1) {
                    cout << "|";
                }
            }
            cout << endl;
            if (i < rows - 1) {
                cout << string(columns * 7 - 1, '-') << endl;
            }
        }
        cout << endl;
    }

    // Check if there is a winning condition
    bool is_win() override {
        string opponentSymbol = (get_current_symbol() == "X") ? "O" : "X";
        if (count_sequences(opponentSymbol) > 0) { // Check if the opponent has a winning sequence
            return true;
        }
        return false;
    }

    // Check if the game is a draw
    bool is_draw() override {
        return ((n_moves == 9 && !is_win() && count_sequences("X") == 0 && count_sequences("O") == 0));
    }

    // Check if the game is over (win or draw)
    bool game_is_over() override {
        if (is_win()) {
            if (count_sequences("X") > 0) {
                cout << "Player 1 (X) loses the game by getting three in a row!" << endl;
            } else if (count_sequences("O") > 0) {
                cout << "Player 2 (O) loses the game by getting three in a row!" << endl;
            }
            return true;
        }
        if (is_draw()) {
            return true;
        }
        return false;
    }

    // Count sequences of three consecutive symbols
    int count_sequences(const string& symbol) {
        int counter = 0;

        // Check rows for sequences
        for (int row = 0; row < rows; row++) {
            if (board[row][0] == symbol && board[row][1] == symbol && board[row][2] == symbol) {
                counter++;
            }
        }

        // Check columns for sequences
        for (int col = 0; col < columns; col++) {
            if (board[0][col] == symbol && board[1][col] == symbol && board[2][col] == symbol) {
                counter++;
            }
        }

        // Check main diagonal for sequence
        if (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol) {
            counter++;
        }

        // Check anti-diagonal for sequence
        if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol) {
            counter++;
        }

        return counter;
    }
};

set<pair<int, int>> usedMoves_game6;
string misere_board:: current_symbol = "";

// Class for human player in Misere Tic Tac Toe game
class current_misere_player_game6 : public Player<string> {
private:
    misere_board* boardPtr2; // Pointer to the game board
public:
    current_misere_player_game6(string n, string s, misere_board* boardPtr) : Player(n, s), boardPtr2(boardPtr) {
        misere_board::set_current_symbol(this->symbol); // Set the current symbol for the board
    }

    // Get count of sequences for a given symbol
    int get_count_sequences(const std::string& symbol) {
        return boardPtr2->count_sequences(symbol);
    }

    // Get move from human player
    void getmove(int& x, int& y) override {
        misere_board::current_symbol = this->symbol; // Set the current symbol for this move
        string opponentSymbol = (this->symbol == "X") ? "O" : "X";

        if (get_count_sequences(opponentSymbol) > 0) { // If the opponent has a winning sequence
            x = 0;
            y = 0;
            return;
        }
        while (true) {
            cout << getname() << "'s turn. Enter your move (row and column): ";
            if (cin >> x >> y) {
                if (x >= 0 && x < 3 && y >= 0 && y < 3) { // Validate the move
                    break;
                } else {
                    cout << "Invalid input. Please enter valid integers within the range 0-2." << endl;
                }
            } else {
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
                cout << "Invalid input. Please enter valid integers within the range 0-2." << endl;
            }
        }
        usedMoves_game6.insert({x, y}); // Insert the move into the set of used moves
    }
};

// Class for random player in Misere Tic Tac Toe game
class random_misere_player_game6 : public Player<string> {
private:
    misere_board* boardPtr3; // Pointer to the game board
public:
    random_misere_player_game6(string n, string s, misere_board* boardPtr) : Player(n, s), boardPtr3(boardPtr) {}

    // Get count of sequences for a given symbol
    int get_count_sequences(const std::string& symbol) {
        return boardPtr3->count_sequences(symbol);
    }

    // Get move from random player
    void getmove(int& x, int& y) override {
        misere_board::current_symbol = this->symbol; // Set the current symbol for this move
        string opponentSymbol = (this->symbol == "X") ? "O" : "X";

        if (get_count_sequences(opponentSymbol) > 0) { // If the opponent has a winning sequence
            x = 0;
            y = 0;
            return;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1)); // Add a delay

        do {
            x = rand() % 3; // Random row
            y = rand() % 3; // Random column
        } while (usedMoves_game6.find({x, y}) != usedMoves_game6.end()); // Check if the move has already been made
        usedMoves_game6.insert({x, y}); // Insert the move into the set of used moves
        cout << "Player " << getsymbol() << " (random) chose: " << x << "," << y << endl << endl;
        cout << "----------------------------------" << endl;
    }
};

void menu_game_6() {
    cout << "Welcome to our misere tic tac toe game!" << endl;

    usedMoves_game6.clear(); // Clear any used moves from previous games
    misere_board::current_symbol = ""; // Reset the current symbol

    srand(time(0)); // Seed the random number generator
    misere_board* board = new misere_board(); // Initialize the game board

    Player<string>* player1;
    Player<string>* player2;

    // Setup Player 1
    while (true) {
        string player1Type;
        cout << "Is Player 1 (symbol 'X') a human or a random player? " << endl;
        cout << "1. Human player " << endl;
        cout << "2. Random player" << endl;
        cout << "Please enter a choice 1 or 2: " << endl;
        cin >> player1Type;
        if (player1Type == "1") {
            player1 = new current_misere_player_game6("Human 1 (X) ", "X", board);
            break;
        } else if (player1Type == "2") {
            player1 = new random_misere_player_game6("Computer Random 1 (X) ", "X", board);
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    // Setup Player 2
    while (true) {
        string player2Type;
        cout << "Is Player 2 (symbol 'O') a human or a random player? " << endl;
        cout << "1. Human player " << endl;
        cout << "2. Random player" << endl;
        cout << "Please enter a choice 1 or 2: ";
        cin >> player2Type;
        if (player2Type == "1") {
            player2 = new current_misere_player_game6("Human 2 (O) ", "O", board);
            break;
        } else if (player2Type == "2") {
            player2 = new random_misere_player_game6("Computer Random 2 (O) ", "O", board);
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    Player<string>* players[2] = {player1, player2}; // Array of players

    // Initialize the game manager and start the game
    GameManager<string> gameManager(board, players);
    gameManager.run();

    // Display the results
    if (board->count_sequences("X") > 0) {
        cout << "Player 1 (X) loses the game by getting three in a row!" << endl;
    }
    if (board->count_sequences("O") > 0) {
        cout << "Player 2 (O) loses the game by getting three in a row!" << endl;
    }
    if (board->count_sequences("X") == 0 && board->count_sequences("O") == 0) {
        cout << "No one of the players gets three in a row!" << endl;
    }

    // Clean up dynamically allocated memory
    delete board;
    delete player1;
    delete player2;
}


//##################################################################################################################################################
//                                               GAME 8


// Class for 9x9 Tic Tac Toe game board
class ninexnine_board : public Board<string> {
public:
    vector<vector<string>> smallGridWinners; // Track the winners of each 3x3 grid
    int total_moves = 0; // Track total moves made in the game

    // Constructor to initialize the board
    ninexnine_board() {
        this->rows = 9;
        this->columns = 9;

        smallGridWinners.resize(3, vector<string>(3, " ")); // Initialize small grid winners

        board = new string*[rows];
        for (int i = 0; i < rows; ++i) {
            board[i] = new string[columns];
            for (int j = 0; j < columns; ++j) {
                board[i][j] = "[" + to_string(i) + "," + to_string(j) + "]"; // Initialize each cell with its coordinates
            }
        }
    }

    // Destructor to delete the board
    ~ninexnine_board() {
        for (int i = 0; i < rows; ++i) {
            delete[] board[i];
        }
        delete[] board;
    }

    // Reset the board to its initial state
    void reset() {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                board[i][j] = "[" + to_string(i) + "," + to_string(j) + "]"; // Reset all cells to initial state
            }
        }

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                smallGridWinners[i][j] = " "; // Reset small grid winners
            }
        }

        total_moves = 0; // Reset move count
    }

    // Update the board with the player's move
    bool update_board(int x, int y, string symbol) override {
        if (x >= 0 && x < rows && y >= 0 && y < columns && board[x][y] == "[" + to_string(x) + "," + to_string(y) + "]") {
            board[x][y] = symbol; // Place the symbol on the board
            total_moves++;

            if (checkSmallGridWin(x / 3, y / 3)) {
                for (int i = x / 3 * 3; i < (x / 3 + 1) * 3; ++i) {
                    for (int j = y / 3 * 3; j < (y / 3 + 1) * 3; ++j) {
                        if (board[i][j] == "[" + to_string(i) + "," + to_string(j) + "]") {
                            board[i][j] = symbol;
                            total_moves++;
                        } else {
                            board[i][j] = symbol;
                        }
                    }
                }
                smallGridWinners[x / 3][y / 3] = symbol; // Mark the small grid as won
                cout << "Player (" << symbol << ") wins the smaller grid!" << endl;
            } else if (isGridFull(x / 3, y / 3)) {
                cout << "Small grid (" << x / 3 << ", " << y / 3 << ") is full!" << endl;
            }
            return true;
        }
        return false; // Invalid move
    }

    // Display the current state of the board
    void display_board() override {
        cout << "\n 3x3 Grid [winners status]:\n";
        displaySmallerGrid();

        cout << "\n\n9x9 Grid:\n";
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                cout << setw(6);
                cout << board[i][j];
                if ((j + 1) % 3 == 0 && j != 8) {
                    cout << " | ";
                }
            }
            cout << endl;
            if ((i + 1) % 3 == 0 && i != 8) {
                cout << string(66, '-') << endl;
            }
        }
        cout << endl;
    }

    // Display the winners of each small 3x3 grid
    void displaySmallerGrid() {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                cout << setw(6) << smallGridWinners[i][j];
                if (j < 2) {
                    cout << "|";
                }
            }
            cout << endl;
            if (i < 2) {
                cout << string(21, '-') << endl;
            }
        }
    }
    // Check if the move is valid
    bool isValidMove(int row, int col) {
        return row >= 0 && row < 9 && col >= 0 && col < 9 &&
               (board[row][col] == "[" + to_string(row) + "," + to_string(col) + "]");
    }

    // Check if a small 3x3 grid is full
    bool isGridFull(int gridRow, int gridCol) {
        for (int i = gridRow * 3; i < (gridRow + 1) * 3; i++) {
            for (int j = gridCol * 3; j < (gridCol + 1) * 3; j++) {
                if (board[i][j] == "[" + to_string(i) + "," + to_string(j) + "]") {
                    return false;
                }
            }
        }
        return true;
    }

    // Check if a player wins a small 3x3 grid
    bool checkSmallGridWin(int gridRow, int gridCol) {
        int startRow = gridRow * 3;
        int startCol = gridCol * 3;

        // Check rows within the small grid
        for (int i = 0; i < 3; i++) {
            if (board[startRow + i][startCol] == board[startRow + i][startCol + 1] &&
                board[startRow + i][startCol] == board[startRow + i][startCol + 2] &&
                board[startRow + i][startCol] != "[" + to_string(startRow + i) + "," + to_string(startCol) + "]") {
                return true;
            }

            // Check columns within the small grid
            if (board[startRow][startCol + i] == board[startRow + 1][startCol + i] &&
                board[startRow][startCol + i] == board[startRow + 2][startCol + i] &&
                board[startRow][startCol + i] != "[" + to_string(startRow) + "," + to_string(startCol + i) + "]") {
                return true;
            }
        }

        // Check main diagonal within the small grid
        if (board[startRow][startCol] == board[startRow + 1][startCol + 1] &&
            board[startRow][startCol] == board[startRow + 2][startCol + 2] &&
            board[startRow][startCol] != "[" + to_string(startRow) + "," + to_string(startCol) + "]") {
            return true;
        }

        // Check anti-diagonal within the small grid
        if (board[startRow + 2][startCol] == board[startRow + 1][startCol + 1] &&
            board[startRow + 2][startCol] == board[startRow][startCol + 2] &&
            board[startRow + 2][startCol] != "[" + to_string(startRow + 2) + "," + to_string(startCol) + "]") {
            return true;
        }

        return false;
    }

    // Check if a player wins the overall board
    bool checkOverallBoardWin(string symbol) {
        // Check rows
        for (int i = 0; i < 3; i++) {
            if (smallGridWinners[i][0] != " " && smallGridWinners[i][0] == symbol && smallGridWinners[i][1] == symbol && smallGridWinners[i][2] == symbol) {
                return true;
            }

            // Check columns
            if (smallGridWinners[0][i] != " " && smallGridWinners[0][i] == symbol && smallGridWinners[1][i] == symbol  &&  smallGridWinners[2][i] == symbol) {
                return true;
            }
        }

        // Check diagonals
        if (smallGridWinners[0][0] != " " && smallGridWinners[0][0] == symbol && smallGridWinners[1][1] == symbol && smallGridWinners[2][2] == symbol) {
            return true;
        }

        if (smallGridWinners[0][2] != " " && smallGridWinners[0][2] == symbol && smallGridWinners[1][1] == symbol && smallGridWinners[2][0] == symbol) {
            return true;
        }

        return false;
    }

    // Check if there is a winning condition
    bool is_win() override {
        if (checkOverallBoardWin("X") || checkOverallBoardWin("O")) {
            display_board();
            return true;
        }
        return false;
    }

    // Check if the game is a draw
    bool is_draw() override {
        return total_moves == 81;
    }

    // Check if the game is over (win or draw)
    bool game_is_over() override {
        return is_win() || is_draw();
    }
};
// Class for human player in 9x9 Tic Tac Toe game
class current_player_game8 : public Player<string> {
private:
    ninexnine_board* boardPtr; // Pointer to the game board
public:
    current_player_game8(string n, string s, ninexnine_board* b) : Player(n, s), boardPtr(b) {}

    // Get move from human player
    void getmove(int& x, int& y) override {
        while (true) {
            cout << "Player (" << getsymbol() << ") Enter the row and column (0-8) where you want to play: ";
            cin >> x >> y;

            if (cin.fail() || !boardPtr->isValidMove(x, y)) { // Validate the move
                cout << "Invalid move. Please try again.\n";
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
            } else {
                break; // Valid move entered
            }
        }
    }
};
// Class for random player in 9x9 Tic Tac Toe game
class random_player_game8 : public Player<string> {
private:
    ninexnine_board* boardPtr; // Pointer to the game board
public:
    random_player_game8(string n, string s, ninexnine_board* b) : Player(n, s), boardPtr(b) {}

    // Get move from random player
    void getmove(int& x, int& y) override {
        std::this_thread::sleep_for(std::chrono::seconds(0)); // Add a delay

        do {
            x = rand() % 9; // Random row
            y = rand() % 9; // Random column
        } while (!boardPtr->isValidMove(x, y)); // Check if the move is valid

        cout << getname() << " chose: (" << x << "," << y << ")" << endl;
    }
};

void menu_game_8() {
    cout << "Welcome to our 9x9 Tic Tac Toe game!" << endl;

    srand(time(0));
    ninexnine_board* board = new ninexnine_board(); // Initialize the game board

    Player<string>* player1;
    Player<string>* player2;

    // Setup Player 1
    while (true) {
        string player1Type;
        cout << "Is Player 1 (X) a Human or a Computer Random? " << endl;
        cout << "1. Human 1 (X)" << endl;
        cout << "2. Computer Random 1 (X)" << endl;
        cout << "Please enter a choice 1 or 2: " << endl;
        cin >> player1Type;
        if (player1Type == "1") {
            player1 = new current_player_game8("Human 1 (X) ", "X", board);
            break;
        } else if (player1Type == "2") {
            player1 = new random_player_game8("Computer Random 1 (X) ", "X", board);
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    // Setup Player 2
    while (true) {
        string player2Type;
        cout << "Is Player 2 (O) a Human or a Computer Random? " << endl;
        cout << "1. Human 2 (O)" << endl;
        cout << "2. Computer Random 2 (O)" << endl;
        cout << "Please enter a choice 1 or 2: " << endl;
        cin >> player2Type;
        if (player2Type == "1") {
            player2 = new current_player_game8("Human 2 (O) ", "O", board);
            break;
        } else if (player2Type == "2") {
            player2 = new random_player_game8("Computer Random 2 (O) ", "O", board);
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    Player<string>* players[2] = {player1, player2}; // Array of players

    // Initialize the game manager and start the game
    GameManager<string> gameManager(board, players);
    gameManager.run();

    // Reset the board for a new game
    board->reset();

    // Clean up dynamically allocated memory
    delete board;
    delete player1;
    delete player2;
}


//##################################################################################################################################################
//                                               GAME 9

set < pair <int,int>> usedMoves_game9;
// Class for SUS Tic Tac Toe game board
class _SUS_board : public Board<string> {
private:
    int main_counter; // Count sequences of "SUS" on the board
    int player1_counter; // Count sequences made by Player 1
    int player2_counter; // Count sequences made by Player 2

public:
    int total_moves; // Track total moves made in the game

    // Constructor to initialize the board
    _SUS_board() {
        this->rows = 3;
        this->columns = 3;
        main_counter = 0;
        player1_counter = 0;
        player2_counter = 0;
        total_moves = 0;
        board = new string*[rows];
        for (int i = 0; i < rows; ++i) {
            board[i] = new string[columns];
            for (int j = 0; j < columns; ++j) {
                board[i][j] = "(" + to_string(i) + "," + to_string(j) + ")"; // Initialize each cell with its coordinates
            }
        }
    }

    // Destructor to delete the board
    ~_SUS_board() {
        for (int i = 0; i < rows; ++i) {
            delete[] board[i];
        }
        delete[] board;
    }

    // Reset the board to its initial state
    void reset() {
        main_counter = 0;
        player1_counter = 0;
        player2_counter = 0;
        total_moves = 0;

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                board[i][j] = "(" + to_string(i) + "," + to_string(j) + ")"; // Reset all cells to initial state
            }
        }
    }

    // Update the board with the player's move
    bool update_board(int x, int y, string symbol) override {
        if (total_moves == 10) {
            return true;
        }

        string cell_content = "(" + to_string(x) + "," + to_string(y) + ")";
        if (x >= 0 && x < rows && y >= 0 && y < columns && board[x][y] == cell_content) {
            int previous_main_counter = main_counter;
            board[x][y] = symbol; // Place the symbol on the board
            main_counter = count_sequences(); // Update main counter
            if (main_counter > previous_main_counter) {
                if (symbol == "S") {
                    player1_counter += main_counter - previous_main_counter ; // Increment Player 1's counter
                    cout << "Player 1 (S) created SUS sequence! " << endl;
                    cout << "Total sequences of player (S): " << player1_counter << " sequences." << endl;
                    cout << "Total sequences of player (U): " << player2_counter << " sequences." <<endl;
                } else if (symbol == "U") {
                    player2_counter += main_counter - previous_main_counter ; // Increment Player 2's counter
                    cout << "Player 2 (U) created SUS sequence! " << endl;
                    cout << "Total sequences of player (S): " << player1_counter << " sequences." << endl;
                    cout << "Total sequences of player (U): " << player2_counter << " sequences." << endl;
                }
            }
            total_moves++; // Increment total moves count
            return true;
        }
        return false; // Invalid move
    }

    // Display the current state of the board
    void display_board() override {
        cout << endl;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                cout << setw(6) << board[i][j];
                if (j < columns - 1) {
                    cout << "|";
                }
            }
            cout << endl;
            if (i < rows - 1) {
                cout << string(columns * 7 - 1, '-') << endl;
            }
        }
        cout << endl ;
    }

    // Check if there is a winning condition
    bool is_win() override {
        if (total_moves == 9) {
            return player1_counter > player2_counter;
        }
        if (total_moves == 10) {
            return player2_counter > player1_counter;
        }
        return false;
    }

    // Check if the game is a draw
    bool is_draw() override {
        return (total_moves == 9 && player1_counter == player2_counter);
    }

    // Check if the game is over (win or draw)
    bool game_is_over() override {
        return is_draw() || is_win();
    }

    // Count sequences of "SUS" on the board
    int count_sequences() {
        int counter = 0;
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col <= columns - 3; col++) {
                if (board[row][col] == "S" && board[row][col + 1] == "U" && board[row][col + 2] == "S") {
                    counter++;
                }
            }
        }
        for (int col = 0; col < columns; col++) {
            for (int row = 0; row <= rows - 3; row++) {
                if (board[row][col] == "S" && board[row + 1][col] == "U" && board[row + 2][col] == "S") {
                    counter++;
                }
            }
        }
        for (int row = 0; row <= rows - 3; row++) {
            for (int col = 0; col <= columns - 3; col++) {
                if (board[row][col] == "S" && board[row + 1][col + 1] == "U" && board[row + 2][col + 2] == "S") {
                    counter++;
                }
            }
        }
        for (int row = 0; row <= rows - 3; row++) {
            for (int col = 2; col < columns; col++) {
                if (board[row][col] == "S" && board[row + 1][col - 1] == "U" && board[row + 2][col - 2] == "S") {
                    counter++;
                }
            }
        }
        return counter;
    }
};

// Class for human player in SUS Tic Tac Toe game
class current_player_game9 : public Player<string> {
private:
    _SUS_board* boardPtr2; // Pointer to the game board

public:
    current_player_game9(string n, string s, _SUS_board* b) : Player(n, s), boardPtr2(b) {}

    // Get move from human player
    void getmove(int &x, int &y) override {
        if (boardPtr2->total_moves == 9) { // If 9 moves have been made, set the move to (0,0)
            x = 0;
            y = 0;
            boardPtr2->total_moves++;
            return;
        }
        while (true) {
            cout << getname() << "'s turn. Enter your move (row and column): ";
            if (cin >> x >> y) {
                if (x >= 0 && x < 3 && y >= 0 && y < 3 && usedMoves_game9.find({x, y}) == usedMoves_game9.end()) {
                    usedMoves_game9.insert({x, y}); // Insert the move into the set of used moves
                    break;
                } else {
                    cout << "Invalid input. Please enter valid integers within the range 0-2 and not already used." << endl;
                }
            } else {
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
                cout << "Invalid input. Please enter valid integers within the range 0-2." << endl;
            }
        }
    }
};

// Class for random player in SUS Tic Tac Toe game
class random_player_game9 : public Player<string> {
private:
    _SUS_board* boardPtr2; // Pointer to the game board

public:
    random_player_game9(string n, string s, _SUS_board* b) : Player(n, s), boardPtr2(b) {}

    // Get move from random player
    void getmove(int &x, int &y) override {
        if (boardPtr2->total_moves == 9) { // If 9 moves have been made, set the move to (0,0)
            x = 0;
            y = 0;
            boardPtr2->total_moves++;
            return;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Add a delay

        do {
            x = rand() % 3; // Random row
            y = rand() % 3; // Random column
        } while (usedMoves_game9.find({x, y}) != usedMoves_game9.end()); // Check if the move has already been made
        usedMoves_game9.insert({x, y}); // Insert the move into the set of used moves
        cout << "Player '" << getsymbol() << "' (random) chose: " << x << "," << y << endl << endl;
        cout << "----------------------------------" << endl;
    }
};

void menu_game_9() {
    cout << "Welcome to our SUS Tic Tac Toe game!" << endl;
    srand(time(0));
    _SUS_board* board = new _SUS_board(); // Initialize the game board

    Player<string>* player1;
    Player<string>* player2;

    // Setup Player 1
    while (true) {
        string player1Type;
        cout << "Is Player 1 (symbol 'S') a human or a random player? " << endl;
        cout << "1. Human player " << endl;
        cout << "2. Random player" << endl;
        cout << "Please enter a choice 1 or 2: " << endl;
        cin >> player1Type;
        if (player1Type == "1") {
            player1 = new current_player_game9("Human 1 (S) ", "S", board);
            break;
        } else if (player1Type == "2") {
            player1 = new random_player_game9("Computer Random 1 (S) ", "S", board);
            break;
        } else {
            cout << "Invalid choice. please try again" << endl;
        }
    }

    // Setup Player 2
    while (true) {
        string player2Type;
        cout << "Is Player 2 (symbol 'U') a human or a random player? " << endl;
        cout << "1. Human player " << endl;
        cout << "2. Random player" << endl;
        cout << "Please enter a choice 1 or 2: " << endl;
        cin >> player2Type;
        if (player2Type == "1") {
            player2 = new current_player_game9("Human 2 (U) ", "U", board);
            break;
        } else if (player2Type == "2") {
            player2 = new random_player_game9("Computer Random 2 (U) ", "U", board);
            break;
        } else {
            cout << "Invalid choice. please try again" << endl;
        }
    }

    Player<string>* players[2] = {player1, player2}; // Array of players

    // Initialize the game manager and start the game
    GameManager<string> gameManager(board, players);
    gameManager.run();


    // Reset the board for a new game
    board->reset();
    usedMoves_game9.clear();

    // Clean up dynamically allocated memory
    delete board;
    delete player1;
    delete player2;
}



#endif
//####################################################################################################################################################################################################################