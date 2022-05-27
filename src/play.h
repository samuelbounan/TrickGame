#ifndef __PLAY
#define __PLAY

#include "player.h"

/**
 * @brief generates random hands of size_hand, update players
 *
 * @param player
 */
void deal_hands(Player *player);

/**
 * @brief during the bidding phase, update the parameters of the game with the
 * bid played
 *
 * @param game
 * @param player
 * @param bid
 */
void update_bid(Game *game, Player *player, int bid);

/**
 * @brief performs the bidding phase
 *
 * @param game played
 * @param player playing
 */
void bidding(Game *game, Player *player, int printing);

/**
 * @brief in a trickgame, update the parameters of the game with the card played
 *
 * @param game
 * @param c
 */
void update_card(Game *game, card c);

/**
 * @brief tricktaking game played until round = N_ROUNDS
 *
 * @param game
 * @param player
 * @param printing to print the game played
 */
void trickgame(Game *game, Player *player, int printing);

/**
 * @brief prints the game, and the players hands
 *
 * @param game
 * @param player
 */
void print(Game game, Player *player);

#endif