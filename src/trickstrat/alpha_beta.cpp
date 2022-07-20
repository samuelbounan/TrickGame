#include "alpha_beta.h"

card alpha_beta(Game game, int id, card hand, card* have_not) {
  Algorithm algo(1, id);
  for (int i = 0; i < N_PLAYERS; i++)
    if (game.team[i] == game.team[game.declarer])
      algo.node_type[i] = 0;
    else
      algo.node_type[i] = 1;
  card world[1][N_PLAYERS];
  for (int i = 0; i < N_PLAYERS; i++) world[0][i] = ~have_not[i];
  return template_ab<int>(game, world, algo);
  return 0;
}