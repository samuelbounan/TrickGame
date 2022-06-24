#include "utils.h"

unordered_map<llu, bool> H_aos;
unordered_map<llu, pair<llu, card>> H_game;
unordered_map<llu, llu> H_equi;
int rec, n_equi, n_sample, max_equi, max_depth;

llu snapg(Game g) {
  llu res = g.points[g.leader] * N_PLAYERS + g.leader;
  for (card c : g.trick) {
    res *= N_CARDS;
    res += __builtin_ctzll(c);
  }
  res = (res << N_CARDS);
  card rem = g.remaining;
  return res + rem;
}

llu snapeq(Game g) {
  llu res = 0;
  if (!g.trick.empty()) {
    card first_card = g.trick.front();
    for (card suit : suits)
      if (suit & first_card) res = __builtin_ctzll(suit);
    res *= N_PLAYERS;
  }
  res += g.team[g.leader];
  res *= N_PLAYERS;
  res += g.turn;
  return res;
}

llu snaps(int *scores) {
  llu s = 0;
  for (int i = N_TEAMS - 1; i >= 0; i--) {
    int x = scores[i];
    if (x >= 0)
      s += 2 * x;
    else
      s += 2 * (-x) + 1;
    if (i) s *= (2 * (MAX_SCORE + 1));
  }
  return s;
}

int sco(llu x, int p) {
  for (int i = 0; i < p; i++) x = x / (2 * (MAX_SCORE + 1));
  x = x % (2 * (MAX_SCORE + 1));
  if (x % 2) return -((x - 1) / 2);
  return (x / 2);
}

bool aos(Game *g, card *world, int threshold) {
  bool res = aos_aux(g, world, threshold);
  H_aos.clear();
  return res;
}

bool aos_aux(Game *g, card *world, int threshold) {
  // end case
  if (g->points[g->declarer] >= threshold) return true;
  if (g->points[(g->declarer + 1) % N_PLAYERS] > MAX_SCORE - threshold)
    return false;

  // check if saved
  bool write_H = g->trick.empty();
  llu sg;
  if (write_H) {
    sg = snapg(*g);
    if (H_aos.find(sg) != H_aos.end()) return H_aos[sg];
  }

  // remove equivalent cards
  int id = g->turn;
  card possible = 0;
  card previous = 0;
  card playb = playable(world[id], *g);
  card c;
  card rem_eq = g->remaining;
  while (playb) {
    c = ONE << __builtin_ctzll(playb);
    possible |= c;
    if (previous && (points_card(previous) == points_card(c)) &&
        ((higher(previous) & lower(c) & rem_eq) == 0))
      for (auto suit : suits)
        if ((suit & c) && (suit & previous)) {
          rem_eq &= ~c;
          possible &= ~c;
        }
    previous = c;
    playb &= ~c;
  }

  // test every possible card
  bool decl_team = (g->team[g->declarer] == g->team[id]);
  pair<int, int> info;
  bool res_aos;
  bool explore = true;
  while (possible && explore) {
    c = ONE << __builtin_ctzll(possible);
    info = update_card(g, c);
    world[id] &= ~c;
    res_aos = aos_aux(g, world, threshold);
    if ((res_aos && decl_team) || (!res_aos && !decl_team)) {
      explore = false;
      decl_team = !decl_team;
    }
    world[id] |= c;
    g->removeCard(info);
    possible &= ~c;
  }
  if (write_H) H_aos[sg] = !decl_team;
  return !decl_team;
}

card alpha_beta(Game game, int id, card hand, card *world, int max_depth_ab,
                int n_sample_ab) {
  // hyperparameters
  rec = 0;
  n_equi = 0;
  max_equi = 1000;
  max_depth = max_depth_ab;
  n_sample = n_sample_ab;

  // run aux
  int alpha[N_TEAMS];
  for (int i = 0; i < N_TEAMS; i++) alpha[i] = -1;

  if (PRINTING >= 7) cout << "run ab_aux" << endl;

  auto start = std::chrono::high_resolution_clock::now();
  alpha_beta_aux(&game, world, alpha, 0);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> total_t = end - start;

  card possible = H_game[snapg(game)].second;

  if (PRINTING >= 6) {
    cout << "max depth " << max_depth << " / ";
    cout << "rec " << rec << " / time " << total_t.count() << "ms / ";
    cout << sco(H_game[snapg(game)].first, 0) << " - "
         << sco(H_game[snapg(game)].first, 1) << endl;
    print_card(possible, game.trump);
  }

  H_game.clear();
  H_equi.clear();
  return possible;
}

llu approx_score(Game g, card *world) {
  int i, j, rd;
  rec += n_sample * ((N_ROUNDS - g.round) * N_PLAYERS - g.trick.size());
  int res[N_TEAMS] = {0};
  int world_cp[N_PLAYERS];
  card possible, c;
  Game g_cp;

  for (i = 0; i < n_sample; i++) {
    g_cp = g;
    copy_n(world, N_PLAYERS, world_cp);
    while (!end_trickgame(&g_cp)) {
      possible = playable(world_cp[g_cp.turn], g_cp);
      rd = mt() % __builtin_popcountll(possible);
      for (j = 0; j < rd; j++) possible &= ~(ONE << __builtin_ctzll(possible));
      c = ONE << __builtin_ctzll(possible);
      world_cp[g_cp.turn] &= ~c;
      update_card(&g_cp, c);
    }
    for (j = 0; j < N_TEAMS; j++) res[j] += score(g_cp, j);
  }
  for (int t = 0; t < N_TEAMS; t++) res[t] /= n_sample;
  return snaps(res);
}

