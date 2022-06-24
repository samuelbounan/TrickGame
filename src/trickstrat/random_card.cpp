#include "random_card.h"

card random_card(Game game, int id, card hand, card *have_not) {
  if (PRINTING >= 5) cout << "RANDOM CARD" << endl << endl;
  card plb = playable(hand, game);
  int idx = mt() % __builtin_popcount(plb);
  for (int i = 0; i < idx; i++) plb &= ~_card(__builtin_ctzll(plb));
  return _card(__builtin_ctzll(plb));
}