#include "alpha_beta.h"

unordered_map<llu, pair<llu, card>> H_game;
unordered_map<llu, llu> H_equi;
int rec;

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

llu snaps(Game g) {
  llu s = 0;
  for (int i = N_TEAMS - 1; i >= 0; i--) {
    int x = score(g, i);
    if (x >= 0)
      s += 2 * x;
    else
      s += 2 * (-x) + 1;
    if (i) s *= (2 * MAX_SCORE);
  }
  return s;
}

int sco(llu x, int p) {
  for (int i = 0; i < p; i++) x = x / (2 * MAX_SCORE);
  x = x % (2 * MAX_SCORE);
  if (x % 2) return -((x - 1) / 2);
  return (x / 2);
}

void order(list<card> *possible, Game game) {
  list<card> res;
  llu equi = snapeq(game);
  card recom = 0;
  if (H_equi.find(equi) != H_equi.end()) recom = H_game[H_equi[equi]].second;
  card masks[2] = {recom, ~recom};
  card previous = 0;

  for (auto mask : masks)
    for (card c : *possible)
      if (c & mask) {
        if ((previous != 0) && (points_card(previous) == points_card(c)) &&
            ((higher(previous) & lower(c) & game.remaining) == 0))
          for (auto suit : suits)
            if ((suit & c) && (suit & previous)) {
              game.remaining &= ~c;
              goto no_add;
            }
        res.push_back(c);
      no_add:
        previous = c;
      }
  *possible = res;
}

card alpha_beta(Game game, int id, card hand, card *have_not) {
  H_game.clear();
  H_equi.clear();
  int alpha[N_TEAMS];
  for (int i = 0; i < N_TEAMS; i++) alpha[i] = INT32_MIN;
  rec = 0;

  auto start = std::chrono::high_resolution_clock::now();

  alpha_beta_aux(&game, have_not, alpha);

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> total_t = end - start;

  cout << "rec " << rec << endl;
  cout << "time " << total_t.count() << endl;
  cout << sco(H_game[snapg(game)].first, 0) << " - "
       << sco(H_game[snapg(game)].first, 1) << endl;
  card possible = H_game[snapg(game)].second;
  cout << "possible ";
  print_card(possible, game.trump);

  return set_cards(possible).front();
}

llu alpha_beta_aux(Game *game, card *have_not, int *alpha) {
  rec++;
  llu g = snapg(*game);

  if (game->trick.empty() && H_game.find(g) != H_game.end())
    return H_game[g].first;

  llu s, best_s = UINT64_MAX;
  card best_c = 0;
  int id = game->turn;
  int tm = game->team[id];
  int alpha_init = alpha[tm];
  int sco_tm;

  list<card> possible = set_cards(playable(~have_not[id], *game));
  order(&possible, *game);

  int hope_score = MAX_SCORE;
  for (int t = 0; t < N_TEAMS; t++)
    if (t != tm) hope_score -= score(*game, t);

  if ((hope_score < alpha[tm]) || end_trickgame(game))
    best_s = snaps(*game);
  else {
    for (card c : possible) {
      auto info = update_card(game, c);
      have_not[id] |= c;
      s = alpha_beta_aux(game, have_not, alpha);
      game->removeCard(info);
      have_not[id] &= ~c;
      sco_tm = sco(s, tm);

      if (sco_tm > alpha[tm]) {
        alpha[tm] = sco_tm;
        best_c = c;
        best_s = s;
      }
      for (int i = 0; i < N_TEAMS; i++)
        if ((i != tm) && (sco(s, i) <= alpha[i])) {
          best_s = UINT64_MAX;
          goto prune_beta;
        }
    }
    H_equi[snapeq(*game)] = g;
  }
prune_beta:
  alpha[tm] = alpha_init;
  if (best_s == UINT64_MAX) {
    best_s = s;
    best_c = 0;
  }
  H_game[g] = {best_s, best_c};
  return best_s;
}