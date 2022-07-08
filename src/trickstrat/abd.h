#ifndef __ABD
#define __ABD

#include "utils.h"

class Algorithm {
  int node_type[N_PLAYERS];
  int threshold_score;

 public:
  int n_worlds;

  void value_init(Game g, unsigned valid_worlds, int i, int* res);
  void fusion(int i, int* a, int* b, int* res);
  bool lower(int i, int* a, int* b, int* res);
  int* copy(int* a, int* b);
};

template <typename Value>
card abd(Game g, card** w, unsigned valid_worlds, Value* alpha, Algorithm algo);

#endif