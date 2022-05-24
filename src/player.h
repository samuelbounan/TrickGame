/* Definition of all the characteristics and functionalities of a player */

#ifndef __PLAYER
#define __PLAYER

#include "strategies/random_card.h"

class Player {
 public:
  int id;
  card hand;
  card have_not[N_PLAYERS];
  card (*strategy)(Game game, int id, card hand, card *have_not);
  Player() { strategy = &random_card; }

  /**
   * @brief returns a card according to the player's strategy, and removes it
   * from its hand
   *
   * @param game
   * @return card
   */
  card playCard(Game game);

  /**
   * @brief update player knowledge according to the card played
   *
   * @param game
   * @param c
   */
  void updateBelief(Game game, card c);
};

#endif