/**
 *   @file: game.h
 * @author: Alex Heffner
 *   @date: 4/11/2021
 *  @brief: Class for a game in euchre
 **/
#include "game.h"
#include <iostream>
#include <vector>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <algorithm>    // std::shuffle
#include <random>
#include <chrono>       // std::chrono::system_clock

// http://www.cplusplus.com/reference/algorithm/random_shuffle/
// https://stackoverflow.com/questions/6926433/how-to-shuffle-a-stdvector

using namespace std;

//Assingment operator
Game& Game::operator =(const Game& other){
  this -> players = other.players;
  this -> smartPlayers = other.smartPlayers;
  this -> kitty = other.kitty;
  this -> usedCards = other.usedCards,
  this -> onTable = other.onTable,
  this -> trickCountP1P3 = other.trickCountP1P3; 
  this -> trickCountP2P4 = other.trickCountP2P4;  
  this -> turnedDownTrump = other.turnedDownTrump;
  this -> turn = other.turn; 
  this -> dealer = other.dealer; 
  this -> hasGameStarted = other.hasGameStarted; 
  this -> trump = other.trump; 
  this -> suitLead = other.suitLead; 
  this -> playerWinningTable = other.playerWinningTable;
  this -> playerLeadTurn = other.playerLeadTurn;
  this -> winner = other.winner;
  return *this;
}
//Constructor
Game::Game(
  vector<vector<Card>> otherPlayers,
  vector<int> othersmartPlayers, 
  vector<Card> otherKitty,
  vector<Card> otherUsedCards,
  vector<Card> otherOnTable,
  int otherTrickCountP1P3,
  int otherTrickCountP2P4,
  int otherTurn,
  int otherDealer,
  int otherplayerWinningTable,
  bool otherHasGameStarted,
  possibleSuits otherturnedDownTrump,
  possibleSuits othertrump,
  possibleSuits othersuitLead,
  int otherwinner,
  int otherplayerLeadTurn
  )
  {
    this-> players = otherPlayers;
    this-> smartPlayers = othersmartPlayers;
    //Make size of players 4
    if(this-> players.size() == 0){
      vector<Card> temp;
      for(int i = 0; i < 4; i++){
        players.push_back(temp);
      }
    }
    //Defualt heuristic is 0 for every person
    if(this-> smartPlayers.size() == 0){
      smartPlayers = {0, 0, 0, 0};
    }
    //See header for defualts
    this -> playerWinningTable = otherplayerWinningTable;
    this -> kitty = otherKitty;
    this -> usedCards = otherUsedCards,
    this -> onTable = otherOnTable,
    this -> trickCountP1P3 = otherTrickCountP1P3; 
    this -> trickCountP2P4 = otherTrickCountP2P4;  
    this -> turn = otherTurn; 
    this -> dealer = otherDealer; 
    this -> hasGameStarted = otherHasGameStarted; 
    this -> turnedDownTrump = otherturnedDownTrump;
    this -> trump = othertrump; 
    this -> suitLead = othersuitLead; 
    this -> winner = otherwinner;
    this -> playerLeadTurn = otherplayerLeadTurn;
} 


//Intializes first part of game (shuffles, deals cards, shows face up kitty card, and randomly selectes dealer)
void Game::start(){
  vector<Card> shuffleDeck;
  Card temp;
  possibleRanks tempRank;
  //make all the cards and push into vector
  for (size_t i = 1; i < 7; i++){ //Possible ranks
    tempRank = static_cast<possibleRanks>(i); 
    for (size_t j = 1; j < 5; j++){ //possible suits
      temp.rank = tempRank;
      temp.suit = static_cast<possibleSuits>(j); 
      shuffleDeck.push_back(temp);
    }
  }
  //shuffle vector
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine e(seed);
  std::shuffle(std::begin(shuffleDeck), std::end(shuffleDeck), e);
  
  // make kitty
  for(int i = 0; i < 4; i++){ // make kitty, first card in kitty
    this->kitty.push_back(shuffleDeck.at(shuffleDeck.size() - 1));
    shuffleDeck.pop_back();
  }
  //Deal out each player
  for(int i = 0; i < 5; i++){//for each hand
    for( int j = 0; j < 4; j ++){//for each player
      this->players[j].push_back(shuffleDeck.at(shuffleDeck.size() - 1));
      shuffleDeck.pop_back();
    }
  }
  //randomly select dealer
  srand (time(NULL));
  this -> dealer = rand() % 4; //get dealer
  this -> turn = (this-> dealer + 1) % 4; // get next turn
}

