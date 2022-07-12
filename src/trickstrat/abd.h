#ifndef __ABD
#define __ABD

#include "utils.h"

class Algorithm {
  int node_type[N_PLAYERS];
  int threshold_score;

 public:
  int n_worlds;

  Algorithm(int* _node_type, int _threshold, int _n_worlds) {
    copy_n(_node_type, N_PLAYERS, node_type);
    threshold_score = _threshold;
    n_worlds = _n_worlds;
  };

  void init_alpha(int* alpha);
  void value_init(const Game& g, unsigned valid_worlds, int i, int& res);
  void fusion(int i, int a, int b, int& res);
  bool lower(int i, int a, int b);
  void copy(int a, int& b);
};

template <typename Value>
card abd(Game g, card w[][N_PLAYERS], Algorithm algo);
template <typename Value>
void abd_aux(card& card_res, Value* res, Game& g, card w[][N_PLAYERS],
             unsigned& valid_worlds, Value* _alpha, Algorithm algo,
             unordered_map<Game, Value>& H, int depth);
template <typename Value>
inline void max(int& i, Value& a, Value& b, Algorithm& algo, Value& res);

#include "abd.tpp"
#endif