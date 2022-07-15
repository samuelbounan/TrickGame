#include "voting.h"

card voting(Game game, int id, card hand, card *have_not) {
  if (PRINTING >= 5) cout << "VOTING" << endl;

  // hyperparameters
  int n_sample = 1;
  int depth_ab = 33;
  int n_sample_ab = 3;

  // for each card gen a random world and add one vote for the best card
  card rd_world[N_PLAYERS];
  card opt_cards;
  int res[N_CARDS] = {0};
  for (int i = 0; i < n_sample; i++) {
    if (PRINTING >= 5) cout << "sample " << i << endl;
    random_world(rd_world, have_not, hand, game);
    if (PRINTING >= 5) cout << "run ab" << endl;
    opt_cards = alpha_beta(game, id, hand, rd_world, depth_ab, n_sample_ab);
    while (opt_cards) {
      int c = CTZ(opt_cards);
      res[c]++;
      opt_cards &= ~(ONE << c);
    }
  }
  int c = distance(res, max_element(res, res + N_CARDS));
  return (ONE << c);
}