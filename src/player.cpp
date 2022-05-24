#include "player.h"

int Player::playBid(Game game) { return bidstrat(game, id, hand, have_not); }

card Player::playCard(Game game) {
  card c = strategy(game, id, hand, have_not);
  hand &= ~c;
  return c;
}

void Player::updateBelief(Game game, card c) { have_not[game.turn] |= c; }