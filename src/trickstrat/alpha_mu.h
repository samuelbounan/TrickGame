#ifndef __ALPHA_MU
#define __ALPHA_MU

#include <algorithm>

#include "pimc.h"

class AlphaMu : public Algorithm {
 public:
  Pimc base;
  int player_root;

  AlphaMu(int _n_worlds, int _player_root, int _node_type[N_PLAYERS]) {
    for (int i = 0; i < N_PLAYERS; i++) {
      node_type[i] = _node_type[i];
      base.node_type[i] = _node_type[i] % 2;
    }
    n_worlds = _n_worlds;
    base.n_worlds = _n_worlds;
    player_root = _player_root;
    depth_leaf = N_ROUNDS * N_PLAYERS + 1;
    depth_rd = N_ROUNDS * N_PLAYERS + 1;
    n_sample = 1;
  };

  void print_value(vector<int*>& a) {
    cout << "{";
    int size_a = a.size();
    for (int i = 0; i < size_a; i++) {
      cout << "(";
      for (int j = 0; j < n_worlds; j++) {
        if (a[i][j] == -1)
          cout << "X";
        else
          cout << a[i][j];
        if (j != n_worlds - 1) cout << ";";
      }
      cout << ")";
      if (i != size_a - 1) cout << ", ";
    }
    cout << "}" << endl;
  }

  void alloc(vector<int*>* a) {}

  void cleanup(vector<int*>* a) {
    for (int* x : *a) {
      free(x);
    }
    a->clear();
  }

  void init_alpha(vector<int*> alpha[N_TEAMS]) {
    for (int t = 0; t < N_TEAMS; t++) {
      cleanup(&alpha[t]);
    }
  }

  void init_value(int i, const Game& g, unsigned valid_worlds,
                  vector<int*>& res) {
    cleanup(&res);
    int* p;
    base.alloc(&p);
    base.init_value(i, g, valid_worlds, p);
    res.push_back(p);
  }

  void add_fronteer(int* x, vector<int*>& res) {
    bool add = true;
    auto it = res.begin();
    while (it != res.end()) {
      bool erased = false;
      if (base.pruning(player_root, x, *it)) {
        free(*it);
        it = res.erase(it);
        erased = true;
      } else if (base.pruning(player_root, *it, x)) {
        add = false;
        break;
      } else {
        bool equal = true;
        for (int i = 0; i < n_worlds; i++) {
          if (x[i] != *it[i]) {
            equal = false;
            break;
          }
          if (equal) {
            add = false;
            break;
          }
        }
      }
      if (!erased) it++;
    }
    if (add) {
      int* p;
      base.alloc(&p);
      base.copy(x, p);
      res.push_back(p);
    }
  }

  void fusion(int i, vector<int*>& a, vector<int*>& b, vector<int*>& res) {
    if (res.begin() == res.end()) {
      copy(a, res);
    } else {
      if (node_type[i] < 2) {
        for (int* x : a) {
          add_fronteer(x, res);
        }
      } else {
        vector<int*> new_res;
        for (int* x : a) {
          int* z;
          base.alloc(&z);
          for (int* y : res) {
            base.fusion(i, x, y, z);
            add_fronteer(z, new_res);
          }
          free(z);
        }
        copy(new_res, res);
        cleanup(&new_res);
      }
    }
  }

  bool pruning(int i, vector<int*>& alpha, vector<int*>& b) {
    for (int* y : b) {
      bool dominated = false;
      for (int* x : alpha) {
        if (base.pruning(i, x, y)) {
          dominated = true;
          break;
        }
      }
      if (!dominated) return false;
    }
    return true;
  }

  bool criterion(int i, vector<int*>& a, vector<int*>& b) {
    int best_sum[2];
    vector<int*> tab[2] = {a, b};
    for (int k = 0; k < 2; k++) {
      bool init = true;
      for (int* x : tab[k]) {
        int sum = 0;
        for (int w = 0; w < n_worlds; w++) sum += x[w];
        if (init) {
          init = false;
          best_sum[k] = sum;
        } else if (((node_type[i] % 2 == 1) && (sum < best_sum[k])) ||
                   ((node_type[i] % 2 == 0) && (sum < best_sum[k])))
          best_sum[k] = sum;
      }
    }
    if (node_type[i] % 2) return best_sum[0] <= best_sum[1];
    return best_sum[0] >= best_sum[1];
  }

  void copy(vector<int*>& a, vector<int*>& b) {
    cleanup(&b);
    b.clear();
    for (int* x : a) {
      int* y;
      base.alloc(&y);
      base.copy(x, y);
      b.push_back(y);
    }
  }

  void leaf_case(Game& g, card w[][N_PLAYERS], unsigned valid_worlds,
                 vector<int*>& r) {
    cleanup(&r);
    int* res;
    base.alloc(&res);
    base.leaf_case(g, w, valid_worlds, res);
    r.push_back(res);
  }
};

card alpha_mu(Game game, int id, card hand, card* have_not);

#endif