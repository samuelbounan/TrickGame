/* Definition of all the characteristics and functionalities of a player */

#ifndef __PLAYER
#define __PLAYER

#include "bidstrat/trivial_bid.h"
#include "trickstrat/random_card.h"

class Player {
 public:
  int id;
  card hand;
  card have_not[N_PLAYERS] = {0};
  card (*trickstrat)(Game game, int id, card hand,
                     card *have_not) = &random_card;
  int (*bidstrat)(Game game, int id, card hand, card *have_not) = &trivial_bid;

  /**
   * @brief returns a bid according to the player bidding strategy
   *
   * @param game
   * @return int
   */
  int playBid(Game game);

  /**
   * @brief returns a card according to the player's strategy, and removes it
   * from its hand
   *
   * @param game
   * @return card
   */
  card playCard(const Game &game);

  /**
   * @brief update player knowledge according to the card played
   *
   * @param game
   * @param c
   */
  void updateBelief(const Game &game, const card &c);

  void print(const card &trump);
};

#endif