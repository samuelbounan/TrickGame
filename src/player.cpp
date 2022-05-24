#include "player.h"

card Player::playCard(Game game) {
  card c = strategy(game, id, hand, have_not);
  hand &= ~c;
  return c;
}

void Player::updateBelief(Game game, card c) { have_not[game.turn] |= c; }