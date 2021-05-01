/**
 *   @file: game.h
 * @author: Alex Heffner
 *   @date: 4/11/2021
 *  @brief: Class for a game in euchre
 **/

#ifndef GAME_H
#define GAME_H

#include "card.h"
#include <iostream>
#include <vector>

using namespace std;

class Game{
  public: 
    Game& operator =(const Game& a); //Assingment operator
    //Constructor
    Game( 
      vector<vector<Card>> otherPlayers = {},
      vector<int> othersmartPlayers = {},
      vector<Card> otherKitty = {},
      vector<Card> otherUsedCards = {},
      vector<Card> otherOnTable = {},
      int otherTrickCountP1P3 = 0,
      int otherTrickCountP2P4 = 0,
      int otherTurn = 0,
      int otherDealer = 0,
      int playerWinningTable = -1,
      bool otherHasGameStarted = false,
      possibleSuits turnedDownTrump = noSuit,
      possibleSuits othertrump = noSuit,
      possibleSuits othersuitLead = noSuit,
      int otherwinner = -1,
      int playerLeadTurn = -1
    );

    void changeHeuristicOfPlayers(int P0, int P1, int P2, int P3); // Sets the herusitc of players
    void start(); //initalizes begining
    void playtotrump(); // plays until trump is chosen
    void shuffle1(); // shuffle cards that current player cant see

    void playtoEnd(); // player the game to then end
    int getWinner() {return this->winner;} // returns winner
    void printGame(); // Prints information about the game
    
  private:
    vector<vector<Card>> players; // players
    vector<Card> kitty; // Cards on table 
    vector<Card> usedCards; // Used cards
    vector<Card> onTable; // cards currently on the table
    int trickCountP1P3; // Amount of trick counted between team 1 
    int trickCountP2P4; // Amount of trick counted between team 2 
    int dealer; // which player is dealer
    possibleSuits turnedDownTrump; // if everyone turned down trump    
    bool hasGameStarted; //If the game has started (5 cards for everyone if hasnt started)
    vector<int> smartPlayers; // Number of players using the algorithim // Could be pointer for certain stragies
    int winner; // is there a winner or is the game

    //Durring trun variables
    int turn; //Turn count (whos playing a card next)
    possibleSuits trump; //Current Trump
    possibleSuits suitLead; //current suit lead
    int playerWinningTable; //who is currently winng table
    int playerLeadTurn; //player who leads table

    string printRank[9] = {"none", "nine", "ten", "jack", "Queen", "King", "ace", "L_bower", "R_bower"};
    string printSuit[5] = {"none", "spades", "Clubs", "Hearts", "diamonds"};

    bool evaldealerPickUp(); //eval for having dealer pick
    possibleSuits chooseTrump(); // eval for picking trump
    void dealerpickup(); // having the dealer pick up their card
    void updatebowers();
    // if a > b then return true
    bool cardisGreaterthan(Card a, Card b, possibleSuits currenttrump); // for dealer specificly could be used later
    bool cardisLessthan(Card a, Card b, possibleSuits currenttrump); //used for finding less than card
    
    void playtoEndSimulation(int theBestCard); // moves a move forward and plays out game
    vector<int> legalMoves();//vector of legal cards in a players hand
    int ranklegalMoves(vector<int> legalCards);  // returns int of the best card in someones hand(index)

};
#endif