#include "template_ab.h"

// int

void Algorithm::print_value(int a) { cout << a << endl; }

void Algorithm::alloc(int* a) {}
void Algorithm::cleanup(int* a) {}

void Algorithm::init_alpha(int* alpha) {
  for (int i = 0; i < N_PLAYERS; i++) {
    if (node_type[i] % 2)
      alpha[i] = MAX_SCORE;
    else
      alpha[i] = 0;
  }
}

void Algorithm::init_value(int i, const Game& g, unsigned valid_worlds,
                           int& res) {
  if (node_type[i] % 2) {
    res = g.max_points;
  } else {
    res = g.min_points;
  }
}
void Algorithm::fusion(int i, int a, int b, int& res) {
  if (node_type[i] % 2)
    res = min(a, b);
  else
    res = max(a, b);
}
bool Algorithm::better(int i, int a, int b) {
  if (node_type[i] % 2) return a < b;
  return a > b;
}

bool Algorithm::criterion(int i, int a, int b) {
  if (node_type[i] % 2) return a < b;
  return a > b;
}

void Algorithm::copy(int a, int& b) { b = a; }

void Algorithm::leaf_case(Game& g, card w[][N_PLAYERS], unsigned valid_worlds,
                          int& r) {
  r = solver(g, w[0], depth_rd, n_sample).first;
}

// int*

void Algorithm::print_value(int* a) {
  for (int i = 0; i < n_worlds; i++) {
    if (a[i] == -1)
      cout << "X";
    else
      cout << a[i];
    if (i != n_worlds - 1) cout << ";";
  }
  cout << endl;
}

void Algorithm::alloc(int** a) { *a = (int*)malloc(n_worlds * sizeof(int)); }

void Algorithm::cleanup(int** a) { free(*a); }

void Algorithm::init_alpha(int* alpha[N_TEAMS]) {
  for (int t = 0; t < N_TEAMS; t++)
    for (int i = 0; i < n_worlds; i++) alpha[t][i] = -1;
}

void Algorithm::init_value(int i, const Game& g, unsigned valid_worlds,
                           int* res) {
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

void Algorithm::fusion(int i, int* a, int* b, int* res) {
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

bool Algorithm::better(int i, int* a, int* b) {
  bool res = false;
  for (int w = 0; w < n_worlds; w++) {
    if (b[w] != -1) {
      if (node_type[i] % 2) {
        if (a[w] > b[w] || a[w] == -1) return false;
        if (a[w] < b[w]) res = true;
      } else {
        if (a[w] < b[w]) return false;
        if (a[w] > b[w]) res = true;
      }
    }
  }
  return res;
}

bool Algorithm::criterion(int i, int* a, int* b) {
  int sum_a = 0;
  int sum_b = 0;
  for (int w = 0; w < n_worlds; w++) {
    sum_a += a[w];
    sum_b += b[w];
  }
  if (node_type[i] % 2) return sum_a < sum_b;
  return sum_a > sum_b;
}

void Algorithm::copy(int* a, int* b) { copy_n(a, n_worlds, b); }

void Algorithm::leaf_case(Game& g, card w[][N_PLAYERS], unsigned valid_worlds,
                          int* r) {
  for (int i = 0; i < n_worlds; i++) r[i] = -1;
  while (valid_worlds) {
    unsigned i = __builtin_ctz(valid_worlds);
    valid_worlds &= ~(((unsigned)1) << i);
    r[i] = solver(g, w[i], depth_rd, n_sample).first;
  }
}

// vector <int*>
void Algorithm::print_value(vector<int*>& a) {
  cout << "{";
  int size_a = a.size();
  for (int i = 0; i < size_a; i++) {
    cout << "(";
    for (int j = 0; j < n_worlds; j++) {
      if (a[i][j] == -1)
        cout << "X";
      else
        cout << a[i][j];
      if (i != n_worlds - 1) cout << ";";
    }
    cout << ")";
    if (i != size_a - 1) cout << ", ";
  }
  cout << "}" << endl;
}

void Algorithm::alloc(vector<int*>* a) {}

void Algorithm::cleanup(vector<int*>* a) {
  for (int* x : *a) {
    cleanup(&x);
  }
  a->clear();
}

void Algorithm::init_alpha(vector<int*> alpha[N_TEAMS]) {
  for (int t = 0; t < N_TEAMS; t++) {
    cleanup(&alpha[t]);
  }
}

void Algorithm::init_value(int i, const Game& g, unsigned valid_worlds,
                           vector<int*>& res) {
  cleanup(&res);
  int* p;
  alloc(&p);
  init_value(i, g, valid_worlds, p);
  res.push_back(p);
}

void Algorithm::add_fronteer(int* x, vector<int*>& res) {
  bool add = true;
  auto it = res.begin();
  while (it != res.end()) {
    bool erased = false;
    if (better(player_root, x, *it)) {
      cleanup(&(*it));
      it = res.erase(it);
      erased = true;
    } else if (better(player_root, *it, x)) {
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
    alloc(&p);
    copy(x, p);
    res.push_back(p);
  }
}

void Algorithm::fusion(int i, vector<int*>& a, vector<int*>& b,
                       vector<int*>& res) {
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
        alloc(&z);
        for (int* y : res) {
          fusion(i, x, y, z);
          add_fronteer(z, new_res);
        }
        cleanup(&z);
      }
      copy(new_res, res);
      cleanup(&new_res);
    }
  }
}

bool Algorithm::better(int i, vector<int*>& a, vector<int*>& b) {
  for (int* y : b) {
    bool not_dominated = true;
    for (int* x : a) {
      if (better(i, x, y)) {
        not_dominated = false;
        break;
      }
    }
    if (not_dominated) return false;
  }
  return true;
}

bool Algorithm::criterion(int i, vector<int*>& a, vector<int*>& b) {
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
      } else if (better(i, sum, best_sum[k]))
        best_sum[k] = sum;
    }
  }
  return criterion(i, best_sum[0], best_sum[1]);
}

void Algorithm::copy(vector<int*>& a, vector<int*>& b) {
  cleanup(&b);
  b.clear();
  for (int* x : a) {
    int* y;
    alloc(&y);
    copy(x, y);
    b.push_back(y);
  }
}

void Algorithm::leaf_case(Game& g, card w[][N_PLAYERS], unsigned valid_worlds,
                          vector<int*>& r) {
  cleanup(&r);
  int* res;
  alloc(&res);
  leaf_case(g, w, valid_worlds, res);
  r.push_back(res);
}