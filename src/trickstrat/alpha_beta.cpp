#include "alpha_beta.h"

unordered_map<llu, pair<llu, card>> H_game;
unordered_map<llu, llu> H_equi;
int rec, n_equi;
int max_depth = 16;
int n_sample = 3;
int max_equi = 1000;

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

card alpha_beta(Game game, int id, card hand, card *have_not) {
  H_game.clear();
  H_equi.clear();
  int alpha[N_TEAMS];
  for (int i = 0; i < N_TEAMS; i++) alpha[i] = -1;
  rec = 0;
  n_equi = 0;

  auto start = std::chrono::high_resolution_clock::now();
  alpha_beta_aux(&game, have_not, alpha, 0);

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> total_t = end - start;

  // cout << "max depth: " << max_depth << endl;
  // cout << "rec " << rec << " - time " << total_t.count() << endl;
  // cout << sco(H_game[snapg(game)].first, 0) << " - "
  //      << sco(H_game[snapg(game)].first, 1) << endl;
  card possible = H_game[snapg(game)].second;
  // cout << "possible ";
  // print_card(possible, game.trump);
  // cout << "total time: " << total_t.count() << " ms" << endl;

  return possible;
}

llu approx_score(Game g, card *have_not) {
  int i, j, rd;
  rec += n_sample * ((N_ROUNDS - g.round) * N_PLAYERS - g.trick.size());
  int res[N_TEAMS] = {0};
  int hand[N_PLAYERS];
  card possible, c;
  Game gv;

  for (i = 0; i < n_sample; i++) {
    gv = g;
    for (j = 0; j < N_PLAYERS; j++) hand[j] = ~have_not[j];
    while (!end_trickgame(&gv)) {
      possible = playable(hand[gv.turn], gv);
      rd = rand() % __builtin_popcountll(possible);
      for (j = 0; j < rd; j++) possible &= ~(ONE << __builtin_ctzll(possible));
      c = ONE << __builtin_ctzll(possible);
      hand[gv.turn] &= ~c;
      update_card(&gv, c);
    }
    for (j = 0; j < N_TEAMS; j++) res[j] += score(gv, j);
  }
  for (int t = 0; t < N_TEAMS; t++) res[t] /= n_sample;
  return snaps(res);
}

llu alpha_beta_aux(Game *game, card *have_not, int *alpha, int depth) {
  llu opt_s = UINT64_MAX;
  llu g = snapg(*game);
  rec++;
  if (game->trick.empty() && H_game.find(g) != H_game.end())
    return H_game[g].first;

  if (end_trickgame(game)) {
    int scores[N_TEAMS];
    for (int i = 0; i < N_TEAMS; i++) scores[i] = score(*game, i);
    opt_s = snaps(scores);
  } else if (depth >= max_depth)
    opt_s = approx_score(*game, have_not);

  else {
    card c, opt_c = 0;
    llu s;
    int i, id = game->turn;
    int tm = game->team[id];
    int alpha_init = alpha[tm];
    int sco_tm;
    card possible = 0;
    card previous = 0;
    card playb = playable(~have_not[id], *game);
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
    for (card mask : masks)
      while (mask) {
        c = ONE << __builtin_ctzll(mask);
        auto info = update_card(game, c);
        have_not[id] |= c;
        s = alpha_beta_aux(game, have_not, alpha, depth + 1);
        game->removeCard(info);
        have_not[id] &= ~c;
        sco_tm = sco(s, tm);

        // if (sco_tm == alpha[tm])
        //   opt_c |= c;
        if (sco_tm > alpha[tm]) {
          alpha[tm] = sco_tm;
          opt_c = c;
          opt_s = s;
        }
        for (i = 0; i < N_TEAMS; i++)
          if ((i != tm) && (sco(s, i) <= alpha[i])) {
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