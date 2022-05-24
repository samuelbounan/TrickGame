#include "rules.h"

void print_bid(list<int> bids) {
  for (auto b : bids) cout << bidname[b] << ";";
  cout << endl;
}

void print_card(card hand, card trump) {
  if (hand == 0) {
    cout << "\u2205" << endl;
    return;
  }
  card sep_masks[2] = {trump, ~trump};
  for (card mask : sep_masks)
    for (card suit : suits) {
      list<card> to_print = set_cards(hand & mask & suit);
      to_print.reverse();  // for well oredered printing
      for (card i : to_print) {
        unsort(&i, trump);
        cout << cardname[__builtin_ctzll(i)] << ";";
        hand &= ~i;
      }
    }
  cout << endl;
}

void print_trick(list<card> trick, card trump) {
  for (auto i : trick) {
    unsort(&i, trump);
    cout << cardname[__builtin_ctzll(i)] << ";";
  }
  cout << endl;
}