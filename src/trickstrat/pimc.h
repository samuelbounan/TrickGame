#ifndef __PIMC
#define __PIMC

#include <algorithm>

#include "template_ab.h"

class SamPimc : public Algorithm {
 public:
  SamPimc() {
    depth_leaf = N_ROUNDS * N_PLAYERS + 1;
    depth_rd = N_ROUNDS * N_PLAYERS + 1;
    n_sample = 1;
  }

  SamPimc(int _n_worlds) {
    n_worlds = _n_worlds;
    depth_leaf = N_ROUNDS * N_PLAYERS + 1;
    depth_rd = N_ROUNDS * N_PLAYERS + 1;
    n_sample = 1;
  };

  void print_value(int* a) {
    for (int i = 0; i < n_worlds; i++) {
      if (a[i] == -1)
        cout << "X";
      else
        cout << a[i];
      if (i < n_worlds - 1) cout << ";";
    }
    cout << endl;
  }

  void alloc(int** a) { *a = (int*)malloc(n_worlds * sizeof(int)); }

  void cleanup(int** a) { free(*a); }

  void init_alpha(int* alpha[N_TEAMS]) {
    for (int t = 0; t < N_TEAMS; t++)
      for (int i = 0; i < n_worlds; i++) alpha[t][i] = -1;
  }

  void init_value(int i, const Game& g, unsigned valid_worlds, int* res) {
    for (int w = 0; w < n_worlds; w++) {
      if ((((unsigned)1) << w) & valid_worlds) {
        if (node_type[i] % 2)
          res[w] = g.max_points;
        else
          res[w] = g.min_points;
      } else
        res[w] = -1;
    }
  }

  void fusion(int i, int* a, int* b, int* res) {
    for (int w = 0; w < n_worlds; w++) {
      if (a[w] == -1)
        res[w] = b[w];
      else if (b[w] == -1)
        res[w] = a[w];
      else {
        if (node_type[i] % 2)
          res[w] = min(a[w], b[w]);
        else
          res[w] = max(a[w], b[w]);
      }
    }
  }

  bool pruning(int i, int* alpha, int* b) {
    bool res = false;
    for (int w = 0; w < n_worlds; w++) {
      if (b[w] != -1) {
        if (node_type[i] % 2) {
          if (alpha[w] > b[w] || alpha[w] == -1) return false;
          if (alpha[w] < b[w]) res = true;
        } else {
          if (alpha[w] < b[w]) return false;
          if (alpha[w] > b[w]) res = true;
        }
      }
    }
    return res;
  }

  bool criterion(int i, int* a, int* b) {
    int sum_a = 0;
    int sum_b = 0;
    for (int w = 0; w < n_worlds; w++) {
      sum_a += a[w];
      sum_b += b[w];
    }
    if (node_type[i] % 2) return sum_a <= sum_b;
    return sum_a >= sum_b;
  }

  void copy(int* a, int* b) { copy_n(a, n_worlds, b); }

  void leaf_case(Game& g, card w[][N_PLAYERS], unsigned valid_worlds, int* r) {
    for (int i = 0; i < n_worlds; i++) r[i] = -1;
    while (valid_worlds) {
      unsigned i = __builtin_ctz(valid_worlds);
      valid_worlds &= ~(((unsigned)1) << i);
      r[i] = solver(g, w[i], depth_rd, n_sample).first;
    }
  }
};

card pimc(Game game, int id, card hand, card* have_not);

#endif