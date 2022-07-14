#include "interactive_card.h"

card interactive_card(Game game, int id, card hand, card *have_not) {
  string blanks = "       ";
  cout << blanks << "HAND: ";
  print_card(hand, game.trump);
  cout << blanks << "      ";

  card legal_cards = legal(hand, game);
  int n_legal = POPCOUNT(legal_cards);
  card res[n_legal] = {0};
  int idx = 1;

  card sep_masks[2] = {game.trump, ~game.trump};
  for (card mask : sep_masks)
    for (card suit : suits) {
      list<card> printed = set_cards(hand & mask & suit);
      printed.reverse();  // for well oredered printing
      for (card i : printed) {
        if (i & legal_cards) {
          res[idx] = i;
          cout << " " << idx << " ";
          idx++;
        } else
          cout << "   ";
      }
    }
  cout << "> ";
  int ans;
  cin >> ans;

  return res[ans];
}