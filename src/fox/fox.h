// 4-player card playing program using expert knowledge, expert games, belief states, and-or search
// (c) 2022, Stefan Edelkamp

#ifndef __CARDAI_H
#define __CARDAI_H

using namespace std;

#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>

#include "cards.h"
#include "util.h"
#include "aos.h"
#include "suitplay.h"

#ifndef FROMFILE
// Base Class
#include "../ai.h"
#else
#include "base.h"
#endif

  
class CardAI : public AI {

public:
  CardAI(); // constructor
  
  //  int p0, p1, p2, p3;    // server play, who is vh in game
  //  int gamenr;            // ID of game for logging
  //  int player1id;         // table id of player1 in first game at table
  //  int player2id;         // table id of player2 in first game at table
  //  int player3id;         // table id of player3 in first game at table
  //  int player4id;         // table id of player3 in first game at table

  int trickround;  // trick round
  int tricknumber;  //  position in trick
  int trickcard[PLAYER];
  
  int id;                // game id of current game for console output
  card myhand = 0;       // current hand cards
  card myskat = 0;       // skat/dog cards if there
  int mypos = 0;         // position at table
  int mybid = 0;         // current bid in bidding process, pushed 
  int mystrength = 0;    // bidding strength computation
  Play* myplay = 0;      // virtual class for trick-game play 
  int mygame;            // game being playerd, as number 9,10,11,12,24,23..59

  bool declarer = false; // flag if player is declarer
  int position;          // position given to player
  Play *player[PLAYER];
  
  long long countrec; // computing how many possible hands are left, global for recursive function
  int tricks[DECKSIZE]; // all cards being played
  int equiv[DECKSIZE][DECKSIZE]; // all cards being played
  int tsize; // cards being played

  OpenCard* AOS; // the and-or search implementation
  GamePlay g; // all information about the game

  card trumpmask = 0; // bitmask for trumpgames

 public:

  virtual void StartGame(int pos, card hand);
  virtual int PlayBid(int bidval);
  virtual void SetTeams(int* teams, int* bids);
  virtual void DeclareGame(card skat, GameDeclaration *gameDeclaration);
  virtual void SetGame(GameDeclaration *gameDeclaration);
  virtual void CardPlayed(int pos, card c);
  virtual card PlayCard(void);
  
  int choose(int cards[PLAYER],int start); // select next card among three on table

 // nine possible options of playing a card P0/1/2 VH/MH/HH

  void mersenne(int seed);
  void take_skat(card& hand, card& skat);  
  void inittrickgame(card hand[PLAYER], int start); // driver for db play
  int playtrickgame(card hand[PLAYER], int start); // driver for db play
  void rotate_pos(card hand[PLAYER], int& start, int pos); // moving AS position 0
  //  void read_samples(char* filename); // read human game from file (db play)
};  // end of class 

#endif


