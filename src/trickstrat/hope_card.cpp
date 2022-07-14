#include "hope_card.h"

card hope_card(Game game, int id, card hand, card *have_not) {
  if (PRINTING >= 5) cout << "HOPE CARD" << endl;

  // hyperparameters
  int n_sample = 10;

  // for each card gen a random world and add one vote for the best card
  card playb = legal(hand, game);
  int n_playb = POPCOUNT(playb);
  int won[n_playb];
  card outcome[n_playb];

  card rd_world[N_PLAYERS];
  for (int i = 0; i < n_sample; i++) {
    if (PRINTING >= 5) cout << "sample " << i << ": ";
    random_world(rd_world, have_not, hand, game);
    card poss = playb;
    int k = 0;
    while (poss) {
      card c = ONE << CTZ(poss);
      if (i == 0) outcome[k] = c;
      auto info = update_card(&game, c);
      if (PRINTING >= 5) cout << "running aos " << endl;
      if (aos(&game, rd_world, MAX_SCORE / 2)) won[k] += 1;
      game.removeCard(info);
      poss &= ~c;
      k++;
    }
  }

  if (PRINTING > 4) {
    for (int i = 0; i < n_playb; i++) {
    }
  }

  int k = distance(won, max_element(won, won + n_playb));
  return outcome[k];
}