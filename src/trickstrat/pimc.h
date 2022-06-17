#ifndef __PIMC
#define __PIMC

#include <algorithm>

#include "alpha_beta.h"
#include "utils.h"

card pimc(Game game, int id, card hand, card *have_not);

#endif