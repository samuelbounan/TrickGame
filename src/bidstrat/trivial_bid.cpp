#include "trivial_bid.h"

int trivial_bid(Game game, int id, card hand, card *have_not) {
  if (!game.bids.empty()) return 0;
  return (biddable(game.bids).front());
}