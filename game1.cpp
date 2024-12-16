#include "BoardGame_Classes.h"
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <limits>
#include <set>
#include <thread>
#include <chrono>
#include <string>
using namespace std;

template <typename T>
class Pyramid_XO : public Board<T> {
public:
    Pyramid_XO() {
        this->rows = 3;
        this->columns = 5;
        this->board = new T*[this->rows];
        for (int i = 0; i < this->rows; ++i) {
            this->board[i] = new T[this->columns];
            for (int j = 0; j < this->columns; ++j) {
                this->board[i][j] = " ";
            }
        }

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

    ~Pyramid_XO() {
        for (int i = 0; i < this->rows; ++i) {
            delete[] this->board[i];
        }
        delete[] this->board;
    }

    bool update_board(int x, int y, T symbol) override {
        if (x < 0 || x >= this->rows || y < 0 || y >= this->columns || this->board[x][y] == " " || this->board[x][y] != "(" + to_string(x) + "," + to_string(y) + ")") {
            cout << "Invalid input" << endl;
            return false;
        } else {
            this->board[x][y] = symbol;
            this->n_moves++;
            return true;
        }
    }

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



    bool is_win() override {
        // Check rows
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->columns - 2; j++) {
                if (this->board[i][j] != " " && this->board[i][j] != "(" + to_string(i) + "," + to_string(j) + ")" && this->board[i][j] == this->board[i][j + 1] && this->board[i][j] == this->board[i][j + 2]) {
                    return true;
                }
            }
        }

        // Check columns
        for (int j = 0; j < this->columns; j++) {
            for (int i = 0; i < this->rows - 2; i++) {
                if (this->board[i][j] != " " && this->board[i][j] != "(" + to_string(i) + "," + to_string(j) + ")" && this->board[i][j] == this->board[i + 1][j] && this->board[i][j] == this->board[i + 2][j]) {
                    return true;
                }
            }
        }

        // Check diagonals
        if (this->board[0][2] != "(" + to_string(0) + "," + to_string(2) + ")" &&
            ((this->board[0][2] == this->board[1][1] && this->board[0][2] == this->board[2][0]) ||
             (this->board[0][2] == this->board[1][3] && this->board[0][2] == this->board[2][4]))) {
            return true;
        }

        if (this->board[2][2] != "(" + to_string(2) + "," + to_string(2) + ")" &&
            ((this->board[2][2] == this->board[1][1] && this->board[2][2] == this->board[0][0]) ||
             (this->board[2][2] == this->board[1][3] && this->board[2][2] == this->board[0][4]))) {
            return true;
        }

        return false;
    }

    bool is_draw() override {
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->columns; j++) {
                if (this->board[i][j] == "(" + to_string(i) + "," + to_string(j) + ")" && this->board[i][j] != " ") {
                    return false;
                }
            }
        }
        return true;
    }

    bool game_is_over() override {
        return this->is_win() || this->is_draw();
    }
};


set<pair<int, int>> usedMoves;
class current_player : public Player<string> {
private:
    Pyramid_XO<string>* boardPtr2;
public:
    current_player(string n, char s, Pyramid_XO<string>* b) : Player<string>(n, string(1, s)), boardPtr2(b) {}

    void getmove(int &x, int &y) override {
        while (true) {
            cout << getname() << "'s turn. Enter your move (row and column): ";
            if (cin >> x >> y) {
                if (x >= 0 && x <= 2 && y >= 0 && y <= 4 && boardPtr2->board[x][y] != " " && usedMoves.find({x, y}) == usedMoves.end()) {
                    usedMoves.insert({x, y});
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

class random_player : public Player<string> {
private:
    Pyramid_XO<string>* boardPtr2;
public:
    random_player(string n, char s, Pyramid_XO<string>* b) : Player<string>(n, string(1, s)), boardPtr2(b) {}

    void getmove(int &x, int &y) override {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        do {
            x = rand() % 3;
            y = rand() % 5;
        } while (boardPtr2->board[x][y] == " " || usedMoves.find({x, y}) != usedMoves.end() );
        usedMoves.insert({x, y});
        cout << "Player '" << getsymbol() << "' (random) chose: " << x << "," << y << endl;
    }
};

void menu_game_3() {
    cout << "Welcome to our pyramid tic tac toe game!" << endl;
    Pyramid_XO<string> board;

    Player<string>* player1;
    Player<string>* player2;

    int player1Type, player2Type;
    string playerName;

    cout << "Is Player 1 (symbol 'X') a human or a random player? " << endl;
    cout << "1. Human player" << endl;
    cout << "2. Random player" << endl;
    cout << "Please enter a choice 1 or 2: ";
    cin >> player1Type;
    if (player1Type == 1) {
        player1 = new current_player("Human 1 (X) ", 'X', &board);
    } else {
        player1 = new random_player("Computer Random 1", 'X', &board);
    }

    cout << "Is Player 2 (symbol 'O') a human or a random player? " << endl;
    cout << "1. Human player" << endl;
    cout << "2. Random player" << endl;
    cout << "Please enter a choice 1 or 2: ";
    cin >> player2Type;
    if (player2Type == 1) {
        player2 = new current_player("Human2 (O) ", 'O', &board);
    } else {
        player2 = new random_player("Computer Random 2", 'O', &board);
    }

    player1->setBoard(&board);
    player2->setBoard(&board);

    Player<string>* players[2] = {player1, player2};

    GameManager<string> gameManager(&board, players);
    gameManager.run();

    delete player1;
    delete player2;
}

int main() {
    menu_game_3();
    return 0;
}
