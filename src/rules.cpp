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
      to_print;  // for well oredered printing
      for (card i : to_print) {
        card ui = unsort(i, trump);  // SE
        cout << card_name[__builtin_ctzll(ui)] << " ";
        hand &= ~ui;
      }
    }
  cout << endl;
}

void print_trick(list<card> trick, card trump) {
  for (auto i : trick) {
    card ui = unsort(i, trump);
    cout << card_name[__builtin_ctzll(ui)] << ";";
  }
}
