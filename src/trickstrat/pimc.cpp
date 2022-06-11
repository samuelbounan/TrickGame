#include "pimc.h"

card pimc(Game game, int id, card hand, card *have_not) {
  cout << "pimc" << endl;
  if (!game.trick.empty()) return 0;
  int n_sample = 100;
  card rd_world[N_PLAYERS];
  card opt_cards;
  int res[N_CARDS] = {0};
  for (int i = 0; i < n_sample; i++) {
    copy_n(have_not, N_PLAYERS, rd_world);
    random_world(rd_world, game);
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