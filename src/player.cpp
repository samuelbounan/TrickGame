#include "player.h"

int Player::playBid(Game game) { return bidstrat(game, id, hand, have_not); }

card Player::playCard(const Game &game) {
  card c = trickstrat(game, id, hand, have_not);
  hand &= ~c;
  return c;
}

void Player::updateBelief(const Game &game, const card &c) {
  for (int p = 0; p < N_PLAYERS; p++) have_not[p] |= c;
  for (card cand : set_cards(~have_not[game.turn]))
    if ((c & legal(c | cand, game)) == 0) have_not[game.turn] |= cand;
}

void Player::print(const card &trump) {
  cout << "P" << id << ": ";
  print_card(hand, trump);
  for (int i = 0; i < N_PLAYERS; i++) {
    cout << "~" << i << ": ";
    print_card(have_not[i], trump);
  }
  cout << endl;
}