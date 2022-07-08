#ifndef __RANDOM_CARD
#define __RANDOM_CARD

#include "../rules.h"

/**
 * @brief strategy of the player random among legal
 *
 * @param game current situation
 * @param id of the player
 * @param hand result selected among them
 * @param have_not knowledge of the player
 * @return card selected according to the strategy
 */
card random_card(Game game, int id, card hand, card *have_not);

#endif