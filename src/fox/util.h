// 4-player card playing program using expert knowledge, expert games, belief states, and-or search
// (c) 2022, Stefan Edelkamp 

#ifndef __UTIL_H
#define __UTIL_H

#include "cards.h"

// pretty printing bitvector sets

void print_vector(card hand, card trumpmask);  // print one hand, sorted according to trump being played
void print_extvector(card hand, card trumpmask);
  
int count(card player); // population count of bitvector, number of cards in set player
//int count(card player, int suit); // same but only counting cards in given suit

int sum(card hand, card trumpmask);
int lzcnt_128 (unsigned __int128 u);
int tzcnt_128 (unsigned __int128 u);
int popcnt_128 (unsigned __int128 u);
void print_128 (unsigned __int128 u);

#endif


