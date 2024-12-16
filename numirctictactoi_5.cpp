#include <iostream>
#include <vector>
#include <algorithm>
#include "BoardGame_classes.h"
#include <cstdlib>
#include <ctime>
using namespace std;

class NumTicTacToeBoard : public Board<int> {
public:
    NumTicTacToeBoard() {
        rows = 3;
        columns = 3;
        board = new int*[rows];
        for (int i = 0; i < rows; i++) {
            board[i] = new int[columns]();
        }
        n_moves = 0;
    }

    bool update_board(int x, int y, int symbol) override {
        if (x >= 0 && x < rows && y >= 0 && y < columns && board[x][y] == 0) {
            board[x][y] = symbol;
            n_moves++;
            return false;
        }
        return true;
    }

    void display_board() override {
    cout << "   0   1   2\n";  
    cout << "  ------------\n";  
    for (int i = 0; i < rows; i++) {
        cout << i << " |";  
        for (int j = 0; j < columns; j++) {
            if (board[i][j] == 0)
                cout << " . |";  
            else
                cout << " " << board[i][j] << " |";  
        }
        cout << endl;
        cout << "  ------------\n";  
    }
}


    bool is_win() override {
        for (int i = 0; i < rows; i++) {
            if (board[i][0] + board[i][1] + board[i][2] == 15 &&
                board[i][0] != 0 && board[i][1] != 0 && board[i][2] != 0) {
                return true;
            }
        }

        for (int j = 0; j < columns; j++) {
            if (board[0][j] + board[1][j] + board[2][j] == 15 &&
                board[0][j] != 0 && board[1][j] != 0 && board[2][j] != 0) {
                return true;
            }
        }
        if ((board[0][0] + board[1][1] + board[2][2] == 15 &&
            board[0][0] != 0 && board[1][1] != 0 && board[2][2] != 0) ||
            (board[0][2] + board[1][1] + board[2][0] == 15 &&
            board[0][2] != 0 && board[1][1] != 0 && board[2][0] != 0)) {
            return true;
        }
        return false;
    }

    bool is_draw() override {
        return n_moves == 9 && !is_win();
    }

    bool game_is_over() override {
        return is_win() || is_draw();
    }
};

class HumanPlayer : public Player<int> {
private:
    vector<int> available_numbers;

public:
    HumanPlayer(string name, int symbol, const vector<int>& numbers)
        : Player<int>(name, symbol), available_numbers(numbers) {}

    void getmove(int& x, int& y) override {
        if (this->boardPtr == nullptr || this->boardPtr->game_is_over()) {
            return;
        }
        int num;
        cout << this->name << ", your available numbers: ";
        for (int n : available_numbers) cout << n << " ";
        cout << endl;
        cout << "Enter the number you want to place: ";
        cin >> num;
        while (find(available_numbers.begin(), available_numbers.end(), num) == available_numbers.end()) {
            cout << "Invalid number. Choose again: ";
            cin >> num;
        }
        available_numbers.erase(remove(available_numbers.begin(), available_numbers.end(), num), available_numbers.end());

        cout << "Enter the row and column to place your number in range ( 0 - 2) , look for the board: ";
        cin >> x >> y;

        while (this->boardPtr->update_board(x, y, num)) {
            cout << "Invalid move. Try again.\n";
            available_numbers.push_back(num);
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

    string getName() const {
        return this->name;  
    }
};

class randomPlayer : public Player<int> {
private:
    vector<int> available_numbers;

public:
    randomPlayer(string name, const vector<int>& numbers)
        : Player<int>(name, symbol), available_numbers(numbers) {
        srand(static_cast<unsigned int>(time(0)));
    }

 void getmove(int& x, int& y) override {
    if (this->boardPtr == nullptr || this->boardPtr->game_is_over()) {
        return;
    }
    int numIndex = rand() % available_numbers.size();
    int num = available_numbers[numIndex];
    bool validMove = false;
    while (!validMove) {
        x = rand() % 3;  
        y = rand() % 3;  
        if (this->boardPtr->update_board(x, y, num) == false) {  
            validMove = true;  
            available_numbers.erase(available_numbers.begin() + numIndex);  
            cout << name <<" "<< "placed " << num << " at (" << x << ", " << y << ")\n";
        }
    }
}


};
int main() {
    NumTicTacToeBoard board;
    vector<int> player1_numbers = {1, 3, 5, 7, 9};
    vector<int> player2_numbers = {2, 4, 6, 8};

    Player<int>* players[2];
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
        players[0] = new HumanPlayer(player1Name, 1 , player1_numbers);
    } else {
        players[0] = new randomPlayer("random player 1", player1_numbers);
        cout << "Player 1 is set as Random Player.\n";
    }
    cout << "-------------------------\n";
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
        players[1] = new HumanPlayer(player2Name, 2 , player2_numbers);
    } else {
        players[1] = new randomPlayer("random player 2", player2_numbers);  
        cout << "Player 2 is set as Random Player.\n";
    }
    cout << "-------------------------\n";
    players[0]->setBoard(&board);
    players[1]->setBoard(&board);
    GameManager<int> game(&board, players);
    game.run(); 
      
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    return 0;
}
