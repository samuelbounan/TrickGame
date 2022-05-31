#ifndef __ALPHA_BETA
#define __ALPHA_BETA

#define MAX_SCORE 500

#include <algorithm>  // copy array
#include <iterator>   // copy array
#include <set>
#include <string>
#include <unordered_map>  // hash

#include "../play.h"

card ab(Game game, int id, card hand, card *have_not);

void ab_aux(Game *game, card *have_not, int *alpha);

typedef long long unsigned llu;
// (remaining, turn, points) -> (scores, {opt_cards})
typedef unordered_map<llu, pair<llu, card>> hash_game;

#endif