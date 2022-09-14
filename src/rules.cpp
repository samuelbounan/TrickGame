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
        card ui = unsort(i, trump);  // SE
        int cards_per_suit = N_CARDS / N_SUITS;
        int idx = CTZ(ui);
        int x = cards_per_suit * (idx / cards_per_suit) + (cards_per_suit - 1 - idx % cards_per_suit);
        cout << card_name[x] << " ";
        hand &= ~ui;
      }
    }
  cout << endl;
}

void print_trick(card trick[N_PLAYERS], const card &trump) {
  for (int i = 0; i < N_PLAYERS; i++) {
    cout << cardname[CTZ(unsort(trick[i], trump))] << ";";
  }
}
