#ifndef __UTILS
#define __UTILS

#include <algorithm>
#include <chrono>
#include <iterator>  // copy array
#include <string>
#include <unordered_map>  // hash

#include "../play.h"
#include "../rules.h"

typedef long long unsigned llu;

llu snapg(Game g);
llu snapeq(Game g);
llu snaps(int *scores);
int sco(llu x, int p);
bool aos(Game *g, card *world, int threshold);
bool aos_aux(Game *g, card *world, int threshold);
card alpha_beta(Game game, int id, card hand, card *world, int max_depth_ab,
                int n_sample_ab);
llu alpha_beta_aux(Game *game, card *world, int *alpha, int depth);
void random_world(card *res, card *have_not, card hand, Game game);

#endif