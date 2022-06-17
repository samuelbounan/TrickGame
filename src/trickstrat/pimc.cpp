#include "pimc.h"

card pimc(Game game, int id, card hand, card *have_not) {
  int n_sample = 5;
  card rd_world[N_PLAYERS];
  card opt_cards;
  int res[N_CARDS] = {0};
  for (int i = 0; i < n_sample; i++) {
    random_world(rd_world, have_not, game);
    opt_cards = alpha_beta(game, id, hand, rd_world);
    while (opt_cards) {
      int c = __builtin_ctzll(opt_cards);
      res[c]++;
      opt_cards &= ~(ONE << c);
    }
  }
  int c = distance(res, max_element(res, res + N_CARDS));
  return (ONE << c);
}