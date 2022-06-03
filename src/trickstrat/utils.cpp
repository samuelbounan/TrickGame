#include "utils.h"

void random_world(card *have_not, card *res, Game game) {
  copy_n(have_not, N_PLAYERS, res);

  int n_cards[N_PLAYERS];
  for (int i = 0; i < N_PLAYERS; i++) n_cards[i] = SIZE_HAND - game.round;
  for (int p = game.leader; p < game.turn; p = ((p + 1) % N_PLAYERS))
    n_cards[p] -= 1;

  for (int p = 0; p < N_PLAYERS; p++) {
    for (int i = 0; i < p; i++) res[p] |= ~res[i];
    list<card> poss_list = set_cards(~res[p]);
    vector<card> poss(poss_list.begin(), poss_list.end());
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(poss.begin(), poss.end(), g);
    auto it = poss.begin();
    while (n_cards[p] < (N_CARDS - __builtin_popcountll(res[p]))) {
      res[p] |= *it;
      it++;
    }
  }
}