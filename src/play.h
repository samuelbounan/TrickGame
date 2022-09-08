#ifndef __PLAY
#define __PLAY

#include "player.h"

/**
 * @brief generates random hands of size_hand, update players
 *
 * @param player
 */
void deal_hands(Player *player, int seed);

/**
 * @brief during the bidding phase, update the parameters of the game with the
 * bid played
 *
 * @param game
 * @param player
 * @param bid
 */
void update_bid(Game &game, int bid);

/**
 * @brief add a card c to the trick
 *
 * @param game
 * @param c
 * @return {eventual points, former turn}
 */
pair<int, int> update_card(Game &game, const card &c);

/**
 * @brief prints the game, and the players hands
 *
 * @param game
 * @param player
 */
void print(const Game &game, Player *player);

#endif