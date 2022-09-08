#ifndef __ALPHA_BETA
#define __ALPHA_BETA

#include "template_ab.h"

class SamAlphaBeta : public Algorithm {
 public:
  int team_decl;
  SamAlphaBeta(int _team_decl) {
    n_worlds = 1;
    team_decl = _team_decl;
    depth_leaf = N_ROUNDS * N_PLAYERS + 1;
    depth_rd = N_ROUNDS * N_PLAYERS + 1;
    n_sample = 1;
  };

  void print_value(int a) { cout << a << endl; }
  void alloc(int* a) {}
  void cleanup(int* a) {}
  void init_alpha(int* alpha) {
    for (int i = 0; i < N_TEAMS; i++) {
      if (i == team_decl)
        alpha[i] = 0;
      else
        alpha[i] = MAX_SCORE;
    }
  }
  void init_value(int i, const Game& g, unsigned valid_worlds, int& res) {
    if (node_type[i] % 2) {
      res = g.max_points;
    } else {
      res = g.min_points;
    }
  }
  void fusion(int i, int a, int b, int& res) {
    if (node_type[i] % 2)
      res = min(a, b);
    else
      res = max(a, b);
  }
  bool pruning(int i, int alpha, int x) {
    if (node_type[i] % 2) return alpha < x;
    return alpha > x;
  }

  bool criterion(int i, int a, int b) {
    if (node_type[i] % 2) return a <= b;
    return a >= b;
  }

  void copy(int a, int& b) { b = a; }

  void leaf_case(Game& g, card w[][N_PLAYERS], unsigned valid_worlds, int& r) {
    r = solver(g, w[0], depth_rd, n_sample).first;
  }
};

card alpha_beta(Game game, int id, card hand, card* have_not);

#endif