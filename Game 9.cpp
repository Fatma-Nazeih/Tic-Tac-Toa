#include "BoardGame_Classes.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <limits>
#include <set>
#include <thread>
#include <chrono>

using namespace std;

class _SUS_board : public Board<string> {
private:
    int main_counter;
    int player1_counter;
    int player2_counter;

public:
    int total_moves;

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
                board[i][j] = "(" + to_string(i) + "," + to_string(j) + ")";
            }
        }
    }

    ~_SUS_board() {
        for (int i = 0; i < rows; ++i) {
            delete[] board[i];
        }
        delete[] board;
    }

    bool update_board(int x, int y, string symbol) override {
        if (total_moves == 10) {
            return true;
        }

        string cell_content = "(" + to_string(x) + "," + to_string(y) + ")";
        if (x >= 0 && x < rows && y >= 0 && y < columns && board[x][y] == cell_content) {
            int previous_main_counter = main_counter;
            board[x][y] = symbol;
            main_counter = count_sequences();
            if (main_counter > previous_main_counter) {
                if (symbol == "S") {
                    player1_counter++;
                } else if (symbol == "U") {
                    player2_counter++;
                }
            }
            total_moves++;
            return true;
        }
        return false;
    }

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

    bool is_win() override {
        if (total_moves == 9){
            return player1_counter > player2_counter;
        }
        if (total_moves == 10){
            return player2_counter > player1_counter;
        }
        return false;
    }

    bool is_draw() override {
        return (total_moves == 9 && player1_counter == player2_counter);
    }

    bool game_is_over() override {
        return is_draw() || is_win();
    }

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
        for (int row = 0 ; row  <= rows - 3; row++) {
            for (int col = 0 ; col <= columns - 3; col++) {
                if (board[row][col] == "S" && board[row + 1][col + 1] == "U" && board[row + 2][col + 2] == "S") {
                    counter++;
                }
            }
        }
        for (int row = 0 ; row  <= rows - 3; row++) {
            for (int col = 2 ; col < columns; col++) {
                if (board[row][col] == "S" && board[row + 1][col - 1] == "U" && board[row + 2][col - 2] == "S") {
                    counter++;
                }
            }
        }
        return counter;
    }
};

set < pair <int,int>> usedMoves;

class current_player : public Player<string> {
private:
    _SUS_board* boardPtr2;

public:
    current_player(string n, string s, _SUS_board* b) : Player(n, s), boardPtr2(b) {}

    void getmove(int &x, int &y) override {
        if (boardPtr2->total_moves == 9) {
            x = 0;
            y = 0;
            boardPtr2->total_moves++;
            return;
        }
        while (true) {
            cout << getname() << "'s turn. Enter your move (row and column): " ;
            if (cin >> x >> y) {
                if (x >= 0 && x < 3 && y >= 0 && y < 3 && usedMoves.find({x, y}) == usedMoves.end()) {
                    usedMoves.insert({x, y});
                    break;
                } else {
                    cout << "Invalid input. Please enter valid integers within the range 0-2 and not already used." << endl;
                }
            } else {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter valid integers within the range 0-2." << endl;
            }
        }
    }
};
class random_player : public Player<string> {
private:
    _SUS_board* boardPtr2;

public:
    random_player(string n, string s, _SUS_board* b) : Player(n, s), boardPtr2(b) {}

    void getmove(int &x, int &y) override {
        if (boardPtr2->total_moves == 9) {
            x = 0;
            y = 0;
            boardPtr2->total_moves++;
            return;
        }
        std::this_thread::sleep_for(std::chrono::seconds(0));
        do {
            x = rand() % 3;
            y = rand() % 3;
        } while (usedMoves.find({x, y}) != usedMoves.end());
        usedMoves.insert({x, y});
        cout << "Player '" << getsymbol() << "' (random) chose: " << x << "," << y << endl << endl;
        cout << "----------------------------------" << endl;
    }
};

void menu_game_9() {
    cout << "Welcome to our SUS Tic Tac Toe game!" << endl;
    srand(time(0));
    _SUS_board* board = new _SUS_board();

    Player<string>* player1;
    Player<string>* player2;

    string player1Type, player2Type;

    cout << "Is Player 1 (symbol 'S') a human or a random player? " << endl;
    cout << "1. Human player " << endl;
    cout << "2. Random player" << endl;
    cout << "Please enter a choice 1 or 2 : " << endl;
    cin >> player1Type;
    if (player1Type == "1") {
        player1 = new current_player("Human 1 (S) ", "S", board);
    } else if (player1Type == "2") {
        player1 = new random_player("Computer Random 1 (S) ", "S", board);
    }

    cout << "Is Player 2 (symbol 'U') a human or a random player? " << endl;
    cout << "1. Human player " << endl;
    cout << "2. Random player" << endl;
    cout << "Please enter a choice 1 or 2 : " << endl;
    cin >> player2Type;
    if (player2Type == "1") {
        player2 = new current_player("Human 2 (U) ", "U", board);
    } else if (player2Type == "2") {
        player2 = new random_player("Computer Random 2 (U) ", "U", board);
    }

    Player<string>* players[2] = {player1, player2};

    GameManager<string> gameManager(board, players);
    gameManager.run();

    delete board;
    delete player1;
    delete player2;
}

int main() {
    menu_game_9();
    return 0;
}
