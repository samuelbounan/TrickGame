#ifndef __XAV
#define __XAV

#include <malloc.h>

#include "utils.h"

class Algorithm {
 public:
  int node_type[N_PLAYERS];
  int n_worlds;
  int player_root;
  int depth_leaf;
  int depth_rd;
  int n_sample;

  Algorithm(int _n_worlds, int _player_root) {
    n_worlds = _n_worlds;
    player_root = _player_root;
    depth_leaf = N_ROUNDS * N_PLAYERS + 1;
    depth_rd = N_ROUNDS * N_PLAYERS + 1;
    n_sample = 1;
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
  void leaf_case(Game& g, card w[][N_PLAYERS], unsigned valid_worlds, int& r);

  void print_value(int* a);
  void alloc(int** a);
  void cleanup(int** a);
  void init_alpha(int* alpha[N_TEAMS]);
  void init_value(int i, const Game& g, unsigned valid_worlds, int* res);
  void fusion(int i, int* a, int* b, int* res);
  bool better(int i, int* a, int* b);
  bool criterion(int i, int* a, int* b);
  void copy(int* a, int* b);
  void leaf_case(Game& g, card w[][N_PLAYERS], unsigned valid_worlds, int* r);

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
  void add_fronteer(int* x, vector<int*>& res);
  void leaf_case(Game& g, card w[][N_PLAYERS], unsigned valid_worlds,
                 vector<int*>& r);
};

template <typename Value>
card template_ab(Game g, card w[][N_PLAYERS], Algorithm algo);
template <typename Value>
int template_ab_aux(card& card_res, Value& r, Game& g, card w[][N_PLAYERS],
                    unsigned& valid_worlds, Value alpha[N_TEAMS],
                    int parent[N_TEAMS], Algorithm algo, int depth);

#include "template_ab.tpp"
#endif