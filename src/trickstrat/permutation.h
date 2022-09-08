#ifndef __PERMUTATION
#define __PERMUTATION

#include "template_ab.h"

class Permutation : public Algorithm {
 public:
  // idx_perm[i][j] is the bit vector of the worlds where player i has hand j (the hand of player j in the original permutation)
  unsigned idx_perm[N_PLAYERS][N_PLAYERS];

  Permutation() {}

  Permutation(int _n_worlds) {
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

  void fusion(int id, int* a, int* b, int* res) {
    for (int h = 0; h < N_PLAYERS; h++) {
      unsigned worlds_h = idx_perm[id][h];
      unsigned loop = worlds_h;
      bool same_worlds = true;
      int sum_a = 0;
      int sum_b = 0;
      while (loop) {
        int i = __builtin_ctz(loop);
        loop &= ~(((unsigned)1) << i);
        if ((a[i] == -1 || b[i] == -1) && a[i] != b[i]) {
          same_worlds = false;
          break;
        }
        sum_a += a[i];
        sum_b += b[i];
      }
      loop = worlds_h;
      if (!same_worlds)
        while (loop) {
          int i = __builtin_ctz(loop);
          loop &= ~(((unsigned)1) << i);
          res[i] = max(a[i], b[i]);
        }
      else if (sum_a > sum_b)
        while (loop) {
          int i = __builtin_ctz(loop);
          loop &= ~(((unsigned)1) << i);
          if (node_type[id])
            res[i] = b[i];
          else
            res[i] = a[i];
        }
      else if (sum_b > sum_a)
        while (loop) {
          int i = __builtin_ctz(loop);
          loop &= ~(((unsigned)1) << i);
          if (node_type[id])
            res[i] = a[i];
          else
            res[i] = b[i];
        }
      else
        while (loop) {
          int i = __builtin_ctz(loop);
          loop &= ~(((unsigned)1) << i);
          res[i] = (a[i] + b[i]) / 2;
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

  bool criterion(int id, int* a, int* b) {
    unsigned loop = idx_perm[id][id];
    int sum_a = 0;
    int sum_b = 0;
    while (loop) {
      int i = __builtin_ctz(loop);
      loop &= ~(((unsigned)1) << i);
      if (a[i] == -1) return false;
      if (b[i] == -1) return true;
      sum_a += a[i];
      sum_b += b[i];
    }
    if (node_type[id] % 2) return sum_a <= sum_b;
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

card permutation(Game game, int id, card hand, card* have_not);

#endif