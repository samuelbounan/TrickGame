#include "player.h"

int Player::playBid(Game game) { return bidstrat(game, id, hand, have_not); }

card Player::playCard(Game game) {
  card c = strategy(game, id, hand, have_not);
  hand &= ~c;
  return c;
}

void Player::updateBelief(Game game, card c) {
  for (int p = 0; p < N_PLAYERS; p++) have_not[p] |= c;
  for (card cand : set_cards(~have_not[game.turn]))
    if ((c & playable(c | cand, game)) == 0) have_not[game.turn] |= cand;
}