// 4-player card program using expert knowledge, expert games, belief states, and-or search
// (c) 2022, Stefan Edelkamp 

#ifndef __GAME_H
#define __GAME_H

#include <iostream>
#include <vector>
#include "cards.h"
#include "util.h"

// hash tables for lookup, often called transposition tables
struct World {
  card hands[PLAYER];
  int won;
  World(card h0, card h1, card h2, card h3) {
    hands[0] = h0; hands[1] = h1; hands[2] = h2; hands[3] = h3;
    won = -1; // tbd
  }
  void print() {
    std::cout << "world print ";
    print_vector(hands[0],0); std::cout << "|";
    print_vector(hands[1],0); std::cout << "|";
    print_vector(hands[2],0); std::cout << "|";
    print_vector(hands[3],0); std::cout << " : " << won << std::endl;
  }
};

// the game that is played with all associated information
class GamePlay {

 public:
  int inverse[DECKSIZE];
  int total[DECKSIZE];
  int equiv[DECKSIZE][DECKSIZE];
  card lower[DECKSIZE];
  card higher[DECKSIZE];
  int team[PLAYER];
  card ALLBITS;

  card Mask[SUITCOUNT];
  int Suit[DECKSIZE];

  int preferred_card = -1;
  card skat = 0;
  int limit = -1;   // game being won
  int sumvalues = -1;   // game total eyes
  int posdecl = -1;   // in 1..4 not in 0..3
  int bv[PLAYER]; // exact bidding value 
  int game = -1; // game being played, 9,10,11,12,24,23,35,46,59
  card trumpmask = 0; // filter for bitvector
  int trumpsuit = -1; // value for chosen game [0..3] in suit
  int id = -1; // id of game (db play)
  int humanwon = 0; // flag if human won game
  int humanscore = -1; // score human play has obtainend
    
  // pretty printing bitvector sets
  void print_vector(card hand, card trumpmask);  // print one hand, sorted according to trump being played
  void print_extvector(card hand, card trumpmask);

  int strength(card hand);
  int any(card hand);
  
  int smallest_value(card hand);
  int smallest(card hand, int suit); // compute smallest card in null game for given suit
  int largest(card hand, int suit); // compute largest card in null game for given suit
  int smallest(card hand); // compute smallest card in null game
  int largest(card hand); // compute largest card in null game
  int smallest_trump(card hand); // compute smallest card in null game
  int largest_trump(card hand); // compute largest card in null game
  int small_trump(card hand);  
  int choose_highest_value(card hand); // compute highest-valued non-trump card in suit game
  int choose_lowest_value(card hand); // compute lowest-valued non-trump card in suit game

  int has_largest_trump(card hand, card reduced, card played);
  
  int equivalent_card(card hand, int index, card playednottable);
  // filter cards that are playable wrt. first one in trick

  card playable_cards(card hand, int issuer, card played, card table, int* i); 
  card playable_cards(card hand, int issuer, card played, int* i); 
  card playable_cards(card hand, card played);

  int choose_highest(card hand); // compute highest card in trump, maybe using equivalences
  int choose_lowest(card hand); // lowest card in trump, maybe using equivalences

  card higher_value(int index, card trumpmask);
  card lower_value(int index, card trumpmask);

  int win(int j, int k, int l, int m, card trumpmask);
  int win(int j, int k, int l, card trumpmask);
  int win(int j, int k, card trumpmask);
  int leading(int j, int k, int l, int m, int issuer, card trumpmask);
  int leading(int j, int k, int l, int issuer, card trumpmask);
  int leading(int j, int k, int issuer, card trumpmask);
  int leading(int j, int issuer, card trumpmask);
  
  card fromabovetrump(card hand, card played);
  card fromabovesuit(card hand, card played);

  void print_hands(card p[PLAYER], int start, card tm); // print all 3 hands with game played, sort  
};

#endif



