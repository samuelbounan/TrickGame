#ifndef __ALPHA_BETA
#define __ALPHA_BETA

#define MAX_SCORE 500

#include <algorithm>  // copy array
#include <iterator>   // copy array
#include <set>
#include <string>
#include <unordered_map>  // hash

#include "../play.h"

card alpha_beta(Game game, int id, card hand, card *have_not);
void alpha_beta_aux(Game *game, card *have_not, int *alpha);

typedef long long unsigned llu;

#endif