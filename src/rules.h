#include <list>

#include "game.h"

/* unicode corresponding to a bid */
extern char bidname[N_BIDS][80];
/* suits set */
extern card suits[N_SUITS];
/* undicode corresponding to the cards */
extern char cardname[N_CARDS][80];

/**
 * @brief list of bidding values that could be taken
 *
 * @param bids
 * @return list<int>
 */
list<int> biddable(list<int> bids);

/**
 * @brief who is the next to bid
 *
 * @param game
 * @return int
 */
int next_bid_turn(Game game);

/**
 * @brief when bidding phase is over.
 * Must also actualise game parameters to play the tricktaking game
 *
 * @param game
 * @return true
 * @return false
 */
bool end_bidding(Game *game);

/**
 * @brief sort a set of cards according to the trump chosen
 *
 * @param hand unsorted set that will be sorted in place
 * @param trump mask
 * @return card sorted set
 */
card sort(card hand, const card &trump);

/**
 * @brief unsort a set of cards according to the trump chosen
 *
 * @param hand sorted set that will be unsorted in place
 * @param trump mask
 * @return card unsorted set
 */
card unsort(card hand, const card &trump);

/**
 * @brief cards that are legal in current situation, with hand
 *
 * @param hand
 * @param game
 * @return card
 */
card legal(const card &hand, const Game &game);

/**
 * @brief determines the player who won the current trick in game
 *
 * @param game
 * @return int
 */
int winner_trick(const Game &game);

int points_card(const card &c);

/**
 * @brief points of current trick in game
 *
 * @param game
 * @return int
 */
int points_trick(const Game &game);

/**
 * @brief when the trickgame is over
 *
 * @param game
 * @return if end of the trickgame
 */
bool end_trickgame(Game *game);

/// Implementation independent to the games rules

/**
 * @brief prints all the bids in the list
 *
 * @param bids
 */
void print_bid(list<int> bids);

/**
 * @brief prints all the cards in hand
 *
 * @param hand set of cards to print
 * @param trump to find unicode
 */
void print_card(card c, const card &trump);

/**
 * @brief prints the cards in the order of the list
 *
 * @param trick list of cards
 * @param trump to find unicode
 */
void print_trick(list<card>, const card &trump);