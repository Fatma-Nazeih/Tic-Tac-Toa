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

class ninexnine_board : public Board<string> {
public:
    vector<vector<string>> smallGridWinners;
    int total_moves = 0;

    ninexnine_board() {
        this->rows = 9;
        this->columns = 9;

        smallGridWinners.resize(3, vector<string>(3, " "));

        board = new string*[rows];
        for (int i = 0; i < rows; ++i) {
            board[i] = new string[columns];
            for (int j = 0; j < columns; ++j) {
                board[i][j] = "[" + to_string(i) + "," + to_string(j) + "]";
            }
        }
    }

    ~ninexnine_board() {
        for (int i = 0; i < rows; ++i) {
            delete[] board[i];
        }
        delete[] board;
    }

    bool update_board(int x, int y, string symbol) override {
        if (x >= 0 && x < rows && y >= 0 && y < columns && board[x][y] == "[" + to_string(x) + "," + to_string(y) + "]") {
            board[x][y] = symbol;
            total_moves ++;
            if (checkSmallGridWin(x / 3, y / 3)) {
                for (int i = x / 3 * 3; i < (x / 3 + 1) * 3; ++i) {
                    for (int j = y / 3 * 3; j < (y / 3 + 1) * 3; ++j) {
                        if (board[i][j] == "[" + to_string(i) + "," + to_string(j) + "]"){
                            board [i][j] = symbol;
                            total_moves ++;
                        }
                        else{
                            board [i][j] = symbol;
                        }
                    }
                }
                smallGridWinners[x / 3][y / 3] = symbol;
                cout << "Player (" << symbol << ") wins the smaller grid!" << endl;
            } else if (isGridFull( x / 3, y / 3)) {
                cout << "Small grid (" << x / 3 << ", " << y / 3 << ") is full!" << endl;
            }
            return true;
        }
        return false;
    }

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

    bool isValidMove(int row, int col) {
        return row >= 0 && row < 9 && col >= 0 && col < 9 &&
               (board[row][col] == "[" + to_string(row) + "," + to_string(col) + "]");
    }
    
    bool isGridFull( int gridRow, int gridCol) {
        for (int i = gridRow * 3; i < (gridRow + 1) * 3; i++) {
            for (int j = gridCol * 3; j < (gridCol + 1) * 3; j++) {
                if (board[i][j] == "[" + to_string(i) + "," + to_string(j) + "]") {
                    return false;
                }
            }
        }
        return true;
    }

    bool checkSmallGridWin(int gridRow, int gridCol) {
        int startRow = gridRow * 3;
        int startCol = gridCol * 3;

        for (int i = 0; i < 3; i++) {
            if (board[startRow + i][startCol] == board[startRow + i][startCol + 1] &&
                board[startRow + i][startCol] == board[startRow + i][startCol + 2] &&
                board[startRow + i][startCol] != "[" + to_string(startRow + i) + "," + to_string(startCol) + "]") {
                return true;
            }

            if (board[startRow][startCol + i] == board[startRow + 1][startCol + i] &&
                board[startRow][startCol + i] == board[startRow + 2][startCol + i] &&
                board[startRow][startCol + i] != "[" + to_string(startRow) + "," + to_string(startCol + i) + "]") {
                return true;
            }
        }

        if (board[startRow][startCol] == board[startRow + 1][startCol + 1] &&
            board[startRow][startCol] == board[startRow + 2][startCol + 2] &&
            board[startRow][startCol] != "[" + to_string(startRow) + "," + to_string(startCol) + "]") {
            return true;
        }

        if (board[startRow + 2][startCol] == board[startRow + 1][startCol + 1] &&
            board[startRow + 2][startCol] == board[startRow][startCol + 2] &&
            board[startRow + 2][startCol] != "[" + to_string(startRow + 2) + "," + to_string(startCol) + "]") {
            return true;
        }

        return false;
    }

    bool checkOverallBoardWin(string symbol) {
        for (int i = 0; i < 3; i++) {
            if (smallGridWinners[i][0] != " " && smallGridWinners[i][0] == symbol && smallGridWinners[i][1] == symbol && smallGridWinners[i][2] == symbol) {
                return true;
            }

            if (smallGridWinners[0][i] != " " && smallGridWinners[0][i] == symbol && smallGridWinners[1][i] == symbol  &&  smallGridWinners[2][i] == symbol) {
                return true;
            }
        }

        if (smallGridWinners[0][0] != " " && smallGridWinners[0][0] == symbol && smallGridWinners[1][1] == symbol && smallGridWinners[2][2] == symbol) {
            return true;
        }

        if (smallGridWinners[0][2] != " " && smallGridWinners[0][2] == symbol && smallGridWinners[1][1] == symbol && smallGridWinners[2][0] == symbol) {
            return true;
        }

        return false;
    }

    bool is_win() override {
        if (checkOverallBoardWin("X") || checkOverallBoardWin("O")) {
            display_board();
            return true;
        }
        return false;
    }

    bool is_draw() override {
        return total_moves == 81;
    }

    bool game_is_over() override {
        return is_win() || is_draw();
    }
};
class current_misere_player : public Player<string> {
private:
    ninexnine_board* boardPtr;
public:
    current_misere_player(string n, string s, ninexnine_board* b) : Player(n, s), boardPtr(b) {}

    void getmove(int& x, int& y) override {
        while (true) {
            cout << "Player (" << getsymbol() << ") Enter the row and column (0-8) where you want to play: ";
            cin >> x >> y;

            if (cin.fail() || !boardPtr->isValidMove(x, y)) {
                cout << "Invalid move. Please try again.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else {
                break;
            }
        }
    }
};
class random_misere_player : public Player<string> {
private:
    ninexnine_board* boardPtr;
public:
    random_misere_player(string n, string s, ninexnine_board* b) : Player(n, s), boardPtr(b) {}

    void getmove(int& x, int& y) override {
        std::this_thread::sleep_for(std::chrono::seconds(0));

        do {
            x = rand() % 9;
            y = rand() % 9;
        } while (!boardPtr->isValidMove(x, y));
        cout << getname() << " chose: (" << x << "," << y << ")" << endl;
    }
};
void menu_game_6() {
    cout << "Welcome to our 9x9 Tic Tac Toe game!" << endl;

    srand(time(0));
    ninexnine_board* board = new ninexnine_board();

    Player<string>* player1;
    Player<string>* player2;

    string player1Type, player2Type;

    cout << "Is Player 1 (X) a Human or a Computer Random? " << endl;
    cout << "1. Human 1 (X)" << endl;
    cout << "2. Computer Random 1 (X)" << endl;
    cout << "Please enter a choice 1 or 2: " << endl;
    cin >> player1Type;
    if (player1Type == "1") {
        player1 = new current_misere_player("Human 1 (X) ", "X", board);
    } else {
        player1 = new random_misere_player("Computer Random 1 (X) ", "X", board);
    }

    cout << "Is Player 2 (O) a Human or a Computer Random? " << endl;
    cout << "1. Human 2 (O)" << endl;
    cout << "2. Computer Random 2 (O)" << endl;
    cout << "Please enter a choice 1 or 2: " << endl;
    cin >> player2Type;
    if (player2Type == "1") {
        player2 = new current_misere_player("Human 2 (O) ", "O", board);
    } else {
        player2 = new random_misere_player("Computer Random 2 (O) ", "O", board);
    }

    Player<string>* players[2] = {player1, player2};

    GameManager<string> gameManager(board, players);
    gameManager.run();

    delete board;
    delete player1;
    delete player2;
}

int main() {
    menu_game_6();
    return 0;
}
