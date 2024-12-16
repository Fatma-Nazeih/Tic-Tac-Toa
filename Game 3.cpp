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

class _5x5_board : public Board<string> {
public:
    int total_moves;
    _5x5_board() {
        this->rows = 5;
        this->columns = 5;
        total_moves = 0;

        board = new string*[rows];
        for (int i = 0; i < rows; ++i) {
            board[i] = new string[columns];
            for (int j = 0; j < columns; ++j) {
                board[i][j] = "(" + to_string(i) + "," + to_string(j) + ")";
            }
        }
    }

    ~_5x5_board() {
        for (int i = 0; i < rows; ++i) {
            delete[] board[i];
        }
        delete[] board;
    }

    bool update_board(int x, int y, string symbol) override {
        if (total_moves == 25) {
            return true;
        }
        string cell_content = "(" + to_string(x) + "," + to_string(y) + ")";
        if (x >= 0 && x < rows && y >= 0 && y < columns && board[x][y] == cell_content) {
            board[x][y] = symbol;
            total_moves++;
            return true;
        }
        return false;
    }
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

    bool is_win() override {
        if (total_moves == 24){
            return count_sequences("O") > count_sequences("X");
        }
        if (total_moves == 25){
            return count_sequences("X") > count_sequences("O");
        }
        return false;
    }

    bool is_draw() override {
        return (total_moves == 24 && count_sequences("X") == count_sequences("O") );
    }

    bool game_is_over() override {
        return is_draw() || is_win();
    }

    int count_sequences(string symbol) {
        int counter = 0;

        for (int row = 0; row < rows; row++) {
            for (int col = 0; col <= columns - 3; col++) {
                if (board[row][col] == symbol && board[row][col + 1] == symbol && board[row][col + 2] == symbol) {
                    counter++;
                }
            }
        }
        for (int col = 0; col < columns; col++) {
            for (int row = 0; row <= rows - 3; row++) {
                if (board[row][col] == symbol && board[row + 1][col] == symbol && board[row + 2][col] == symbol) {
                    counter++;
                }
            }
        }
        for (int row = 0; row <= rows - 3; row++) {
            for (int col = 0; col <= columns - 3; col++) {
                if (board[row][col] == symbol && board[row + 1][col + 1] == symbol && board[row + 2][col + 2] == symbol) {
                    counter++;
                }
            }
        }
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


set<pair<int, int>> usedMoves;

class current_player : public Player<string> {
private:
    _5x5_board* boardPtr2;
public:
    current_player(string n, string s, _5x5_board* b) : Player(n, s), boardPtr2(b) {}
    void getmove(int &x, int &y) override {
        if (boardPtr2->total_moves == 24) {
            x = 0;
            y = 0;
            boardPtr2->total_moves++;
            return;
        }
        while (true) {
            cout << getname() << "'s turn. Enter your move (row and column): ";
            if (cin >> x >> y) {
                if (x >= 0 && x < 5 && y >= 0 && y < 5) {
                    break;
                } else {
                    cout << "Invalid input. Please enter valid integers within the range 0-4." << endl;
                }
            } else {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter valid integers within the range 0-4." << endl;
            }
        }
        usedMoves.insert({x, y});
    }
};



class random_player : public Player<string> {
private:
    _5x5_board* boardPtr2;
public:
    random_player(string n, string s, _5x5_board* b) : Player(n, s), boardPtr2(b) {}

    void getmove(int &x, int &y) override {
        if (boardPtr2->total_moves == 24) {
            x = 0;
            y = 0;
            boardPtr2->total_moves++;
            return;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));

        do {
            x = rand() % 5;
            y = rand() % 5;
        } while (usedMoves.find({x, y}) != usedMoves.end());
        usedMoves.insert({x, y});
        cout << "Player \'" << getsymbol() << "\' (random) chose: " << x << "," << y << endl << endl;
        cout << "----------------------------------" << endl;
    }
};


void menu_game_3() {
    cout << "Welcome to our 5x5 tic tac toe game!" << endl;

    srand(time(0));
    _5x5_board* board = new _5x5_board();

    Player<string>* player1;
    Player<string>* player2;

    string player1Type, player2Type;

    cout << "Is Player 1 (symbol 'X') a human or a random player?  " << endl;
    cout << "1. Human player " << endl;
    cout << "2. Random player" << endl;
    cout << "please enter a choice 1 or 2 : " << endl;
    cin >> player1Type;
    if (player1Type == "1") {
        player1 = new current_player("Human 1 (X) ", "X" , board);
    } else if ( player1Type == "2"){
        player1 = new random_player("Computer Random 1 (X) " , "X" , board);
    }

    cout << "Is Player 2 (symbol 'O') a human or a random player?  " << endl;
    cout << "1. Human player " << endl;
    cout << "2. Random player" << endl;
    cout << "please enter a choice 1 or 2 : " << endl;
    cin >> player2Type;
    if (player2Type == "1") {
        player2 = new current_player("Human 2 (O) ", "O" , board);
    } else if ( player2Type == "2"){
        player2 = new random_player("Computer Random 2 (O) " , "O" , board);
    }

    Player<string>* players[2] = {player1, player2};

    GameManager<string> gameManager(board, players);
    gameManager.run();

    if (board->count_sequences("X") > board->count_sequences("O")){
        cout << "Total : " << board->count_sequences("X") <<" sequences of (X) ,and " << board->count_sequences("O") << " sequences of (O). ";
    }
    if (board->count_sequences("O") > board->count_sequences("X")){
        cout << "Total : " << board->count_sequences("O") <<" sequences of (O) ,and " << board->count_sequences("X") << " sequences of (X). ";
    }
    if (board->count_sequences("X") == board->count_sequences("O")){
        cout << "It is a draw with sequences equal  " << board->count_sequences("X") << " for each player!" << endl;
    }
    delete board;
    delete player1;
    delete player2;
}

int main() {
    menu_game_3();
    return 0;
}