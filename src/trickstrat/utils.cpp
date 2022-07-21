#include "utils.h"

unordered_map<Game, bool> H_aos;
unordered_map<Game, int> H_game;
unordered_map<llu, card> H_equi;
int rec, n_prune, r_Hequi, r_Hgame, w_Hequi, w_Hgame;

llu snapeq(Game g) {
  llu res = 0;
  if (g.leader != g.turn) {
    card first_card = g.trick[g.leader];
    for (card suit : suits)
      if (suit & first_card) res = CTZ(suit);
    res *= N_TEAMS;
  }
  res += g.team[winner_trick(g)];
  res *= N_PLAYERS;
  res += g.turn;
  return res;
}

bool aos(Game &g, card *world, int threshold) {
  bool res = aos_aux(g, world, threshold);
  H_aos.clear();
  return res;
}

bool aos_aux(Game &g, card *world, int threshold) {
  // end case
  int team_decl = g.team[g.declarer];
  if (g.min_points >= threshold) return true;
  if (g.max_points < threshold) return false;

  // check if saved
  bool write_H = g.turn == g.leader && g.round < N_ROUNDS - 3;
  if (write_H) {
    if (H_aos.find(g) != H_aos.end()) return H_aos[g];
  }

  // test every possible card
  int id = g.turn;
  card possible = reduce_legal(legal(world[id], g), g);
  card c;
  bool max_node = (team_decl == g.team[id]);
  bool res = !max_node;
  pair<int, int> info;
  while (possible) {
    c = ONE << CTZ(possible);
    info = update_card(g, c);
    world[id] &= ~c;
    res = aos_aux(g, world, threshold);
    world[id] |= c;
    g.removeCard(info);
    possible &= ~c;
    if (res == max_node) break;
  }
  if (write_H) H_aos[g] = res;
  return res;
}

pair<int, card> solver(Game &game, card *world, int max_depth_rd,
                       int n_sample) {
  int n_prune = 0;
  int r_Hequi = 0;
  int r_Hgame = 0;
  int w_Hequi = 0;
  int w_Hgame = 0;
  int rec = 0;
  // run aux
  auto start = std::chrono::high_resolution_clock::now();
  int s = solver_aux(game, world, 0, MAX_SCORE, 0, max_depth_rd, n_sample, rec,
                     n_prune, r_Hequi, r_Hgame, w_Hequi, w_Hgame);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> total_t = end - start;

  card possible = H_equi[snapeq(game)];

  if (PRINTING > 9) {
    cout << " / time " << total_t.count() << "ms / ";
    cout << s << " pts" << endl;
    print_card(possible, game.trump);
  }

  H_game.clear();
  H_equi.clear();
  return {s, possible};
}

int solver_aux(Game &g, card *world, int alpha, int beta, int depth,
               int max_depth_rd, int n_sample, int &rec, int &n_prune,
               int &r_Hequi, int &r_Hgame, int &w_Hequi, int &w_Hgame) {
  rec++;
  // end case
  if (end_trickgame(g)) {
    return g.min_points;
  }
  if (g.turn == g.leader) {
    auto got = H_game.find(g);
    if (got != H_game.end()) {
      r_Hgame++;
      return got->second;
    }
  }
  if (depth >= max_depth_rd) {
    int res = approx_score(g, world, n_sample);
    if (g.turn == g.leader) H_game[g] = res;
    return res;
  }

  // rec case
  int id = g.turn;
  bool max_node = (g.team[id] == g.team[g.declarer]);
  card c, card_res = 0;
  int s, v;
  if (max_node)
    v = g.min_points;
  else
    v = g.max_points;
  if (alpha > g.max_points || beta < g.min_points) return v;

  // order search
  card possible = reduce_legal(legal(world[id], g), g);
  bool cond_equi = (POPCOUNT(possible) > 1) && (g.turn != g.leader);
  card masks[2] = {possible, 0};

  unordered_map<llu, card>::const_iterator got_eq = H_equi.end();
  if (cond_equi) {
    got_eq = H_equi.find(snapeq(g));
    if (got_eq != H_equi.end()) {
      r_Hequi++;
      card recom = got_eq->second;
      masks[0] = recom & possible;
      masks[1] = (~recom) & possible;
    }
  }
  // main loop
  for (card mask : masks)
    while (mask) {
      // rec
      c = ONE << CTZ(mask);
      mask &= ~c;
      auto info = update_card(g, c);
      world[id] &= ~c;
      s = solver_aux(g, world, alpha, beta, depth + 1, max_depth_rd, n_sample,
                     rec, n_prune, r_Hequi, r_Hgame, w_Hequi, w_Hgame);
      g.removeCard(info);
      world[id] |= c;

      if (max_node) {
        if (s == v) {
          card_res |= c;
        } else if (s > v) {
          if (s > alpha) alpha = s;
          card_res = c;
          v = s;
        }
      } else {
        if (s == v) {
          card_res |= c;
        } else if (s < v) {
          if (s < beta) beta = s;
          card_res = c;
          v = s;
        }
      }

      if (alpha >= beta) {
        n_prune++;
        return v;
      }
    }
  if (cond_equi || depth == 0) {
    llu g_equi = snapeq(g);
    if ((cond_equi && got_eq == H_equi.end()) || depth == 0) {
      w_Hequi++;
      H_equi[g_equi] = card_res;
    }
  }
  if (id == g.leader) {
    w_Hgame++;
    H_game[g] = v;
  }
  return v;
}

