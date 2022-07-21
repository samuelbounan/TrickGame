#include "alpha_mu.h"

card alpha_mu(Game game, int id, card hand, card *have_not) {
  if (PRINTING >= 5) cout << "ALPHA MU" << endl;

  // hyperparameters
  int n_sample = 10;

  // init
  Algorithm algo(n_sample, id);
  for (int i = 0; i < N_PLAYERS; i++) {
    if (game.team[i] == game.team[game.declarer])
      algo.node_type[i] = 0;
    else
      algo.node_type[i] = 1;
    if (i != id) algo.node_type[i] += 2;
  }
  algo.depth_leaf = 6;
  card w[n_sample][N_PLAYERS];

  // gen worlds and run ab
  for (int i = 0; i < n_sample; i++) {
    if (PRINTING > 4) cout << "sample " << i << endl;
    random_world(w[i], have_not, hand, game);
  }
  if (PRINTING > 4) cout << "run ab " << endl;
  return template_ab<vector<int *>>(game, w, algo);
  // return 0;
}