///Plays first part of round where players choose trump
void Game::playtotrump(){
  if(!this->hasGameStarted){ // decided on picking up trump
    if(this->turnedDownTrump == noSuit){ // first time going around
      if(this->evaldealerPickUp()){ // if person tells dealer to pick up
        // ------------------------------------------------------------------------------------------------------------
        cout << "player " << this-> turn << " Told player " << this->dealer << " to pick up " 
        << printRank[kitty.at(kitty.size() - 1).rank] << " of " << printSuit[kitty.at(kitty.size() - 1).suit] << endl;
        cout << printSuit[kitty.at(kitty.size() - 1).suit] << " Is now trump" << endl;
         //---------------------------------------------------------------------------------Used for show information
        this -> trump = kitty.at(kitty.size() - 1).suit;
        this -> dealerpickup();
        this -> updatebowers();
        this -> turn = (this -> dealer + 1) % 4;
        this -> hasGameStarted = true;
      }else{ // pass
        if (this-> dealer == this-> turn){
          this->turnedDownTrump = kitty.at(kitty.size() - 1).suit;
        }
        this -> turn = (this -> turn + 1) % 4;
      }
    }else{ // first card turned down
      if((this->chooseTrump() != noSuit) && (this->turn != this-> dealer)){ // used for choosing trump when not dealer
        cout << "player " << this-> turn << " chose " << printSuit[chooseTrump()] << " as trump" << endl; //------------Uncomment for seeing information
        this -> trump = chooseTrump();
        this -> dealerpickup();
        this-> updatebowers();
        this -> turn = (this -> dealer + 1) % 4;
        this -> hasGameStarted = true;
      }else{
        if(this-> dealer == this-> turn){ // dealer stuck
          cout << "dealer was stuck and chose " << printSuit[chooseTrump()] << " as trump" << endl; //-------------------Uncomment for information
          this -> trump = chooseTrump();
          this -> dealerpickup();
          this-> updatebowers();
          this -> turn = (this -> dealer + 1) % 4;
          this -> hasGameStarted = true;
        }else{ // pass
          this -> turn = (this -> turn + 1) % 4;
        }      
      }
    }
    this->playtotrump();
  }else{
    return;
  }
}

//Can be changed for better AI
//Returns true if the dealer should pick up the card based on current players turn
bool Game::evaldealerPickUp(){
  int trumpCount = 0;
  //find how many trump you would have
  for(size_t i = 0; i < this->players[this->turn].size(); i++){
    if(this->players[this->turn].at(i).suit == kitty.at(kitty.size() - 1).suit){
      trumpCount++;
    }
  }
  //If you have 3 trump and you or your parenter is the dealer pick it up
  if((this->turn == this-> dealer && trumpCount >=3) || (((this->turn + 2) % 4) == this-> dealer && trumpCount >=3)){
    return true;
  }
  // if you have 4 trump tell dealer to pick up trump
  if(trumpCount >= 4){
    return true;
  }
  return false;
}

// called for when the dealer is picking up the trump
void Game::dealerpickup(){ 
  Card temp = this->players[this-> dealer].at(0);
  int worstIndex = 0;
  //find worst card
  for(size_t i = 1; i < this->players[this->dealer].size(); i++){
    if(cardisLessthan(this->players[this->dealer].at(i), temp, this-> trump)){
      temp = this->players[this->dealer].at(i);
      worstIndex = i;
    }
  }
  //swap cards
  temp = kitty.at(kitty.size() - 1);
  kitty.at(kitty.size() - 1) = this->players[this->dealer].at(worstIndex);
  this->players[this->dealer].at(worstIndex) = temp;
}

// Only used in prior trump picking
// Return true if card a is less than card be
// can edit this latter for smarter AI (two suits is better than 3)
bool Game::cardisLessthan(Card a, Card b, possibleSuits currenttrump){
  //check for trump
  if(a.suit == currenttrump && b.suit != currenttrump){
    return false;
  }
  if (a.suit != currenttrump && b.suit == currenttrump){
    return true;
  }else{// check rank otherwise
    if(a.rank >= b.rank){ ///AI HERE
      return false;
    }else{
      return true;
    }
  }
}

