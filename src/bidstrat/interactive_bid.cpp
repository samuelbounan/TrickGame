#include "interactive_bid.h"

int interactive_bid(Game game, int id, card hand, card *have_not) {
  string blanks = "       ";
  cout << blanks << "HAND: ";
  print_card(hand, game.trump);
  cout << blanks << "      ";

  list<int> bids = biddable(game.bids);

  int res[bids.size() + 1];
  unsigned i = 1;
  list<int> first_print;
  list<int> last_print;

  for (int b : bids) {
    if (i <= 3) {
      cout << "[" << i << "]";
      print_bid({b});
    }
    if (i == 4) cout << "...";
    if (i > 3 && (i >= bids.size() - 2)) {
      cout << "[" << i << "]";
      print_bid({b});
    }
    res[i] = b;
    ++i;
  }
  cout << " > ";
  int ans;
  cin >> ans;

  return res[ans];
}