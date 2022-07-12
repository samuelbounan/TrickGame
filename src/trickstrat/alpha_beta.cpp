#include "alpha_beta.h"

card ab(Game game, int id, card hand, card* have_not) {
  card world[1][N_PLAYERS];
  for (int i = 0; i < N_PLAYERS; i++) world[0][i] = ~have_not[i];
  int node_type[N_PLAYERS] = {0, 1, 0, 1};
  Algorithm algo(node_type, 0, 1);
  return abd<int>(game, world, algo);
}