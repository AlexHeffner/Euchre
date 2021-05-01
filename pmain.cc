/**
 *   @file: main.cc
 * @author: Alex Heffner
 *   @date: 4/11/2021
 *  @brief: parallel Main used for evaluation of a euchre game
 **/

#include <iostream>
#include "game.h"
#include <omp.h>

int main(){
  double MteamP0P2H0H0 = 0;
  double MteamP1P3H0H0 = 0;
  double MteamP0P2H1H0 = 0;
  double MteamP1P3H1H0 = 0;
  double MteamP0P2H1H1 = 0;
  double MteamP1P3H1H1 = 0;
  double MteamP0P2H2H0 = 0;
  double MteamP1P3H2H0 = 0;
  double MteamP0P2H2H1 = 0;
  double MteamP1P3H2H1 = 0;
  #pragma omp parallel num_threads(4)
  {
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

    #pragma omp for 
    for(int i = 0; i < 1000; i++){
      Game one;
      one.start();
      one.playtotrump();

      Game tempH0H0;
      tempH0H0 = one;
      tempH0H0.changeHeuristicOfPlayers(0,0,0,0);
      tempH0H0.playtoEnd();
      if(tempH0H0.getWinner() == 0) {
        teamP0P2H0H0++;
      }
      else if(tempH0H0.getWinner() == 1) {
        teamP1P3H0H0++;
      }
      else{
        cout << "Error";
        exit(0);
      }

      Game tempH1H0;
      tempH1H0 = one;
      tempH1H0.changeHeuristicOfPlayers(1,0,1,0);
      tempH1H0.playtoEnd();
      if(tempH1H0.getWinner() == 0) {
        teamP0P2H1H0++;
      }
      else if(tempH1H0.getWinner() == 1) {
        teamP1P3H1H0++;
      }
      else{
        cout << "Error";
        exit(0);
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
      if(tempH2H0.getWinner() == 0) {
        teamP0P2H2H0++;
      }
      else if(tempH2H0.getWinner() == 1){
        teamP1P3H2H0++;
      }
      else{
        cout << "Error";
        exit(0);
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
    #pragma omp critical
    {
      MteamP0P2H0H0 += teamP0P2H0H0;
      MteamP1P3H0H0 += teamP1P3H0H0;
      MteamP0P2H1H0 += teamP0P2H1H0;
      MteamP1P3H1H0 += teamP1P3H1H0;
      MteamP0P2H1H1 += teamP0P2H1H1;
      MteamP1P3H1H1 += teamP1P3H1H1;
      MteamP0P2H2H0 += teamP0P2H2H0;
      MteamP1P3H2H0 += teamP1P3H2H0;
      MteamP0P2H2H1 += teamP0P2H2H1;
      MteamP1P3H2H1 += teamP1P3H2H1;
    }
  }
  cout << "H0H0\n";
  cout << "Player 0 and player 2 wins = " << MteamP0P2H0H0 << endl;
  cout << "Player 1 and player 3 wins = " << MteamP1P3H0H0 << endl;
  cout << "team 1 % of winning = " << MteamP0P2H0H0 / 1000  * 100 <<  "%" << endl;

  cout << "----------------------------------------------------\n\nH1H0 \n";
  cout << "Player 0 and player 2 wins = " << MteamP0P2H1H0 << endl;
  cout << "Player 1 and player 3 wins = " << MteamP1P3H1H0 << endl;
  cout << "team 1 % of winning = " << MteamP0P2H1H0 / 1000  * 100 <<  "%" << endl;

  cout << "----------------------------------------------------\n\nH1H1 \n";
  cout << "Player 0 and player 2 wins = " << MteamP0P2H1H1 << endl;
  cout << "Player 1 and player 3 wins = " << MteamP1P3H1H1 << endl;
  cout << "team 1 % of winning = " << MteamP0P2H1H1 / 1000  * 100 <<  "%" << endl;

  cout << "----------------------------------------------------\n\nH2H0 \n";
  cout << "Player 0 and player 2 wins = " << MteamP0P2H2H0 << endl;
  cout << "Player 1 and player 3 wins = " << MteamP1P3H2H0 << endl;
  cout << "team 1 % of winning = " << MteamP0P2H2H0 / 1000  * 100 <<  "%" << endl;

  cout << "----------------------------------------------------\n\nH2H1 \n";
  cout << "Player 0 and player 2 wins = " << MteamP0P2H2H1 << endl;
  cout << "Player 1 and player 3 wins = " << MteamP1P3H2H1 << endl;
  cout << "team 1 % of winning = " << MteamP0P2H2H1 / 1000  * 100 <<  "%" << endl;

  return 0;
}

/**

Using 1000 games

H0H0
Player 0 and player 2 wins = 501
Player 1 and player 3 wins = 499
team 1 % of winning = 50.1%
----------------------------------------------------

H1H0
Player 0 and player 2 wins = 587
Player 1 and player 3 wins = 413
team 1 % of winning = 58.7%
----------------------------------------------------

H1H1
Player 0 and player 2 wins = 482
Player 1 and player 3 wins = 518
team 1 % of winning = 48.2%
----------------------------------------------------

H2H0
Player 0 and player 2 wins = 648
Player 1 and player 3 wins = 352
team 1 % of winning = 64.8%
----------------------------------------------------

H2H1
Player 0 and player 2 wins = 620
Player 1 and player 3 wins = 380
team 1 % of winning = 62%

real    5m5.415s
user    13m27.906s
sys     2m49.531s


 */

/**
/mnt/d/AlexHeffner/Documents/3Junior/JSem2/CS4900AI/Euchre$ make pmain
g++ -Wall  -o pmain  -fopenmp pmain.cc game.cc
/mnt/d/AlexHeffner/Documents/3Junior/JSem2/CS4900AI/Euchre$ time ./pmain
H0H0
Player 0 and player 2 wins = 5087
Player 1 and player 3 wins = 4913
team 1 % of winning = 50.87%
----------------------------------------------------

H1H0
Player 0 and player 2 wins = 5848
Player 1 and player 3 wins = 4152
team 1 % of winning = 58.48%
----------------------------------------------------

H1H1
Player 0 and player 2 wins = 4732
Player 1 and player 3 wins = 5268
team 1 % of winning = 47.32%
----------------------------------------------------

H2H0
Player 0 and player 2 wins = 6713
Player 1 and player 3 wins = 3287
team 1 % of winning = 67.13%
----------------------------------------------------

H2H1
Player 0 and player 2 wins = 6189
Player 1 and player 3 wins = 3811
team 1 % of winning = 61.89%

real    54m32.950s
user    141m45.406s
sys     35m39.828s
 * 
 */