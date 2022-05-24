#include <list>

#include "game.h"

/* undicode corresponding to the cards */
extern char cardname[N_CARDS][80];

/**
 * @brief determines the player who won the current trick in game
 *
 * @param game
 * @return int
 */
int winner_trick(Game game);

/**
 * @brief points of current trick in game
 *
 * @param game
 * @return int
 */
int points_trick(Game game);

/**
 * @brief to avoid bidding play
 *
 * @param game
 */
void trivial_bid(Game *game);

/**
 * @brief cards that are playable in current situation, with hand
 *
 * @param hand
 * @param game
 * @return card
 */
card playable(card hand, Game game);

/**
 * @brief to update game parameters at the end of the game
 *
 * @param game
 */
void finish(Game *game);

/**
 * @brief sort a set of cards according to the trump chosen
 *
 * @param hand unsorted set that will be sorted in place
 * @param trump mask
 * @return card sorted set
 */
card sort(card *hand, card trump);

/**
 * @brief unsort a set of cards according to the trump chosen
 *
 * @param hand sorted set that will be unsorted in place
 * @param trump mask
 * @return card unsorted set
 */
card unsort(card *hand, card trump);

/**
 * @brief returns if player p won the game
 *
 * @param p
 * @param game
 * @return p's team won
 */
bool won(int p, Game game);

/**
 * @brief prints all the cards in hand
 *
 * @param hand set of cards to print
 * @param trump to find unicode
 */
void print_card(card c, card trump);

/**
 * @brief prints the cards in the order of the list
 *
 * @param trick list of cards
 * @param trump to find unicode
 */
void print_trick(list<card>, card trump);