//updates the hands and correctly ranks jacks
void Game::updatebowers(){
  // do each player
  for (size_t i = 0; i < this->players.size(); i++){
    for(size_t j = 0; j < this->players[i].size(); j++){
      if(this->players[i][j].rank == jack){
        if(this->players[i][j].suit == this->trump){
          this->players[i][j].rank = bowerRight;
        }
        else{
          if(this->trump == spades && this->players[i][j].suit == clubs){
            this->players[i][j].rank = bowerLeft;
            this->players[i][j].suit = spades;
          }
          else if(this->trump == clubs && this->players[i][j].suit == spades){
            this->players[i][j].rank = bowerLeft;
            this->players[i][j].suit = clubs;
          }
          else if(this->trump == hearts && this->players[i][j].suit == dimonds){
            this->players[i][j].rank = bowerLeft;
            this->players[i][j].suit = hearts;
          }
          else if(this->trump == dimonds && this->players[i][j].suit == hearts){
            this->players[i][j].rank = bowerLeft;
            this->players[i][j].suit = dimonds;
          }
        }
      }
    }
  }
  //update the kitty
  for (size_t i = 0; i < this->kitty.size(); i++){
    if(this->kitty[i].rank == jack){
      if(this->kitty[i].suit == this->trump){
        this->kitty[i].rank = bowerRight;
      }else{
        if(this->trump == spades && this->kitty[i].suit == clubs){
          this->kitty[i].rank = bowerLeft;
        }
        if(this->trump == clubs && this->kitty[i].suit == spades){
          this->kitty[i].rank = bowerLeft;
        }
        if(this->trump == hearts && this->kitty[i].suit == dimonds){
          this->kitty[i].rank = bowerLeft;
        }
        if(this->trump == dimonds && this->kitty[i].suit == hearts){
          this->kitty[i].rank = bowerLeft;
        }
      }
    }
  }
}

// Can be changed for better choseing AI
// Used when the first option for choosing trump is turned down
// Helps choose between 3 suits or no Suit (a pass)
possibleSuits Game::chooseTrump(){
  vector<int> suitPick;
  for(int i = 0; i < 5; i++){
    suitPick.push_back(0);
  }
  if(this-> turn == this-> dealer){ //if the dealer has to pick
    for(size_t i = 0; i < players[this->turn].size(); i++){
      suitPick.at(players[this->turn].at(i).suit)++;
    }
    int max = 0; // just checking for maxium nuber of trumps
    for(size_t i = 1; i < suitPick.size(); i++){
      if(suitPick.at(i) > suitPick.at(max)){
        max = i;
      }
    }
    return static_cast<possibleSuits>(max);
  }
  else{ // other player than dealer
    for(size_t i = 0; i < players[this->turn].size(); i++){
      suitPick.at(players[this->turn].at(i).suit)++;
    }
    int max = 0; // just checking for maxium nuber of trumps
    for(size_t i = 1; i < suitPick.size(); i++){
      if(suitPick.at(i) > suitPick.at(max)){
        max = i;
      }
    }
    if (suitPick.at(max) >= 3){ // AI CAN BE CHANGED HERE
      return static_cast<possibleSuits>(max);
    }else{
      return noSuit; // pass if do dont have 3 trump
    }
  }
}
///---------FUNCTIONCS FOR AFTER SELECTEING TRUMP-------------------------------------------------------------

