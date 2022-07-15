#ifndef __XAV
#define __XAV

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
  void value_init(int i, const Game& g, unsigned valid_worlds, int& res);
  void fusion(int i, int a, int b, int& res);
  bool better(int i, int a, int b);
  void copy(int a, int& b);

  void init_alpha(vector<int> alpha[N_TEAMS]);
  void value_init(int i, const Game& g, unsigned valid_worlds,
                  vector<int>& res);
  void fusion(int i, const vector<int>& a, const vector<int>& b,
              vector<int>& res);
  bool better(int i, const vector<int>& a, const vector<int>& b);
  void copy(const vector<int>& a, vector<int>& b);
};

template <typename Value>
card xav(Game g, card w[][N_PLAYERS], Algorithm algo);
template <typename Value>
int xav_aux(card& card_res, Value& r, Game& g, card w[][N_PLAYERS],
            unsigned& valid_worlds, Value alpha[N_TEAMS], int parent[N_TEAMS],
            Algorithm algo, int depth);
template <typename Value>
inline void max(int& i, Value& a, Value& b, Algorithm& algo, Value& res);

#include "ab.tpp"
#endif