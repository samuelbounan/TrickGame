#ifndef __ALPHA_BETA
#define __ALPHA_BETA

#include <algorithm>  // copy array
#include <chrono>
#include <iterator>  // copy array
#include <set>
#include <string>
#include <unordered_map>  // hash

#include "../play.h"

typedef long long unsigned llu;

card alpha_beta(Game game, int id, card hand, card *have_not);
llu alpha_beta_aux(Game *game, card *have_not, int *alpha, int depth);

#endif