#include "alpha_mu.h"

card alpha_mu(Game game, int id, card hand, card *have_not) {
  if (PRINTING >= 5) cout << "ALPHA MU" << endl;

  // hyperparameters
  int n_sample = 5;

  // init
  int node_type[N_PLAYERS];
  for (int i = 0; i < N_PLAYERS; i++) node_type[i] = i % 2 + 2;
  node_type[id] = id % 2;
  Algorithm algo(node_type, 0, n_sample);
  card w[n_sample][N_PLAYERS];

  // gen worlds and run ab
  for (int i = 0; i < n_sample; i++) {
    if (PRINTING > 4) cout << "sample " << i << endl;
    random_world(w[i], have_not, hand, game);
  }
  if (PRINTING > 4) cout << "run ab " << endl;
  return xav<vector<int *>>(game, w, algo);
  // return 0;
}