llu alpha_beta_aux(Game *game, card *world, int *alpha, int depth) {
  llu opt_s = UINT64_MAX;
  llu g = snapg(*game);
  int i, id = game->turn;
  int tm = game->team[id];
  rec++;
  if (game->trick.empty() && H_game.find(g) != H_game.end())
    return H_game[g].first;

  // end case
  int scores[N_TEAMS];
  bool end_search = end_trickgame(game);
  int points_left = MAX_SCORE;
  for (i = 0; i < N_TEAMS; i++) {
    scores[i] = score(*game, i);
    points_left -= scores[i];
  }
  for (i = 0; i < N_TEAMS; i++)
    if (points_left + scores[i] < alpha[i]) {
      scores[i] += points_left;
      end_search = true;
    }
  if (end_search)
    opt_s = snaps(scores);
  else if (depth >= max_depth)
    opt_s = approx_score(*game, world);

  // rec case
  else {
    card c, opt_c = 0;
    llu s;
    int alpha_init = alpha[tm];
    int i, sco_tm;

    // order search
    card possible = 0;
    card previous = 0;
    card playb = playable(world[id], *game);
    card rem_eq = game->remaining;
    while (playb) {
      c = ONE << __builtin_ctzll(playb);
      possible |= c;
      if (previous && (points_card(previous) == points_card(c)) &&
          ((higher(previous) & lower(c) & rem_eq) == 0))
        for (auto suit : suits)
          if ((suit & c) && (suit & previous)) {
            rem_eq &= ~c;
            possible &= ~c;
          }
      previous = c;
      playb &= ~c;
    }
    bool save_equi = (__builtin_popcountll(possible) > 1 && n_equi < max_equi);
    card recom = 0;
    llu equi = snapeq(*game);
    if (H_equi.find(equi) != H_equi.end()) recom = H_game[H_equi[equi]].second;
    card masks[2] = {recom & possible, (~recom) & possible};

    // main loop
    for (card mask : masks)
      while (mask) {
        c = ONE << __builtin_ctzll(mask);
        auto info = update_card(game, c);
        world[id] &= ~c;
        s = alpha_beta_aux(game, world, alpha, depth + 1);
        game->removeCard(info);
        world[id] |= c;
        sco_tm = sco(s, tm);

#if PRINTING > 6
        if (PRINTING > 7 || depth < 1) {
          for (i = 0; i < depth; i++) cout << "  ";
          cout << "tested (" << sco_tm << "): ";
          print_card(c, game->trump);
        }
#endif

        if (sco_tm == alpha[tm]) opt_c |= c;
        if (sco_tm > alpha[tm]) {
          alpha[tm] = sco_tm;
          opt_c = c;
          opt_s = s;
        }
        for (i = 0; i < N_TEAMS; i++)
          if ((i != tm) && (sco(s, i) < alpha[i])) {
            opt_s = s;
            opt_c = 0;
            goto prune_beta;
          }
        mask &= ~c;
      }
    if (save_equi) {
      H_equi[snapeq(*game)] = g;
      n_equi++;
    }
    if (opt_s == UINT64_MAX) opt_s = s;
  prune_beta:
    alpha[tm] = alpha_init;
    if (depth == 0 || save_equi || game->trick.empty())
      H_game[g] = {opt_s, opt_c};
  }
  return opt_s;
}

void random_world(card *res, card *have_not, card hand, Game game) {
try_again:
  copy_n(have_not, N_PLAYERS, res);
  for (int p = 0; p < N_PLAYERS; p++)
    if (p != game.turn)
      res[p] |= hand;
    else
      res[p] = ~hand;
  int n_cards[N_PLAYERS];
  for (int i = 0; i < N_PLAYERS; i++) n_cards[i] = SIZE_HAND - game.round;
  for (int p = game.leader; p != game.turn; p = ((p + 1) % N_PLAYERS))
    n_cards[p] -= 1;

  for (int p = 0; p < N_PLAYERS; p++) {
    for (int i = 0; i < p; i++) res[p] |= ~res[i];
    card only = ~res[p];
    for (int i = p + 1; i < N_PLAYERS; i++)
      only &= res[i];  // only is cards that only p can have
    // all cards that can be removed from possible for p
    list<card> poss_list = set_cards(~res[p] & ~only);
    vector<card> poss(poss_list.begin(), poss_list.end());
    std::shuffle(poss.begin(), poss.end(), mt);
    auto it = poss.begin();
    unsigned k = 0;
    while ((N_CARDS - __builtin_popcountll(res[p])) != n_cards[p]) {
      if (k >= poss.size()) goto try_again;
      res[p] |= *(it++);
    }
  }
  for (int i = 0; i < N_PLAYERS; i++) {
    res[i] = ~res[i];
    if (PRINTING >= 6) print_card(res[i], game.trump);
  }
}