/// Plays the game to the end of the round
/// Sets a winner
void Game::playtoEnd(){
  if(this->winner != -1){ // this is an error, there must be a winner no ties
    cout << "ERROR: Game over implement other information here\n";
  }else{
    //get best move according to heuristic
    vector<int> legalMoves = this->legalMoves();
    int theBestCard = ranklegalMoves(legalMoves);

    if(this->onTable.size() == 0){ // If you are leading
      this->suitLead = this->players[this->turn][theBestCard].suit;
      //make the move
      this->onTable.push_back(this->players[this->turn][theBestCard]);
      this->players[this->turn].erase(this->players[this->turn].begin() + theBestCard);

      this->playerWinningTable = this->turn;
      this->playerLeadTurn = this->turn;

      this->turn = (this-> turn + 1) % 4; // get next turn
      this->printGame(); ///////  UNCOMMET THIS TO SEE COMPLETE PLay out... ---------------
      this->playtoEnd(); // contiune the game
    }
    else if(this->onTable.size() >= 1 && this->onTable.size() < 3){ // if you are not leading and not last
      //check to see if card is better than any other currently down
      bool isnowbest = true;
      for(size_t i = 0; i < onTable.size(); i++){
        if(!this->cardisGreaterthan(this->players[this->turn][theBestCard], this->onTable.at(i), this->trump)){
          isnowbest = false;
        }
      }
      if(isnowbest){
        this->playerWinningTable = this->turn;
      }
      //make the move
      this->onTable.push_back(this->players[this->turn][theBestCard]);
      this->players[this->turn].erase(this->players[this->turn].begin() + theBestCard);
      this->turn = (this-> turn + 1) % 4; // get next turn
      this->printGame(); ///////  UNCOMMET THIS TO SEE COMPLETE PLay out... ---------------
      this->playtoEnd(); // continue game
    }
    else if(this->onTable.size() == 3){ // if laster person to play
      //Find if you card is gonna beat any others
      bool isnowbest = true;
      for(size_t i = 0; i < onTable.size(); i++){
        if(!this->cardisGreaterthan(this->players[this->turn][theBestCard], this->onTable.at(i), this->trump)){
          isnowbest = false;
        }
      }
      if(isnowbest){
        this->playerWinningTable = this->turn;
      }
      this->onTable.push_back(this->players[this->turn][theBestCard]);
      this->players[this->turn].erase(this->players[this->turn].begin() + theBestCard);

      //update trick amount aka take the trick
      if(this->playerWinningTable == 0 || this->playerWinningTable == 2){
        this->trickCountP1P3++;
      }else{
        this->trickCountP2P4++;
      }
      if(this->players[this->turn].size() != 0){// not done with game
        this->turn = this->playerWinningTable;
        this->playerWinningTable = -1;
        this->suitLead = noSuit;
        this->playerLeadTurn = -1;
        while(this->onTable.size() != 0){
          this->usedCards.push_back(this->onTable[this->onTable.size() -1]);
          this->onTable.pop_back();
        }
        this->printGame(); ///////  UNCOMMET THIS TO SEE COMPLETE PLay out... ---------------
        this->playtoEnd(); // continue
      }else{///GAME OVER
        if(this->trickCountP1P3 > this->trickCountP2P4){
          this->winner = 0;
        }else{
          this->winner = 1;
        }
        this->printGame(); ///////  UNCOMMET THIS TO SEE COMPLETE PLay out... ---------------
        //cout << "winner: " << winner << endl; //Uncommnet this to see winner
        return;
      }
    }else{ // should not get here
      cout << "ERROR to many cards on table\n";
      exit(0);
    }
  }
}

/// Generate leagal moves for next play
vector<int> Game::legalMoves(){
  vector<int> legalCardinex;
  if(this->onTable.size() == 0){ // if you are going first all cards are legal
    for(size_t i = 0; i < this->players[this->turn].size(); i++){
      legalCardinex.push_back(i);
    }
  }else{///if you are not going first
    if(this->suitLead == noSuit){ // some suit must be first
      cout << "ERROR no suit declared";
      exit(0);
    }
    bool haveSuitLead = false;
    //check to see if you have suit
    for(size_t i = 0; i < this->players[this->turn].size(); i++){ // only push same suit back
      if(this->suitLead == this->players[this->turn].at(i).suit){
        haveSuitLead = true;
        legalCardinex.push_back(i);
      }
    }
    if(!haveSuitLead){ //whole hand is playable then
      for(size_t i = 0; i < this->players[this->turn].size(); i++){
        legalCardinex.push_back(i);
      }
    }
  }
  return legalCardinex;
}

