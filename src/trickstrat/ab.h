#ifndef __XAV
#define __XAV

#include <malloc.h>

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

  void print_value(int a);
  void alloc(int* a);
  void cleanup(int* a);
  void init_alpha(int* alpha);
  void init_value(int i, const Game& g, unsigned valid_worlds, int& res);
  void fusion(int i, int a, int b, int& res);
  bool better(int i, int a, int b);
  bool criterion(int i, int a, int b);
  void copy(int a, int& b);

  void print_value(int* a);
  void alloc(int** a);
  void cleanup(int** a);
  void init_alpha(int* alpha[N_TEAMS]);
  void init_value(int i, const Game& g, unsigned valid_worlds, int* res);
  void fusion(int i, int* a, int* b, int* res);
  bool better(int i, int* a, int* b);
  bool criterion(int i, int* a, int* b);
  void copy(int* a, int* b);

  void print_value(vector<int*>& a);
  void alloc(vector<int*>* a);
  void cleanup(vector<int*>* a);
  void init_alpha(vector<int*> alpha[N_TEAMS]);
  void init_value(int i, const Game& g, unsigned valid_worlds,
                  vector<int*>& res);
  void fusion(int i, vector<int*>& a, vector<int*>& b, vector<int*>& res);
  bool better(int i, vector<int*>& a, vector<int*>& b);
  bool criterion(int i, vector<int*>& a, vector<int*>& b);
  void copy(vector<int*>& a, vector<int*>& b);
  void add_fronteer(int i, int* x, vector<int*>& res);
};

template <typename Value>
card xav(Game g, card w[][N_PLAYERS], Algorithm algo);
template <typename Value>
int xav_aux(card& card_res, Value& r, Game& g, card w[][N_PLAYERS],
            unsigned& valid_worlds, Value alpha[N_TEAMS], int parent[N_TEAMS],
            Algorithm algo, int depth);

#include "ab.tpp"
#endif