/**
 *   @file: main.cc
 * @author: Alex Heffner
 *   @date: 4/11/2021
 *  @brief: seq main used for evaluation of a euchre game
 **/

#include <iostream>
#include "game.h"

int main(){
  double teamP0P2H0H0 = 0;
  double teamP1P3H0H0 = 0;
  double teamP0P2H1H0 = 0;
  double teamP1P3H1H0 = 0;
  double teamP0P2H1H1 = 0;
  double teamP1P3H1H1 = 0;
  double teamP0P2H2H0 = 0;
  double teamP1P3H2H0 = 0;
  double teamP0P2H2H1 = 0;
  double teamP1P3H2H1 = 0;

  for(int i = 0; i < 50; i++){
    Game one;
    one.start();
    one.playtotrump();

    Game tempH0H0;
    tempH0H0 = one;
    tempH0H0.changeHeuristicOfPlayers(0,0,0,0);
    tempH0H0.playtoEnd();
    if(tempH0H0.getWinner() == 0) teamP0P2H0H0++;
    else if(tempH0H0.getWinner() == 1) teamP1P3H0H0++;
    else{
      cout << "Error";
      return 0;
    }

    Game tempH1H0;
    tempH1H0 = one;
    tempH1H0.changeHeuristicOfPlayers(1,0,1,0);
    tempH1H0.playtoEnd();
    if(tempH1H0.getWinner() == 0) teamP0P2H1H0++;
    else if(tempH1H0.getWinner() == 1) teamP1P3H1H0++;
    else{
      cout << "Error";
      return 0;
    }

    Game tempH1H1;
    tempH1H1 = one;
    tempH1H1.changeHeuristicOfPlayers(1,1,1,1);
    tempH1H1.playtoEnd();
    if(tempH1H1.getWinner() == 0) {
      teamP0P2H1H1++;
    }
    else if(tempH1H1.getWinner() == 1) {
      teamP1P3H1H1++;
    }
    else{
      cout << "Error: ";
      cout << "Winner = " << tempH1H1.getWinner() << endl;
      exit(0);
    }
    Game tempH2H0;
    tempH2H0 = one;
    tempH2H0.changeHeuristicOfPlayers(2,0,2,0);
    tempH2H0.playtoEnd();
    if(tempH2H0.getWinner() == 0) teamP0P2H2H0++;
    else if(tempH2H0.getWinner() == 1) teamP1P3H2H0++;
    else{
      cout << "Error";
      return 0;
    }
    Game tempH2H1;
    tempH2H1 = one;
    tempH2H1.changeHeuristicOfPlayers(2,1,2,1);
    tempH2H1.playtoEnd();
    if(tempH2H1.getWinner() == 0) {
      teamP0P2H2H1++;
    }
    else if(tempH2H1.getWinner() == 1){
      teamP1P3H2H1++;
    }
    else{
      cout << "Error";
      exit(0);
    }
  }
  cout << "H0H0\n";
  cout << "Player 0 and player 2 wins = " << teamP0P2H0H0 << endl;
  cout << "Player 1 and player 3 wins = " << teamP1P3H0H0 << endl;
  cout << "team 1 % of winning = " << teamP0P2H0H0 / 50  * 100 <<  "%" << endl;

  cout << "----------------------------------------------------\n\nH1H0 \n";
  cout << "Player 0 and player 2 wins = " << teamP0P2H1H0 << endl;
  cout << "Player 1 and player 3 wins = " << teamP1P3H1H0 << endl;
  cout << "team 1 % of winning = " << teamP0P2H1H0 / 50  * 100 <<  "%" << endl;

  cout << "----------------------------------------------------\n\nH1H1 \n";
  cout << "Player 0 and player 2 wins = " << teamP0P2H1H1 << endl;
  cout << "Player 1 and player 3 wins = " << teamP1P3H1H1 << endl;
  cout << "team 1 % of winning = " << teamP0P2H1H1 / 50  * 100 <<  "%" << endl;

  cout << "----------------------------------------------------\n\nH2H0 \n";
  cout << "Player 0 and player 2 wins = " << teamP0P2H2H0 << endl;
  cout << "Player 1 and player 3 wins = " << teamP1P3H2H0 << endl;
  cout << "team 1 % of winning = " << teamP0P2H2H0 / 50  * 100 <<  "%" << endl;

  cout << "----------------------------------------------------\n\nH2H1 \n";
  cout << "Player 0 and player 2 wins = " << teamP0P2H2H1 << endl;
  cout << "Player 1 and player 3 wins = " << teamP1P3H2H1 << endl;
  cout << "team 1 % of winning = " << teamP0P2H2H1 / 50  * 100 <<  "%" << endl;

  return 0;
}