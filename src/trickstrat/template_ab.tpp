#include "template_ab.h"

#define depth_print 2

template <typename Value>
card template_ab(Game g, card w[][N_PLAYERS], Algorithm algo) {
  // init
  card card_res = 0;
  Value r;
  algo.alloc(&r);
  unsigned valid_worlds = (((unsigned)1) << algo.n_worlds) - 1;
  Value alpha[N_TEAMS];
  for (int i = 0; i < N_TEAMS; i++) algo.alloc(&alpha[i]);
  algo.init_alpha(alpha);
  int parent[N_TEAMS] = {0, 1};

  // run aux
  template_ab_aux(card_res, r, g, w, valid_worlds, alpha, parent, algo, 0);
  if (PRINTING > 5) algo.print_value(r);
  algo.cleanup(&r);
  for (int i = 0; i < N_TEAMS; i++) algo.cleanup(&alpha[i]);
  return card_res;
}

template <typename Value>
int template_ab_aux(card& card_res, Value& r, Game& g, card w[][N_PLAYERS],
                    unsigned& valid_worlds, Value alpha[N_TEAMS],
                    int parent[N_TEAMS], Algorithm algo, int depth) {
  int res = -1;

  // SPECIAL CASES
  if (end_trickgame(g))
    algo.init_value(g.declarer, g, valid_worlds, r);
  else if (depth == algo.depth_leaf)
    algo.leaf_case(g, w, valid_worlds, r);
  else if (depth == 0) {
    int id = g.turn;
    Value v;
    algo.alloc(&v);
    algo.init_value(id, g, valid_worlds, r);

    // compute possible and new valid worlds
    card possible = 0;
    unsigned valid_worlds_copy = valid_worlds;
    unsigned valid_with[N_CARDS];
    for (int i = 0; i < N_CARDS; i++) {
      valid_with[i] = valid_worlds;
    }
    while (valid_worlds_copy) {
      int id_w = CTZ(valid_worlds_copy);
      unsigned vc_w = ((unsigned)1) << id_w;
      valid_worlds_copy &= ~vc_w;

      card legal_w = legal(w[id_w][id], g);

      possible |= reduce_legal(legal_w, g);
      while (~legal_w & deck) {
        int id_c = CTZ(~legal_w & deck);
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
      if (PRINTING > 7 || depth < depth_print) {
        for (int i = 0; i < depth; i++) cout << "  ";
        cout << "card: ";
        print_card(c, g.trump);
      }
#endif

      algo.init_alpha(alpha);

      // compute rec value
      auto update_save = update_card(g, c);
      unsigned new_valid = valid_with[id_c];
      while (new_valid) {  // remove c from valid worlds
        int id_w = CTZ(new_valid);
        new_valid &= ~(((unsigned)1) << id_w);
        w[id_w][id] &= ~c;
      }
      new_valid = valid_with[id_c];
      res = template_ab_aux(card_res, v, g, w, new_valid, alpha, parent, algo,
                            depth + 1);
      while (new_valid) {  // put c again in every valid world
        int id_w = CTZ(new_valid);
        new_valid &= ~(((unsigned)1) << id_w);
        w[id_w][id] |= c;
      }
      g.removeCard(update_save);

#if PRINTING > 6
      if (PRINTING > 7 || depth < depth_print) {
        for (int i = 0; i < depth; i++) cout << "  ";
        cout << "value: ";
        algo.print_value(v);
      }
#endif

      if (!algo.criterion(id, r, v)) {
        algo.copy(v, r);
        card_res = c;
#if (PRINTING > 8)
        cout << "update card_res: ";
        print_card(card_res, g.trump);
        cout << "r: ";
        algo.print_value(r);
        cout << "v: ";
        algo.print_value(v);
#endif
      }
    }
    algo.cleanup(&v);
  }  // CASE 0 < DEPTH < MAX
  else {
    int id = g.turn;
    int tm = g.team[id];
    Value alpha_save;
    algo.alloc(&alpha_save);
    algo.copy(alpha[tm], alpha_save);
    int parent_save = parent[tm];
    parent[tm] = id;
    Value v;
    algo.alloc(&v);
    algo.init_value(id, g, valid_worlds, r);

#if PRINTING > 6
    if (PRINTING > 7 || depth < depth_print) {
      for (int i = 0; i < depth; i++) cout << "  ";
      cout << "id: " << id << endl;
      for (int i = 0; i < depth; i++) cout << "  ";
      cout << "init_value: ";
      algo.print_value(r);
      for (int i = 0; i < depth; i++) cout << "  ";
      cout << "alpha[0]: ";
      algo.print_value(alpha[0]);
      for (int i = 0; i < depth; i++) cout << "  ";
      cout << "alpha[1]: ";
      algo.print_value(alpha[1]);
    }
#endif

    // compute possible and new valid worlds
    card possible = 0;
    unsigned valid_worlds_copy = valid_worlds;
    unsigned valid_with[N_CARDS];
    for (int i = 0; i < N_CARDS; i++) {
      valid_with[i] = valid_worlds;
    }
    while (valid_worlds_copy) {
      int id_w = CTZ(valid_worlds_copy);
      unsigned vc_w = ((unsigned)1) << id_w;
      valid_worlds_copy &= ~vc_w;

      card legal_w = legal(w[id_w][id], g);

      possible |= reduce_legal(legal_w, g);
      while (~legal_w & deck) {
        int id_c = CTZ(~legal_w & deck);
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
      if (PRINTING > 7 || depth < depth_print) {
        for (int i = 0; i < depth; i++) cout << "  ";
        cout << "card: ";
        print_card(c, g.trump);
      }
#endif

      // update alpha and check pruning condition
      algo.fusion(id, r, alpha[tm], alpha[tm]);

#if (PRINTING > 8)
      for (int i = 0; i < depth; i++) cout << "  ";
      cout << "update alpha: ";
      algo.print_value(alpha[tm]);
#endif

      for (int i = 0; i < N_TEAMS; i++) {
        if (i != tm && algo.better(parent[i], alpha[i], r)) {
#if PRINTING > 8
          for (int i = 0; i < depth; i++) cout << "  ";
          cout << "pruning: " << endl;
          for (int i = 0; i < depth; i++) cout << "  ";
          cout << "alpha[" << i << " ]: ";
          algo.print_value(alpha[i]);
          for (int i = 0; i < depth; i++) cout << "  ";
          cout << "r ";
          algo.print_value(r);
#endif
          res = i;
          goto end;
        }
      }

      // compute rec value
      auto update_save = update_card(g, c);
      unsigned new_valid = valid_with[id_c];
      while (new_valid) {  // remove c from valid worlds
        int id_w = CTZ(new_valid);
        new_valid &= ~(((unsigned)1) << id_w);
        w[id_w][id] &= ~c;
      }
      new_valid = valid_with[id_c];
      res = template_ab_aux(card_res, v, g, w, new_valid, alpha, parent, algo,
                            depth + 1);
      while (new_valid) {  // put c again in every valid world
        int id_w = CTZ(new_valid);
        new_valid &= ~(((unsigned)1) << id_w);
        w[id_w][id] |= c;
      }
      g.removeCard(update_save);

#if PRINTING > 6
      if (PRINTING > 7 || depth < depth_print) {
        for (int i = 0; i < depth; i++) cout << "  ";
        cout << "value: ";
        algo.print_value(v);
      }
#endif

      // update r with v
      if (res != -1) {
        if (res == tm) {
#if (PRINTING > 8)
          for (int i = 0; i < depth; i++) cout << "  ";
          cout << "pruning allies asked" << endl;
#endif
          res = -1;
          continue;
        } else {
#if (PRINTING > 8)
          for (int i = 0; i < depth; i++) cout << "  ";
          cout << "pruning enemies asked" << endl;
#endif
          goto end;
        }
      }

      algo.fusion(id, v, r, r);
#if (PRINTING > 8)
      for (int i = 0; i < depth; i++) cout << "  ";
      cout << "update r: ";
      algo.print_value(r);
#endif
    }
  end:
    algo.copy(alpha_save, alpha[tm]);
    parent[tm] = parent_save;
    algo.cleanup(&alpha_save);
    algo.cleanup(&v);
  }
  return res;
}