int approx_score(Game &g, card *world, int n_sample) {
  int i, j, rd;
  int res = 0;
  int world_cp[N_PLAYERS];
  card possible, c;
  Game g_cp;

  for (i = 0; i < n_sample; i++) {
    g_cp = g;
    copy_n(world, N_PLAYERS, world_cp);
    while (!end_trickgame(g_cp)) {
      possible = reduce_legal(legal(world_cp[g_cp.turn], g_cp), g_cp);
      rd = mt() % POPCOUNT(possible);
      c = ONE << CTZ(possible);
      for (j = 0; j < rd; j++) possible &= ~c;
      world_cp[g_cp.turn] &= ~c;
      update_card(g_cp, c);
    }
    res += g_cp.min_points;
  }
  return res /= n_sample;
}

void random_world(card *res, card *have_not, const card &hand, Game game) {
try_again:

  // add information of hand to res
  copy_n(have_not, N_PLAYERS, res);
  for (int p = 0; p < N_PLAYERS; p++)
    if (p != game.turn)
      res[p] |= hand;
    else
      res[p] = ~hand;

  // compute the number of cards of each player
  int n_cards[N_PLAYERS];
  for (int i = 0; i < N_PLAYERS; i++) n_cards[i] = SIZE_HAND - game.round;
  for (int p = game.leader; p != game.turn; p = ((p + 1) % N_PLAYERS))
    n_cards[p] -= 1;

  // for each player, set his hand
  for (int p = 0; p < N_PLAYERS; p++) {
    // remove the cards already set
    for (int i = 0; i < p; i++) res[p] |= ~res[i];

    // only is cards that only p can have
    card only = ~res[p];
    for (int i = p + 1; i < N_PLAYERS; i++) only &= res[i];

    int waste = POPCOUNT(only & deck) - n_cards[p];
    if (waste > 0) {
      list<card> only_list = set_cards(only);
      vector<card> only_v(only_list.begin(), only_list.end());
      std::shuffle(only_v.begin(), only_v.end(), mt);
      for (int i = 0; i < waste; i++) {
        res[p] |= only_v[i];
      }
    } else {
      // poss_list are the cards that p could have or not
      list<card> poss_list = set_cards(~res[p] & ~only);
      vector<card> poss(poss_list.begin(), poss_list.end());
      std::shuffle(poss.begin(), poss.end(), mt);

      // while p has too much cards possible add one card to res
      unsigned k = 0;
      while ((N_CARDS - POPCOUNT(res[p] & deck)) > n_cards[p]) {
        if (k >= poss.size()) goto try_again;
        res[p] |= poss[k];
        k++;
      }
    }
  }
  for (int i = 0; i < N_PLAYERS; i++) {
    res[i] = ~res[i] & deck;
    if (PRINTING >= 6) print_card(res[i], game.trump);
  }
}