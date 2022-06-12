// 4-player card program using expert knowledge, expert games, belief states, and-or search
// (c) 2022, Stefan Edelkamp 

#include <iostream>
#include <bit>
#include <bitset>
#include "util.h"
#include "cards.h"

using namespace std;

int lzcnt_128 (unsigned __int128 u) {
  unsigned long long hi = u>>64;
  unsigned long long lo = u;
  if (hi == 0) return 64 + __builtin_clzll(lo);
  return __builtin_clzll(hi);
}


int tzcnt_128 (unsigned __int128 u) {
  unsigned long long hi = u>>64;
  unsigned long long lo = u;
  if (lo == 0) return 64 + __builtin_ctzll(hi);
  return __builtin_ctzll(lo);
}

int popcnt_128 (unsigned __int128 u) {
  unsigned long long hi = u>>64;
  unsigned long long lo = u;
  return __builtin_popcountll(hi) + __builtin_popcountll(lo);
}

void print_128 (unsigned __int128 u) {
  unsigned long long hi = u>>64;
  unsigned long long lo = u;
  std::cout << std::bitset<64>(hi) << std::bitset<64>(lo);
}

int sum(card hand, card trumpmask) {
  int s = 0;
  while (hand) {
    int index = CTZ(hand);
    s += VALUE(index);
    hand &= ~(ONE << index);
  }
  return s;
}

void print_vector(card hand, card trumpmask) {
   card oldhand = hand;
#ifdef BELOTE
  while (hand) {
    int index = CTZ(hand);
    if ((ONE<<index) & trumpmask & JACKS) 
      std::cout << card_name[index] << " ";
    hand &= ~(ONE << index);
  }
  hand = oldhand & trumpmask & ~JACKS;
  while (hand) {
    int index = CTZ(hand);
    if ((ONE<<index) & trumpmask & NINES) 
      std::cout << card_name[index] << " ";
    hand &= ~(ONE << index);
  }
  hand = oldhand & trumpmask & ~JACKS & ~NINES;
#endif
  //  cout << "h:"; print_128(hand);
  while (hand) {
    int index = CTZ(hand);
    if ((ONE<<index) & trumpmask)
      std::cout << "|" << card_name[index] << "| ";
    hand &= ~(ONE << index);
  }
  for (int suit=0;suit<SUITCOUNT;suit++) {
    hand = oldhand & mask[suit];
    
    //    cout << "m:"; print_128(mask[suit]); cout << endl;
    while (hand) { 
      int index = CTZ(hand);
      if (((ONE<<index) & trumpmask) == 0)
	std::cout << "| " << card_name[index] << "| ";
      hand &= ~(ONE << index);
    }
  }
}

void print_extvector(card hand, card trumpmask) {
  card oldhand = hand;
#ifdef BELOTE
  while (hand) {
    int index = CTZ(hand);
    if (((ONE<<index) & trumpmask & JACKS) != 0) 
      std::cout << card_name[index] << "[" << index << "]" << " ";
    hand &= ~(ONE << index);
  }
  hand = oldhand & trumpmask & ~JACKS;
  while (hand) {
    int index = CTZ(hand);
    if (((ONE<<index) & trumpmask & NINES) != 0) 
      std::cout << card_name[index] << "[" << index << "]" << " ";
    hand &= ~(ONE << index);
  }
  hand = oldhand & trumpmask & ~NINES & ~JACKS;
#endif
  while (hand) {
    int index = CTZ(hand);
    if (((ONE<<index) & trumpmask) != 0)
      std::cout << card_name[index] << "[" << index << "]" << " ";
    hand &= ~(ONE << index);
  }
  for (int suit=0;suit<4;suit++) {
    hand = oldhand & mask[suit];  
    while (hand) { 
      int index = CTZ(hand);
      if (((ONE<<index) & trumpmask) == 0)
	std::cout << card_name[index] << "[" << index << "]" << " ";
      hand &= ~(ONE << index);
    }
  }
}

int count(card player) {
  return POPCOUNT(player);
}

/*
int count(card player, int suit) {
  return POPCOUNT(player & mask[suit]);
}
*/



