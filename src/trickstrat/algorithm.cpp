#include "abd.h"

void Algorithm::init_alpha(int* alpha) {
  for (int i = 0; i < N_PLAYERS; i++) {
    alpha[i] = 0;
  }
}
void Algorithm::value_init(const Game& g, unsigned valid_worlds, int i,
                           int& res) {
  res = g.points[g.team[i]];
}
void Algorithm::fusion(int i, int a, int b, int& res) { res = a; }
bool Algorithm::lower(int i, int a, int b) {
  if (node_type[i]) return a > b;
  return a < b;
  ;
}
void Algorithm::copy(int a, int& b) { b = a; }