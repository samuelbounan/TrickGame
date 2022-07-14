#include "random_card.h"

card random_card(Game game, int id, card hand, card *have_not) {
  if (PRINTING >= 5) cout << "RANDOM CARD" << endl << endl;
  card plb = legal(hand, game);
  int idx = mt() % POPCOUNT(plb);
  for (int i = 0; i < idx; i++) plb &= ~(ONE << (CTZ(plb)));
  return ONE << (CTZ(plb));
}