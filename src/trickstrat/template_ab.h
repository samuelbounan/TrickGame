#ifndef __XAV
#define __XAV

#include <malloc.h>

#include "utils.h"

class Algorithm {
 public:
  int node_type[N_PLAYERS];
  int n_worlds;
  int depth_leaf;
  int depth_rd;
  int n_sample;
};

template <typename Value, typename Alg>
card template_ab(Game g, card w[][N_PLAYERS], Alg algo);
template <typename Value, typename Alg>
int template_ab_aux(card& card_res, Value& r, Game& g, card w[][N_PLAYERS],
                    unsigned& valid_worlds, Value alpha[N_TEAMS],
                    int parent[N_TEAMS], Alg algo, int depth);

#include "template_ab.tpp"
#endif