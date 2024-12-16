#include <iostream>
#include <fstream>
#include <unordered_set>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include "BoardGame_Classes.h" // Assuming this includes the necessary base classes
using namespace std;

class Word_TicTacToe : public Board<char> {
public:
    Word_TicTacToe() {
        this->rows = 3;
        this->columns = 3;

        this->board = new char*[this->rows];
        for (int i = 0; i < this->rows; ++i) {
            this->board[i] = new char[this->columns];
            for (int j = 0; j < this->columns; ++j) {
                this->board[i][j] = ' ';
            }
        }

        load_dictionary();
    }

    ~Word_TicTacToe() {
        for (int i = 0; i < this->rows; ++i) {
            delete[] this->board[i];
        }
        delete[] this->board;
    }

    bool update_board(int x, int y, char symbol) override {
        if (x < 1 || x > 3 || y < 1 || y > 3) return false; // Out of bounds
        if (this->board[x - 1][y - 1] != ' ') return false; // Cell occupied
        this->board[x - 1][y - 1] = symbol;
        return true;
    }

    void display_board() override {
        cout << " " << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << endl;
        cout << "---|---|---" << endl;
        cout << " " << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << endl;
        cout << "---|---|---" << endl;
        cout << " " << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << endl;
    }

    bool is_win() override {
        for (int i = 0; i < this->rows; ++i) {
            for (int j = 0; j < this->columns; ++j) {
                if (check_word(i, j, 1, 0) || check_word(i, j, 0, 1) ||
                    check_word(i, j, 1, 1) || check_word(i, j, 1, -1)) {
                    return true;
                }
            }
        }
        return false;
    }

    bool is_draw() override {
        for (int i = 0; i < this->rows; ++i) {
            for (int j = 0; j < this->columns; ++j) {
                if (this->board[i][j] == ' ') return false;
            }
        }
        return true;
    }

    bool game_is_over() override {
        return is_win() || is_draw();
    }

private:
    unordered_set<string> dictionary;

    void load_dictionary() {
        ifstream file("dic.txt");
        string word;
        while (file >> word) {
            dictionary.insert(word);
        }
    }

    bool check_word(int x, int y, int dx, int dy) {
        string word;
        for (int i = 0; i < 3; ++i) {
            int nx = x + i * dx;
            int ny = y + i * dy;
            if (nx >= 0 && nx < this->rows && ny >= 0 && ny < this->columns) {
                word += this->board[nx][ny];
            } else {
                return false;
            }
        }
        return dictionary.find(word) != dictionary.end();
    }
};

template <typename T>
class Human_Player : public Player<T> {
public:
    Human_Player(string name, T symbol) : Player<T>(name, symbol) {}

    void getmove(int& x, int& y) override {
        char letter;
        do {
            cout << this->getname() << ", enter your move (row, column, letter): ";
            cin >> x >> y >> letter;
            letter = toupper(letter);
        } while (!this->boardPtr->update_board(x, y, letter));
    }
};

template <typename T>
class Random_Player : public Player<T> {
public:
    Random_Player(string name, T symbol) : Player<T>(name, symbol) {
        srand(static_cast<unsigned>(time(nullptr)));
    }

    void getmove(int& x, int& y) override {
        char letter;
        int attempts = 0;
        do {
            x = rand() % 3 + 1; // Random row between 1 and 3
            y = rand() % 3 + 1; // Random column between 1 and 3
            letter = 'A' + rand() % 26; // Random letter from A-Z
            attempts++;
        } while (!this->boardPtr->update_board(x, y, letter) && attempts < 100);

        // Handle edge case if all cells are filled
        if (attempts >= 100) {
            cout << "Random_Player couldn't find a valid move. Game might be in a draw state." << endl;
        }
    }
};

void menu_game_4() {
    cout << "Welcome to Word Tic-tac-toe!" << endl;

    Word_TicTacToe board;

    Player<char>* player1 = nullptr;
    Player<char>* player2 = nullptr;

    string playerName;
    int choice;

    cout << "Select player type for Player 1:" << endl;
    cout << "1. Human Player" << endl;
    cout << "2. Random Player" << endl;
    cin >> choice;
    if (choice == 1) {
        cout << "Enter name for Player 1: ";
        cin >> playerName;
        player1 = new Human_Player<char>(playerName, ' ');
    } else {
        player1 = new Random_Player<char>("RandomPlayer1", 'X');
    }

    cout << "Select player type for Player 2:" << endl;
    cout << "1. Human Player" << endl;
    cout << "2. Random Player" << endl;
    cin >> choice;
    if (choice == 1) {
        cout << "Enter name for Player 2: ";
        cin >> playerName;
        player2 = new Human_Player<char>(playerName, ' ');
    } else {
        // Generate a random symbol for the second random player
        char randomSymbol = 'A' + rand() % 26;
        player2 = new Random_Player<char>("RandomPlayer2", randomSymbol);
    }

    player1->setBoard(&board);
    player2->setBoard(&board);

    bool gameOver = false;
    Player<char>* currentPlayer = player1;

    while (!gameOver) {
        int x, y;
        currentPlayer->getmove(x, y);

        // Display the board after each move
        if (dynamic_cast<Human_Player<char>*>(currentPlayer)) {
            board.display_board();
        } else {
            cout << "Computer's turn:" << endl;
            board.display_board();
        }

        if (board.is_win()) {
            cout << currentPlayer->getname() << " wins!" << endl;
            gameOver = true;
        } else if (board.is_draw()) {
            cout << "The game is a draw!" << endl;
            gameOver = true;
        } else {
            currentPlayer = (currentPlayer == player1) ? player2 : player1;
        }
    }

    delete player1;
    delete player2;
}

int main() {
    menu_game_4();
    return 0;
}