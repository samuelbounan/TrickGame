#include "rules.h"

void print_bid(list<int> bids) {
  for (auto b : bids) cout << bidname[b] << ";";
}

void print_card(card hand, const card &trump) {
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
        card c = unsort(i, trump);
        cout << card_name[CTZ(c)] << ";";
        hand &= ~i;
      }
    }
  cout << endl;
}

void print_trick(card trick[N_PLAYERS], const card &trump) {
  for (int i = 0; i < N_PLAYERS; i++) {
    cout << cardname[CTZ(unsort(trick[i], trump))] << ";";
  }
}
