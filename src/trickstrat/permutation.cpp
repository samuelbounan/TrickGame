#include "permutation.h"

int fact(int n) {
  if (n <= 0) return 1;
  return n * fact(n - 1);
}

template <typename T>
void heap_algo(T perm[][N_PLAYERS], int k, int &idx, T A[N_PLAYERS]) {
  if (k == 1) {
    copy_n(A, N_PLAYERS, perm[idx]);
    idx++;
  } else {
    heap_algo(perm, k - 1, idx, A);
    for (int i = 0; i <= k - 2; i++) {
      int tmp = A[k - 1];
      if (k % 2) {
        A[k - 1] = A[0];
        A[0] = tmp;
      } else {
        A[k - 1] = A[i];
        A[i] = tmp;
      }
      heap_algo(perm, k - 1, idx, A);
    }
  }
}

void fill_idx_perm(unsigned idx_perm[N_PLAYERS][N_PLAYERS]) {
  for (int i = 0; i < N_PLAYERS; i++)
    for (int j = 0; j < N_PLAYERS; j++) idx_perm[i][j] = 0;
  int B[N_PLAYERS];
  for (int i = 0; i < N_PLAYERS; i++) B[i] = i;
  int fact_n = fact(N_PLAYERS);
  int perm[fact_n][N_PLAYERS];
  int idx = 0;
  heap_algo(perm, N_PLAYERS, idx, B);
  for (int w = 0; w < fact_n; w++) {
    for (int p = 0; p < N_PLAYERS; p++) {
      int h = perm[w][p];
      idx_perm[p][h] |= (((unsigned)1) << w);
    }
  }
}

void fill_worlds(card have_not[N_PLAYERS], card hand, Game &game, card w[][N_PLAYERS], int n_worlds) {
  // gen a world
  card world[N_PLAYERS];
  random_world(world, have_not, hand, game);

  // only[p] are the cards that p has and only p can have
  card only[N_PLAYERS] = {0};
  for (int p = 0; p < N_PLAYERS; p++) {
    only[p] = world[p];
    for (int i = 0; i < N_PLAYERS; i++)
      if (i != p) only[p] &= have_not[i];
  }

  // we want world[p] & ~only[p] of same length for each p
  int min_rem = SIZE_HAND;
  for (int p = 0; p < N_PLAYERS; p++)
    min_rem = min(min_rem, POPCOUNT(world[p] & ~only[p]));

  // complete only to reach min_rem
  for (int p = 0; p < N_PLAYERS; p++) {
    while (POPCOUNT(world[p] & ~only[p]) > min_rem) {
      card hand_p = world[p];
      int idx = mt() % POPCOUNT(hand_p);
      while (idx > 0) {
        hand_p &= ~(ONE << CTZ(hand_p));
        idx--;
      }
      card c = ONE << CTZ(hand_p);
      if ((c & only[p]) == 0)
        only[p] |= c;
    }
  }

  // permutation of world[p] & ~only[p]
  card A[N_PLAYERS];
  for (int p = 0; p < N_PLAYERS; p++)
    A[p] = world[p] & ~only[p];
  int k = 0;
  heap_algo(w, N_PLAYERS, k, A);

  // add only[p]
  for (int i = 0; i < n_worlds; i++)
    for (int p = 0; p < N_PLAYERS; p++) {
      w[i][p] |= only[p];
    }
}

card permutation(Game game, int id, card hand, card *have_not) {
  if (PRINTING >= 5) cout << "PERMUTATION" << endl;

  // init
  int n_worlds = fact(N_PLAYERS);
  SamPermutation algo(n_worlds);
  for (int i = 0; i < N_PLAYERS; i++)
    if (game.team[i] == game.team[game.declarer])
      algo.node_type[i] = 0;
    else
      algo.node_type[i] = 1;
  algo.depth_leaf = 5;
  algo.depth_rd = 20;

  // gen worlds and run ab
  card w[n_worlds][N_PLAYERS];
  fill_worlds(have_not, hand, game, w, n_worlds);
  fill_idx_perm(algo.idx_perm);

  return template_ab<int *>(game, w, algo);
}