#include "pimc.h"

card pimc(Game game, int id, card hand, card *have_not) {
  if (PRINTING >= 5) cout << "PIMC" << endl;

  // hyperparameters
  int n_sample = 10;

  // init
  Algorithm algo(n_sample, id);
  for (int i = 0; i < N_PLAYERS; i++)
    if (game.team[i] == game.team[game.declarer])
      algo.node_type[i] = 0;
    else
      algo.node_type[i] = 1;
  card w[n_sample][N_PLAYERS];
  algo.depth_leaf = 1;

  // gen worlds and run ab
  for (int i = 0; i < n_sample; i++) {
    if (PRINTING > 4) cout << "sample " << i << endl;
    random_world(w[i], have_not, hand, game);
  }
  if (PRINTING > 4) cout << "run ab " << endl;
  return template_ab<int *>(game, w, algo);
  // return 0;
}