/// Can ALWAYS Add to make new strategies AI Improvment
// returns index of legal card that gives the best card to play
int Game::ranklegalMoves(vector<int> legalCards){
  if(this->smartPlayers[this->turn] == 0){ // dumb/drunk player choose random legal
    srand(unsigned (std::time(0)));
    int randomNum = std::rand() % legalCards.size();
    return legalCards[randomNum];
  }
  // This stratgies says if you are currently losing the trick and able to take the trick 
  // you take it
  else if (this->smartPlayers[this->turn] == 1){ 
    //if nobody has played yet
    //play best card
    if(this->playerWinningTable == -1){
      int indexBest = 0;
      for(size_t i = 0; i < legalCards.size(); i++){
        if(this->cardisGreaterthan(this->players[this->turn].at(legalCards.at(i)),
          this->players[this->turn].at(legalCards.at(indexBest)), this->trump)){
            indexBest = i;
        }
      }
      return indexBest;
    }
    else if(this->playerWinningTable == (this->turn + 2) % 4){ //if teamate is winning
        int worstIndex = 0;
        //throw worst card
        for(size_t i = 0; i < legalCards.size(); i++){
          if(!this->cardisGreaterthan(this->players[this->turn].at(legalCards.at(i)),
            this->players[this->turn].at(legalCards.at(worstIndex)), this->trump)){
              worstIndex = i;
          }
        }
      return worstIndex;
    }
    else{ // other team is winning // just plays a winning card
      int winningCard = -1;
      //find a winning card
      for(size_t i = 0; i < legalCards.size(); i++){
        if(this->cardisGreaterthan(this->players[this->turn].at(legalCards.at(i)),
          this->onTable[(this->playerLeadTurn + this->playerWinningTable) % 4], this->trump)){
            winningCard = i;
        }
      }
      // play winning card if you have one
      if(winningCard != -1){
        return winningCard;
      }
      else{ // play worst card
        int worstIndex = 0;
        for(size_t i = 0; i < legalCards.size(); i++){
          if(!this->cardisGreaterthan(this->players[this->turn].at(legalCards.at(i)),
            this->players[this->turn].at(legalCards.at(worstIndex)), this->trump)){
              worstIndex = i;
          }
        }
      return worstIndex;
      }
    }
  }
  //Heuristic of simulating each card play and rolling out rest of game
  else if (this->smartPlayers[this->turn] == 2){ 
    if(this->players[this->turn].size() == 5){//if this is the frist one
      vector<pair<int,int>> legalCardWincounts;
      for(size_t i = 0; i < legalCards.size(); i++){
        pair<int,int> temp = {0,0};
        legalCardWincounts.push_back(temp);
      }
      //shuffle and simulate
      for(int i = 0; i < 1; i++){
        Game temp = *this;
        temp.shuffle1();
        //play each card and simulate
        for(size_t j = 0; j < legalCards.size(); j++){
          Game tempNewPlay = temp;
          tempNewPlay.playtoEndSimulation(legalCards.at(j));
          //count the wins
          if(tempNewPlay.winner == 0) legalCardWincounts.at(j).first++;
          if(tempNewPlay.winner == 1) legalCardWincounts.at(j).second++;
        }
      }
      //find the best win number and play that card
      int bestCardIndex = 0;
      int maxWinCount = 0;
      for(size_t i = 0; i < legalCardWincounts.size(); i++){
        if(this-> turn == 0 || this-> turn == 2){
          if(maxWinCount < legalCardWincounts.at(i).first){
            maxWinCount = legalCardWincounts.at(i).first;
            bestCardIndex = i;
          }
        }
        if (this->turn == 1 || this->turn == 3){
          if(maxWinCount < legalCardWincounts.at(i).second){
            maxWinCount = legalCardWincounts.at(i).second;
            bestCardIndex = i;
          }
        }
      }
      return bestCardIndex;///Should be index in legal cards
    }else{ /// not begining  same but no shuffle
      vector<pair<int,int>> legalCardWincounts;
      for(size_t i = 0; i < legalCards.size(); i++){
        pair<int,int> temp = {0,0};
        legalCardWincounts.push_back(temp);
      }
      //play simulation for each card
      Game temp = *this;
      for(size_t j = 0; j < legalCards.size(); j++){
        Game tempNewPlay = temp;
        tempNewPlay.playtoEndSimulation(legalCards.at(j));
        if(tempNewPlay.winner == 0) legalCardWincounts.at(j).first++;
        if(tempNewPlay.winner == 1) legalCardWincounts.at(j).second++;
      }
      //get best winning number
      int bestCardIndex = 0;
      int maxWinCount = 0;
      for(size_t i = 0; i < legalCardWincounts.size(); i++){
        if(this-> turn == 0 || this-> turn == 2){
          if(maxWinCount < legalCardWincounts.at(i).first){
            maxWinCount = legalCardWincounts.at(i).first;
            bestCardIndex = i;
          }
        }
        if (this->turn == 1 || this->turn == 3){
          if(maxWinCount < legalCardWincounts.at(i).second){
            maxWinCount = legalCardWincounts.at(i).second;
            bestCardIndex = i;
          }
        }
      }
      return bestCardIndex;///Should be index in legal cards
    }
  }else{ // Can alwasy add more stategies ------ AI
    cout << "error not that many stratgies";
    return -1;
  }
}

