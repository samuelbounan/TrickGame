#include "abd.h"

template <typename Value>
card abd(Game g, card** w, unsigned valid_worlds, Value* alpha,
         Algorithm algo) {
  unordered_map<Game, Value> H;
  Value res[N_PLAYERS];
  abd_aux(res, g, w, valid_worlds, alpha, H);
  return 0;
}

template <typename Value>
void abd_aux(Value* res, Game& g, card** w, unsigned valid_worlds,
             Value* _alpha, Algorithm algo, unordered_map<Game, Value> H) {
  // init
  bool save_H = g.trick.empty();
  Value init[N_PLAYERS];
  if (save_H && H.find(g) != H.end()) {
    copy_n(H[g], N_PLAYERS, init);
  } else {
    for (int i = 0; i < N_PLAYERS; i++) {
      algo.value_init(g, valid_worlds, i, init[i]);
    }
  }
  for (int i = 0; i < N_PLAYERS; i++) {
    algo.copy(init[i], res[i]);
  }
  Value alpha[N_PLAYERS];
  for (int i = 0; i < N_PLAYERS; i++) {
    max(i, _alpha[i], init[i], algo, alpha[i]);
  }

  // compute enemies
  int id = g.turn;
  list<int> enemies;
  for (int i = 0; i < N_PLAYERS; i++) {
    if (g.team[i] != g.team[id]) {
      enemies.push_back(i);
    }
  }

  // compute possible and new valid worlds
  card possible = 0;
  unsigned valid_worlds_copy = valid_worlds;
  unsigned valid_with[N_CARDS];
  for (int i = 0; i < N_CARDS; i++) {
    valid_with[i] = valid_worlds;
  }
  while (valid_worlds_copy) {
    int id_w = __builtin_ctzll(valid_worlds_copy);
    unsigned vc_w = ((unsigned)1) << id_w;
    valid_worlds_copy &= ~vc_w;
    card legal = legal(w[id_w][id], g);
    possible |= legal;
    while (~legal) {
      int id_c = __builtin_ctzll(~legal);
      legal |= ONE << id_c;
      valid_world[id_c] &= ~vc_w;
    }
  }

  // main loop
  while (possible) {
    int id_c = __builtin_ctzll(possible);
    card c = ONE << id_c;
    possible &= ~c;

    // check pruning condition
    for (int i : enemies) {
      if (algo.lower(i, alpha[id], alpha[i])) {
        for (int j = 0; j < N_PLAYERS; j++) {
          if (j != id) {
            algo.copy(init[j], res[j]);
          }
        }
        goto end;
      }
    }

    // compute rec value
    auto update_save = update_card(&g, c);
    unsigned new_valid = valid_with[id_c];
    while (new_valid) {
      int id_w = __builtin_ctzll(new_valid);
      new_valid &= ~(((unsigned)1) << id_w);
      w[id_w][id] &= ~c;
    }
    new_valid = valid_with[id_c];
    Value v[N_PLAYERS];
    abd_aux(v, g, w, new_valid, alpha, algo, H);
    while (new_valid) {
      int id_w = __builtin_ctzll(new_valid);
      new_valid &= ~(((unsigned)1) << id_w);
      w[id_w][id] |= c;
    }
    g.removeCard(update_save);

    // update alpha and res
    max(id, alpha[id], v[id], alpha[id]);
    if (algo.lower(id, res[id], v[id])) {
      for (int i = 0; i < N_PLAYERS; i++) {
        copy(v[i], res[i]);
      }
    } else if (!algo.lower(id, v[id], res[id])) {
      for (int i = 0; i < N_PLAYERS; i++) {
        algo.fusion(i, res[i], v[i], res[i]);
      }
    }
  }
  if (save_H) {
    H[g] = res;
  }
}

template <typename Value>
inline void max(int& i, Value& a, Value& b, Algorithm& algo, Value& res) {
  if (algo.lower(i, a, b)) {
    algo.copy(b, res);
  } else if (algo.lower(i, b, a)) {
    algo.copy(a, res);
  } else {
    algo.fusion(i, a, b, res);
  }
}