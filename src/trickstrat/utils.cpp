#include "utils.h"

void random_world(card *res, card *have_not, Game game) {
try_again:
  copy_n(have_not, N_PLAYERS, res);

  int n_cards[N_PLAYERS];
  for (int i = 0; i < N_PLAYERS; i++) n_cards[i] = SIZE_HAND - game.round;
  for (int p = game.leader; p != game.turn; p = ((p + 1) % N_PLAYERS))
    n_cards[p] -= 1;

  for (int p = 0; p < N_PLAYERS; p++) {
    for (int i = 0; i < p; i++) res[p] |= ~res[i];
    card only = ~res[p];
    for (int i = p + 1; i < N_PLAYERS; i++)
      only &= res[i];  // only is cards that only p can have
    // all cards that can be removed from possible for p
    list<card> poss_list = set_cards(~res[p] & ~only);
    vector<card> poss(poss_list.begin(), poss_list.end());
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(poss.begin(), poss.end(), g);
    auto it = poss.begin();
    unsigned k = 0;
    while ((N_CARDS - __builtin_popcountll(res[p])) != n_cards[p]) {
      if (k >= poss.size()) goto try_again;
      res[p] |= *(it++);
    }
  }
}