/**
 *   @file: main.cc
 * @author: Alex Heffner
 *   @date: 4/11/2021
 *  @brief: Main used to show basic play of the euchre game
 **/
//USE make a golbal variable
#include <iostream>
#include "game.h"
int main(){
  Game one;
  one.start();
  one.playtotrump();
  one.printGame();
  one.playtoEnd();
  if(one.getWinner() == 0){
    cout << "\nplayers 0 and 2 won the round\n";
  }else if(one.getWinner() == 1){
    cout << "\nplayers 1 and 4 won the round\n";
  }
  return 0;
}