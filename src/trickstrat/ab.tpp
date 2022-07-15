#include "ab.h"

template <typename Value>
card xav(Game g, card w[][N_PLAYERS], Algorithm algo) {
  card card_res = 0;
  Value r;
  unsigned valid_worlds = (((unsigned)1) << algo.n_worlds) - 1;
  algo.value_init(g.turn, g, valid_worlds, r);
  Value alpha[N_TEAMS];
  algo.init_alpha(alpha);
  int parent[N_TEAMS] = {0, 1};
  xav_aux(card_res, r, g, w, valid_worlds, alpha, parent, algo, 0);
  return card_res;
}

template <typename Value>
inline void max(int& i, Value& a, Value& b, Algorithm& algo, Value& res) {
  if (algo.better(i, a, b)) {
    algo.copy(a, res);
  } else if (algo.better(i, b, a)) {
    algo.copy(b, res);
  } else {
    algo.fusion(i, a, b, res);
  }
}

template <typename Value>
int xav_aux(card& card_res, Value& r, Game& g, card w[][N_PLAYERS],
            unsigned& valid_worlds, Value alpha[N_TEAMS], int parent[N_TEAMS],
            Algorithm algo, int depth) {
  // init
  int res = -1;
  int id = g.turn;
  int tm = g.team[id];
  Value alpha_save;
  algo.copy(alpha[tm], alpha_save);
  int parent_save = parent[tm];
  parent[tm] = id;
  algo.value_init(id, g, valid_worlds, r);

  // compute possible and new valid worlds
  card possible = 0;
  unsigned valid_worlds_copy = valid_worlds;
  unsigned valid_with[N_CARDS];
  if (end_trickgame(&g)) goto end;
  for (int i = 0; i < N_CARDS; i++) {
    valid_with[i] = valid_worlds;
  }
  while (valid_worlds_copy) {
    int id_w = CTZ(valid_worlds_copy);
    unsigned vc_w = ((unsigned)1) << id_w;
    valid_worlds_copy &= ~vc_w;
    card legal_w = legal(w[id_w][id], g);
    possible |= legal_w;
    while (~legal_w & deck) {
      int id_c = CTZ(~legal_w);
      legal_w |= ONE << id_c;
      valid_with[id_c] &= ~vc_w;
    }
  }

  // main loop
  while (possible) {
    int id_c = CTZ(possible);
    card c = ONE << id_c;
    possible &= ~c;

#if PRINTING > 6
    if (PRINTING > 7 || depth < 1) {
      for (int i = 0; i < depth; i++) cout << "  ";
      print_card(c, g.trump);
    }
#endif

    // update alpha and check pruning condition
    max(id, alpha[tm], r, algo, alpha[tm]);

    for (int i = 0; i < N_TEAMS; i++) {
      if (i != tm && algo.better(parent[i], alpha[i], r)) {
        res = i;
        goto end;
      }
    }

    // compute rec value
    auto update_save = update_card(&g, c);
    unsigned new_valid = valid_with[id_c];
    while (new_valid) {  // remove c from valid worlds
      int id_w = CTZ(new_valid);
      new_valid &= ~(((unsigned)1) << id_w);
      w[id_w][id] &= ~c;
    }
    new_valid = valid_with[id_c];
    Value v;
    res = xav_aux(card_res, v, g, w, valid_worlds, alpha, parent, algo,
                  depth + 1);
    while (new_valid) {  // put c again in every valid world
      int id_w = CTZ(new_valid);
      new_valid &= ~(((unsigned)1) << id_w);
      w[id_w][id] |= c;
    }
    g.removeCard(update_save);

#if PRINTING > 6
    if (PRINTING > 7 || depth < 1) {
      for (int i = 0; i < depth; i++) cout << "  ";
      cout << "(" << v << ")" << endl;
    }
#endif

    // update r with v
    if (depth == 0 && !algo.better(id, r, v)) card_res = c;
    if (res != -1) {
      if (res == tm) {
        res = -1;
        continue;
      } else {
        goto end;
      }
    }
    max(id, r, v, algo, r);
  }
end:
  algo.copy(alpha_save, alpha[tm]);
  parent[tm] = parent_save;
  return res;
}