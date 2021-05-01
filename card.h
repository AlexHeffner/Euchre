/**
 *   @file: card.h
 * @author: Alex Heffner
 *   @date: 4/11/2021
 *  @brief: Class for a card in euchre
 **/

#ifndef CARD_H
#define CARD_H

#include <iostream>

using namespace std;
//possilbe ranks
enum possibleRanks{ 
  none = 0,
  nine = 1,
  ten = 2,
  jack = 3,
  queen = 4,
  king = 5,
  ace = 6,
  bowerLeft = 7,    //Outliers for jacks
  bowerRight = 8
};
//possible suits
enum possibleSuits{
  noSuit,
  spades,
  clubs,
  hearts,
  dimonds
};
//Card is made up of a rank and a suit
struct Card{
  possibleRanks rank;
  possibleSuits suit;

  bool operator ==(const Card& a) const{
    return (rank == a.rank && suit == a.suit);
  }
  Card& operator =(const Card& a){
    rank = a.rank;
    suit = a.suit;
    return *this;
  }
};

#endif