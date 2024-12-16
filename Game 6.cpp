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

set<pair<int, int>> usedMoves;

class misere_board : public Board<string> {
public:
    static string current_symbol;

    static void set_current_symbol(const string& s) {
        current_symbol = s;
    }

    string get_current_symbol() {
        return current_symbol;
    }

    misere_board() {
        this->rows = 3;
        this->columns = 3;

        board = new string*[rows];
        for (int i = 0; i < rows; ++i) {
            board[i] = new string[columns];
            for (int j = 0; j < columns; ++j) {
                board[i][j] = "(" + to_string(i) + "," + to_string(j) + ")";
            }
        }
    }

    ~misere_board() {
        for (int i = 0; i < rows; ++i) {
            delete[] board[i];
        }
        delete[] board;
    }

    bool update_board(int x, int y, string symbol) override {
        string opponentSymbol = (symbol == "X") ? "O" : "X";

        if (count_sequences(opponentSymbol) > 0) {
            cout << endl << endl;
            return true;
        }

        string cell_content = "(" + to_string(x) + "," + to_string(y) + ")";
        if (x >= 0 && x < rows && y >= 0 && y < columns && board[x][y] == cell_content) {
            board[x][y] = symbol;
            n_moves++;
            return true;
        }
        return false;
    }

    void display_board() override {
        cout<< endl;
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
        string opponentSymbol = (get_current_symbol() == "X") ? "O" : "X";
        if (count_sequences(opponentSymbol) > 0) {
            return true;
        }
        return false;
    }

    bool is_draw() override {
        return ((n_moves == 9  && !is_win() && count_sequences("X") == 0 && count_sequences("O") == 0) );
    }

    bool game_is_over() override {
        if (is_win()){
            if (count_sequences("X") > 0 ){
                cout << "Player 1 (X) loses the game by getting three in a row!" << endl;
            } else if(count_sequences("O") > 0){
                cout << "Player 2 (O) loses the game by getting three in a row!" << endl;
            }
            return true;
        }
        if (is_draw()){
            return true;
        }
        return false;
    }

    int count_sequences(const string& symbol) {
        int counter = 0;

        for (int row = 0; row < rows; row++) {
            if (board[row][0] == symbol && board[row][1] == symbol && board[row][2] == symbol) {
                counter++;
            }
        }
        for (int col = 0; col < columns; col++) {
            if (board[0][col] == symbol && board[1][col] == symbol && board[2][col] == symbol) {
                counter++;
            }
        }
        if (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol) {
            counter++;
        }
        if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol) {
            counter++;
        }
        return counter;
    }
};

string misere_board:: current_symbol ="";

class current_misere_player : public Player<string> {
private:
    misere_board* boardPtr2;
public:
    current_misere_player(string n, string s, misere_board* boardPtr) : Player(n, s), boardPtr2(boardPtr) {
        misere_board::set_current_symbol(this -> symbol);
    }

    int get_count_sequences(const std::string& symbol) {
        return boardPtr2->count_sequences(symbol);
    }

    void getmove(int& x, int& y) override {
        misere_board:: current_symbol = this ->symbol;
        string opponentSymbol = (this->symbol == "X") ? "O" : "X";

        if (get_count_sequences(opponentSymbol) > 0) {
            x = 0;
            y = 0;
            return;
        }
        while (true) {
            cout << getname() << "'s turn. Enter your move (row and column): ";
            if (cin >> x >> y) {
                if (x >= 0 && x < 3 && y >= 0 && y < 3) {
                    break;
                } else {
                    cout << "Invalid input. Please enter valid integers within the range 0-2." << endl;
                }
            } else {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter valid integers within the range 0-2." << endl;
            }
        }
        usedMoves.insert({x, y});
    }
};

class random_misere_player : public Player<string> {
private:
    misere_board* boardPtr3;
public:
    random_misere_player(string n, string s, misere_board* boardPtr) : Player(n, s), boardPtr3(boardPtr) {}

    int get_count_sequences(const std::string& symbol) {
        return boardPtr3->count_sequences(symbol);
    }

    void getmove(int& x, int& y) override {
        misere_board:: current_symbol = this ->symbol;
        string opponentSymbol = (this->symbol == "X") ? "O" : "X";

        if (get_count_sequences(opponentSymbol) > 0) {
            x = 0;
            y = 0;
            return;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));

        do {
            x = rand() % 3;
            y = rand() % 3;
        } while (usedMoves.find({x, y}) != usedMoves.end());
        usedMoves.insert({x, y});
        cout << "Player " << getsymbol() << " (random) chose: " << x << "," << y << endl << endl;
        cout << "----------------------------------" << endl;
    }
};

void menu_game_6() {
    cout << "Welcome to our misere tic tac toe game!" << endl;

    srand(time(0));
    misere_board* board = new misere_board();

    Player<string>* player1;
    Player<string>* player2;

    string player1Type, player2Type;

    cout << "Is Player 1 (symbol 'X') a human or a random player? " << endl;
    cout << "1. Human player " << endl;
    cout << "2. Random player" << endl;
    cout << "Please enter a choice 1 or 2: " << endl;
    cin >> player1Type;
    if (player1Type == "1") {
        player1 = new current_misere_player("Human 1 (X) ", "X", board);
    } else if (player1Type == "2") {
        player1 = new random_misere_player("Computer Random 1 (X) ", "X", board);
    }

    cout << "Is Player 2 (symbol 'O') a human or a random player? " << endl;
    cout << "1. Human player " << endl;
    cout << "2. Random player" << endl;
    cout << "Please enter a choice 1 or 2: ";
    cin >> player2Type;
    if (player2Type == "1") {
        player2 = new current_misere_player("Human 2 (O) ", "O", board);
    } else if (player2Type == "2") {
        player2 = new random_misere_player("Computer Random 2 (O) ", "O", board);
    }

    Player<string>* players[2] = {player1, player2};

    GameManager<string> gameManager(board, players);
    gameManager.run();

    if (board-> count_sequences("X") > 0 ) {
        cout << "Player 1 (X) loses the game by getting three in a row!" << endl;
    }
    if(board -> count_sequences("O") > 0){
        cout << "Player 2 (O) loses the game by getting three in a row!" << endl;
    }
    if (board ->count_sequences("X") == 0 && board ->count_sequences("O") == 0 ){
        cout << "No one of the players get three in a row!" << endl;
    }
    delete board;
    delete player1;
    delete player2;
}

int main() {
    menu_game_6();
    return 0;
}


