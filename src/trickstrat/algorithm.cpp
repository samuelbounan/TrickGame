#include "ab.h"

// int

void Algorithm::init_alpha(int* alpha) {
  for (int i = 0; i < N_PLAYERS; i++) {
    if (node_type[i])
      alpha[i] = MAX_SCORE;
    else
      alpha[i] = 0;
  }
}
void Algorithm::value_init(int i, const Game& g, unsigned valid_worlds,
                           int& res) {
  if (node_type[i]) {
    res = MAX_SCORE - g.points[1];
  } else {
    res = g.points[0];
  }
}
void Algorithm::fusion(int i, int a, int b, int& res) { res = a; }
bool Algorithm::better(int i, int a, int b) {
  if (node_type[i]) return a < b;
  return a > b;
}
void Algorithm::copy(int a, int& b) { b = a; }

// vector<int>

void Algorithm::init_alpha(vector<int> alpha[N_TEAMS]) {
  for (int i = 0; i < n_worlds; i++) {
    alpha[0].push_back(0);
    alpha[1].push_back(MAX_SCORE);
  }
}

void Algorithm::value_init(int i, const Game& g, unsigned valid_worlds,
                           vector<int> res) {
  res.clear();
  for (int w = 0; w < n_worlds; w++) {
    if ((((unsigned)1) << w) & valid_worlds) {
      if (node_type[i])
        res.push_back(MAX_SCORE - g.points[1]);
      else
        res.push_back(g.points[0]);
    } else
      res.push_back(-1);
  }
}

void Algorithm::fusion(int i, vector<int> a, vector<int> b, vector<int> res) {
  for (int w = 0; w < n_worlds; w++) {
    if (a[w] == -1)
      res[w] = b[w];
    else if (b[w] == -1)
      res[w] = a[w];
    else {
      if (node_type[i])
        res[w] = min(a[w], b[w]);
      else
        res[i] = max(a[w], b[w]);
    }
  }
}

bool Algorithm::better(int i, vector<int> a, vector<int> b) {
  for (int w = 0; w < n_worlds; w++) {
    if (a[w] == -1 && b[w] != -1) return false;
    if (node_type[i]) {
      if (a[w] > b[w]) return false;
    } else if (a[w] < b[w])
      return false;
  }
  return true;
}

void copy(vector<int> a, vector<int> b) { b = a; }