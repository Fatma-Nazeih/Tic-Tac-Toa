#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include "boardgame_classes.h"
using namespace std;

class ConnectFourBoard : public Board<char> {
public:
    ConnectFourBoard(int rows, int columns) {
        this->rows = rows;
        this->columns = columns;
        this->board = new char*[rows];
        for (int i = 0; i < rows; ++i) {
            this->board[i] = new char[columns];
            fill(this->board[i], this->board[i] + columns, ' ');
        }
    }

    int getBoardColumns() const {
        return this->columns;  
    }

    bool is_column_full(int column) const {
    return this->board[0][column] != ' ';  
   }

    bool update_board(int x, int y, char symbol) override {
        if (y < 0 || y >= this->columns) return false; 
        for (int i = this->rows - 1; i >= 0; --i) {
            if (this->board[i][y] == ' ') {
                this->board[i][y] = symbol;
                this->n_moves++;
                return true;
            }
        }
        return false;
    }

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

    bool is_win() override {
        for (int i = 0; i < this->rows; ++i) {
            for (int j = 0; j < this->columns; ++j) {
                char symbol = this->board[i][j];
                if (symbol == ' ') continue;
                if (j + 3 < this->columns &&
                     this->board[i][j + 1] == symbol  &&
                     this->board[i][j + 2] == symbol &&
                     this->board[i][j + 3] == symbol)
                    return true;

                if (i + 3 < this->rows &&
                     this->board[i + 1][j] == symbol &&
                     this->board[i + 2][j] == symbol &&
                     this->board[i + 3][j] == symbol)
                    return true;

                if (i + 3 < this->rows && j + 3 < this->columns &&
                     this->board[i + 1][j + 1] == symbol &&
                     this->board[i + 2][j + 2] == symbol &&
                     this->board[i + 3][j + 3] == symbol)
                    return true;

                if (i + 3 < this->rows && j - 3 >= 0 &&
                     this->board[i + 1][j - 1] == symbol &&
                     this->board[i + 2][j - 2] == symbol&&
                     this->board[i + 3][j - 3] == symbol)
                    return true;
            }
        }
        return false;
    }

    bool is_draw() override {
        return this->n_moves == this->rows * this->columns && !is_win();
    }

    bool game_is_over() override {
        return is_win() || is_draw();
    }
};


class HumanPlayer : public Player<char> {
public:
    HumanPlayer(string name, char symbol) : Player<char>(name, symbol) {}

    void getmove(int& x, int& y) override {
    auto* connectFourBoard = dynamic_cast<ConnectFourBoard*>(this->boardPtr);
    int columns = connectFourBoard->getBoardColumns();
    while (true) {
        cout << this->getname() << " (" << this->getsymbol() << "), enter your move (column 0-" << columns - 1 << "): ";
        cin >> y;
        if (y < 0 || y >= columns) {
            cout << "Invalid column. Please choose a column between 0 and " << columns - 1 << ".\n";
            continue;
        }
        if (connectFourBoard->is_column_full(y)) {
            cout << "Column " << y << " is full. Please choose a different column.\n";
            continue;
        }
        x = 0; 
        break;  
    }
}
};
class randomPlayer : public Player<char> {
public:
    randomPlayer(string name ,char symbol)
        : Player<char>(name , symbol) {
        srand(static_cast<unsigned int>(time(0)));
    }

    void getmove(int& x, int& y) override {
    auto* connectFourBoard = dynamic_cast<ConnectFourBoard*>(this->boardPtr);
    int columns = connectFourBoard->getBoardColumns();
    while (true) {
        x = 0; 
        y = rand() % columns;
        if (connectFourBoard->is_column_full(y)) {
            continue;  
        }

        cout << name <<" placed " << this->getsymbol() << " at column " << y << "\n";
        break;  
    }
}


};

int main() {
    ConnectFourBoard board(6, 7);
    Player<char>* players[2];

    cout << "---- Select Player 1 ----\n";
    cout << "1. Human Player\n";
    cout << "2. Random Player\n";
    cout << "Enter your choice: ";
    int choice1;
    cin >> choice1;

    if (choice1 == 1) {
        string player1Name;
        cout << "Enter Player 1 Name: ";
        cin >> player1Name;
        players[0] = new HumanPlayer(player1Name, 'X');
    } else {
        players[0] = new randomPlayer("random player1",'X');
        cout << "Player 1 is set as Random Player.\n";
    }

    cout << "---- Select Player 2 ----\n";
    cout << "1. Human Player\n";
    cout << "2. Random Player\n";
    cout << "Enter your choice: ";
    int choice2;
    cin >> choice2;

    if (choice2 == 1) {
        string player2Name;
        cout << "Enter Player 2 Name: ";
        cin >> player2Name;
        players[1] = new HumanPlayer(player2Name, 'O');
    } else {
        players[1] = new randomPlayer("random player 2" ,'O');
        cout << "Player 2 is set as Random Player.\n";
    }

    players[0]->setBoard(&board);
    players[1]->setBoard(&board);

    GameManager<char> game(&board, players);
    game.run();

    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    return 0;
}