// Used for comparrions of cards
// returns true if card a is greater than b
// used post trump selection
// Could implement More AI in here
bool Game::cardisGreaterthan(Card a, Card b, possibleSuits currenttrump){
  //comparing trumps
  if(a.suit == currenttrump && b.suit != currenttrump){
    return true;
  }
  if (a.suit != currenttrump && b.suit == currenttrump){
    return false;
  }else{//comparing ranks
    if(a.rank > b.rank){ 
      return true;
    }else if (a.rank < b.rank){
      return false;
    }else{///check for frist suit played
      if(this->suitLead == noSuit){
        return false; //shouldnt matter could add AI
      }
      if(a.suit == this->suitLead){
        return true;
      }
      if(b.suit == this->suitLead){
        return false;
      }else{
        return false;
      }
    }
  }
}

// Helper function used to print information about the game
void Game::printGame(){
  cout << "Dealer: "<< this-> dealer << endl;
  cout << "Trump: "<< this->printSuit[trump] << endl;
  cout << "Player winning table: " << this->playerWinningTable << endl;
  cout << "Table Size: " << this->onTable.size() << endl;
  cout << "On Table:" << endl;
  //cards on the table in order of who layed them down
  for(size_t i = 0; i < this->onTable.size(); i++){  //Prints cards on table
    cout << "\tPlayer " << ((i + this->playerLeadTurn) % 4) << " ";//player
    cout << printRank[this->onTable.at(i).rank] << " " << printSuit[this->onTable.at(i).suit] << endl;
  }
  //Current turn
  cout << endl;
  cout << "Player: " <<  this->turn  << "'s Turn\n\n";
  //players
  for(int j = 0; j < 4; j++){
    cout << "Player " << j << "  ";
    for (size_t i = 0; i < players[j].size(); i++){ // print current players hand
      cout << printRank[players[j].at(i).rank] << " " << printSuit[players[j].at(i).suit] << "\t";
    }
    cout << endl;
  }
  // the kitty
  cout <<"Kitty\t";
  for (size_t i = 0; i < 4; i++){
    cout << printRank[kitty.at(i).rank] << " " << printSuit[kitty.at(i).suit] << "\t";
  }
  cout << "\n-----------------------------------------------------------------------\n";
}

// used to shuffle cards
// shuffles cards that are not seen by current player (everyhting but current
// players hand, cards on table, top kitty card, and used cards)
void Game::shuffle1(){
  vector<Card> tempShuff;
  vector<int> cardCountPerPersonAndKitty = {0,0,0,0,0};
  //Taking cards out
  for (size_t i = 0; i < this->players.size(); i++){
    if(static_cast<int>(i) != this->turn){
      for(size_t j = 0; j < this->players[i].size(); j++){
        tempShuff.push_back(this->players[i][j]);
        cardCountPerPersonAndKitty[i]++;
      }
    }
  }
  //get cards out of kitty
  for(size_t i = 0; i < this->kitty.size() - 1; i++){
    tempShuff.push_back(this->kitty[i]);
    cardCountPerPersonAndKitty[4]++;
  }
  //shuffleing
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine e(seed);
  std::shuffle(std::begin(tempShuff), std::end(tempShuff), e);

  //Putting cards back in
  Card temp;
  for (size_t j = 0; j < this->players.size();j++){
    for(int i = 0; i < cardCountPerPersonAndKitty.at(j); i++){ //player 1
      temp = tempShuff.at(tempShuff.size() - 1);
      tempShuff.pop_back();
      this->players[j][i] = temp;
    }
  }
  for(int i = 0; i < cardCountPerPersonAndKitty.at(4); i++){ // kitty
    temp = tempShuff.at(tempShuff.size() - 1);
    tempShuff.pop_back();
    this->kitty[i] = temp;
  }
}

