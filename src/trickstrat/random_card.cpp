#include "random_card.h"

card random_card(Game game, int id, card hand, card *have_not) {
  if (PRINTING >= 5) cout << "RANDOM CARD" << endl << endl;
  card plb = legal(hand, game);
  int idx = mt() % __builtin_popcount(plb);
  for (int i = 0; i < idx; i++) plb &= ~(ONE << (__builtin_ctzll(plb)));
  return ONE << (__builtin_ctzll(plb));
}