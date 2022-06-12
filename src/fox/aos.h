// Bridge/Belote playing program using expert knowledge, expert games, belief states, and-or search
// (c) 2022, Stefan Edelkamp

#ifndef __AOS_H
#define __AOS_H

#include <vector>
#include <list>
#include <algorithm>
#include <iostream>
#include <unordered_map>

#include "cards.h"
#include "game.h"

// hash tables for lookup, often called transposition tables

#ifdef TAROT
typedef std::unordered_map<unsigned long long,int> umap;
#else 
typedef std::unordered_map<card,int> umap;
#endif

// main class for and-or play

class OpenCard {

  GamePlay g;
  umap ht;
  int i[PLAYER]; // cards on table
  int decl_points, opp_points; // score player and opponent
  int ao_depth; // reached depth for finding proposing card
  int max_depth; // max-depth  of search
  card played, hands[PLAYER]; // current state of game
  int limit; // current game contract in trump game
  card trumpmask; // filter for trumpgame
  int order[PLAYER]; // chaning order in search tree
  int sumvalues;
  int team[PLAYER];
  int full;
  
 public:

  OpenCard(GamePlay g);
  ~OpenCard();
  
  int proposed_card; // one (!) card to be played
  card proposed_set; // all (!) cards to be played
  int proposed; // flag if card has been found
  long long generations; // count number of calls for statistics

  void clear_ht(); // full erase of lookup table entries
  
  void table_add(card state, const int best); // insert into transposition table
  void table_add(card state); // insert into transposition table
  int table_lookup(card state, int as);  // searching transposition table
  int winner(int North, int West, int South, int East); // determine winner of three cards on table

  // different choices of next card, mainly used for move ordering
  
  int select(card hand, int player);
  int issuer(int player);  
  
  int AND1(card playable); // P0 move in trump game
  int OR1(card playable); // P1 move in trump game
  int AND2(card playable); // P2 move in trump game
  int OR2(card playable); // P3 move in trump game

  // binary search for finding game value in open-card solver, also known as moving-threshold search
  int solve(GamePlay g, card hand0, card hand1, card hand2, card hand3, card play, int P0, int P1, int P2, int P3, int score_as, int score_gs, int start); 

  // interface status functions for players
  
  int run(GamePlay g, card hand0, card hand1, card hand2, card hand3, card play, int P0, int P1, int P2, int P3, int score_as, int score_gs, int start);
  int fullrun(GamePlay g, card hand0, card hand1, card hand2, card hand3, card play, int P0, int P1, int P2, int P3, int score_as, int score_gs, int start);
  int execute(GamePlay g, card hand0, card hand1, card hand2, card hand3, card play, int P0, int P1, int P2, int P3, int score_as, int score_gs, int start, int testcard); 
  int cardontable(int player);
  void print_hands(card p0, card p1, card p2, card p3, int start, card tm);
};

#endif










