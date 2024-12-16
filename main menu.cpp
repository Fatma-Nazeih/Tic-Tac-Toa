// File name: A2_S13_Task2345_GroupA_20230284_20230292_20230452.zip

// Authors: Nourhan Adel Mohamed El-Hady
//          Caroline Ayman Isaac
//          Fatma Nazeih Hanfy

// Section: S 13
// IDs: 20230452 - 20230292 - 20230284

// Date: 15/15/2024
// Purpose: We have created eight different board games using an existing game framework. This framework makes us
//          develop board games by reusing and extending existing classes. We did not change the original code;
//          instead, we made new classes that inherit from the base classes to define the rules and behaviors
//          for each game.

//The games we developed are:
//1. Pyramid Tic-Tac-Toe : A pyramid-shaped board where players aim to get three in a row.
//2. Four-in-a-Row : Similar to Connect Four, but players mark a 7x6 grid with Xs and Os.
//3. 5x5 Tic Tac Toe : Played on a 5x5 grid, players aim to create the most three-in-a-row sequences.
//4. Word Tic-Tac-Toe : Players use letters to form words on a 3x3 grid.
//5. Numerical Tic-Tac-Toe : Players use numbers to make sums of 15 in a row, column, or diagonal.
//6. Misere Tic Tac Toe : Players avoid getting three marks in a row.
//7. Ultimate Tic Tac Toe : A game within a game, where players win smaller Tic Tac Toe boards within a 3x3 grid.
//8. SUS : Players aim to form the sequence "S-U-S" on a 3x3 grid.
//
//By inheriting from the base classes, we created specific rules and random computer players for each game,
// showcasing how OOP allows us to create new games quickly and efficiently without altering the existing framework.

#include "BoardGame_Classes.h"
#include "OurGamesImplementation.h"

int main(){
    cout << "Welcome to our Game Board!" << endl << endl;

    while (true) {
        cout << "Which game do you want to play?" << endl;
        cout << "1. Game 1 : Pyramid Tic-Tac-Toe" << endl;
        cout << "2. Game 2 : Four-in-a-row" << endl;
        cout << "3. Game 3 : 5 x 5 Tic Tac Toe" << endl;
        cout << "4. Game 4 : Word Tic-tac-toe" << endl;
        cout << "5. Game 5 : Numerical Tic-Tac-Toe" << endl;
        cout << "6. Game 6 : Misere Tic Tac Toe," << endl;
        cout << "7. Game 8 : Ultimate Tic Tac Toe" << endl;
        cout << "8. Game 9 : SUS " << endl;
        cout << "9. Exit" << endl;
        cout <<"Please choose an option (1 - 9): " << endl;

        string choice;
        cin >> choice;

        if (choice == "1") {
            menu_game_1();
            cout << endl << endl;
        }
        else if (choice == "2") {
            menu_game_2();
            cout << endl << endl;
        }
        else if (choice == "3") {
            menu_game_3();
            cout << endl << endl;
        }
        else if (choice == "4") {
            cout << "game4" ;
            cout << endl << endl;
        }
        else if (choice == "5") {
            menu_game_5();
            cout << endl << endl;
        }
        else if (choice == "6") {
            menu_game_6();
            cout << endl << endl;
        }
        else if (choice == "7") {
            menu_game_8();
            cout << endl << endl;
        }
        else if (choice == "8") {
            menu_game_9();
            cout << endl << endl;
        }
        else if (choice == "9") {
            cout << "Exiting the game...";
            return 0;
        }
        else if (choice != "1" && choice != "2" && choice != "3" && choice != "4" && choice != "5" &&
                choice != "6" &&choice != "7" &&choice != "8" &&choice != "9" ){
            cout << "Invalid choice. please try again" << endl << endl;
        }
    }
}