#ifndef __PLAY
#define __PLAY

#include "player.h"

/**
 * @brief generates random hands of SIZE_HAND, update players
 *
 * @param player
 */
void deal_hands(Player *player);

/**
 * @brief performs the bidding phase
 *
 * @param game played
 * @param player playing
 */
void bidding(Game *game, Player *player);

/**
 * @brief in a trickgame, update the parameters of the game and the players to
 * the card c played
 *
 * @param game
 * @param player
 * @param c
 */
void update_card(Game *game, Player *player, card c);

/**
 * @brief tricktaking game played until round = N_ROUNDS
 *
 * @param game
 * @param player
 * @param printing to print the game played
 */
void trickgame(Game *game, Player *player, bool printing);

/**
 * @brief prints the game, and the players hands
 *
 * @param game
 * @param player
 */
void print(Game game, Player *player);

#endif