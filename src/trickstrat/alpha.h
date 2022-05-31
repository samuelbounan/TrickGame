#ifndef __ALPHA
#define __ALPHA

#include <algorithm>  // copy array
#include <iterator>   // copy array
#include <set>
#include <unordered_map>  // hash

#include "../play.h"

card alpha(Game game, int id, card hand, card *have_not);

card alpha_search(int *res, Game game, int id, card hand, card *have_not,
                  int *alpha);

#endif