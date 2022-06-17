#include "rules.h"

void print_bid(list<int> bids) {
  for (auto b : bids) cout << bidname[b] << ";";
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
        card c = unsort(i, trump);
        cout << cardname[__builtin_ctzll(c)] << ";";
        hand &= ~i;
      }
    }
  cout << endl;
}

void print_trick(list<card> trick, card trump) {
  for (auto i : trick) {
    cout << cardname[__builtin_ctzll(unsort(i, trump))] << ";";
  }
}