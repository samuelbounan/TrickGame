#include "random_card.h"

card random_card(Game game, int id, card hand, card *have_not) {
  list<card> res = deal(playable(hand, game), {1});
  return res.front();
}