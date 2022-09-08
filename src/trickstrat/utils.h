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

llu snapeq(Game g);
bool aos(Game &g, card *world, int threshold);
bool aos_aux(Game &g, card *world, int threshold);
pair<int, card> solver(Game &game, card *world, int max_depth_rd, int n_sample);
int solver_aux(Game &g, card *world, int alpha, int beta, int depth,
               int max_depth_rd, int n_sample, int &rec, int &n_prune,
               int &r_Hequi, int &r_Hgame, int &w_Hequi, int &w_Hgame);
int approx_score(Game &g, card *world, int n_sample);
void random_world(card *res, card *have_not, const card &hand, Game game);

#endif