// Sets the herusitc of players
void Game::changeHeuristicOfPlayers(int P0, int P1, int P2, int P3){
  this->smartPlayers[0] = P0;
  this->smartPlayers[1] = P1;
  this->smartPlayers[2] = P2;
  this->smartPlayers[3] = P3;
  return;
}

//Same as play to end but it takes in the best card rather than recalculating it
void Game::playtoEndSimulation(int theBestCard){
  if(this->winner != -1){ // this is an error, there must be a winner no ties
    cout << "Game over implement other information here\n";
  }else{
    if(this->onTable.size() == 0){// If you are leading
      this->suitLead = this->players[this->turn][theBestCard].suit;
      //make the move
      this->onTable.push_back(this->players[this->turn][theBestCard]);
      this->players[this->turn].erase(this->players[this->turn].begin() + theBestCard);

      this->playerWinningTable = this->turn;
      this->playerLeadTurn = this->turn;

      this->turn = (this-> turn + 1) % 4; // get next turn
      //this->printGame(); ///////  UNCOMMET THIS TO SEE COMPLETE PLay out... ---------------
      this->playtoEnd(); // contiune the game
    }
    else if(this->onTable.size() >= 1 && this->onTable.size() < 3){
      //check to see if card is better than any other currently down
      bool isnowbest = true;
      for(size_t i = 0; i < onTable.size(); i++){
        if(!this->cardisGreaterthan(this->players[this->turn][theBestCard], this->onTable.at(i), this->trump)){
          isnowbest = false;
        }
      }
      if(isnowbest){
        this->playerWinningTable = this->turn;
      }
      //make the move
      this->onTable.push_back(this->players[this->turn][theBestCard]);
      this->players[this->turn].erase(this->players[this->turn].begin() + theBestCard);
      this->turn = (this-> turn + 1) % 4; // get next turn
      //this->printGame(); ///////  UNCOMMET THIS TO SEE COMPLETE PLay out... ---------------
      this->playtoEnd();// contiune the game
    }
    else if(this->onTable.size() == 3){ // if laster person to play
      //Find if you card is gonna beat any others
      bool isnowbest = true;
      for(size_t i = 0; i < onTable.size(); i++){
        if(!this->cardisGreaterthan(this->players[this->turn][theBestCard], this->onTable.at(i), this->trump)){
          isnowbest = false;
        }
      }
      if(isnowbest){
        this->playerWinningTable = this->turn;
      }
      this->onTable.push_back(this->players[this->turn][theBestCard]);
      this->players[this->turn].erase(this->players[this->turn].begin() + theBestCard);

      //update trick amount aka take the trick
      if(this->playerWinningTable == 0 || this->playerWinningTable == 2){
        this->trickCountP1P3++;
      }else{
        this->trickCountP2P4++;
      }
      if(this->players[this->turn].size() != 0){// not done with game
        this->turn = this->playerWinningTable;
        this->playerWinningTable = -1;
        this->suitLead = noSuit;
        this->playerLeadTurn = -1;
        while(this->onTable.size() != 0){
          this->usedCards.push_back(this->onTable[this->onTable.size() -1]);
          this->onTable.pop_back();
        }
        //this->printGame(); ///////  UNCOMMET THIS TO SEE COMPLETE PLay out... ---------------
        this->playtoEnd(); // contiune the game
      }else{///GAME OVER
        if(this->trickCountP1P3 > this->trickCountP2P4){
          this->winner = 0;
        }else{
          this->winner = 1;
        }
        //this->printGame(); ///////  UNCOMMET THIS TO SEE COMPLETE PLay out... ---------------
        //cout << "winner: " << winner << endl; Uncommnet this to see winner
        return;
      }
    }else{ //should not get here
      cout << "ERROR to many cards on table\n";
      exit(0);
    }
  }
}