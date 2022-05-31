#include "alpha_beta.h"

hash_game H;
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

void print_a(int *a, string blank) {
  cout << blank << "a[";
  for (int i = 0; i < N_PLAYERS - 1; i++) cout << a[i] << ", ";
  cout << a[N_PLAYERS - 1] << "]" << endl;
}

llu snaps(Game g) {
  llu s = 0;
  for (int i = N_PLAYERS - 1; i >= 0; i--) {
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
  x = x % MAX_SCORE;
  if (x % 2) return (-x / 2);
  return (x / 2);
}

card ab(Game game, int id, card hand, card *have_not) {
  H.clear();
  int alpha[N_PLAYERS];
  for (int i = 0; i < N_PLAYERS; i++) alpha[i] = INT32_MIN;
  rec = 0;
  ab_aux(&game, have_not, alpha);
  cout << "rec " << rec << endl;
  return H.find(snapg(game))->second.second;
}

void ab_aux(Game *game, card *have_not, int *alpha) {
  rec++;
  llu s = snaps(*game);
  int id = game->turn;
  int a_init = alpha[id];
  llu g = snapg(*game);
  if (H.find(g) != H.end()) return;

  for (card c : set_cards(playable(~have_not[id], *game))) {
    auto info = update_card(game, c);
    have_not[id] |= c;
    ab_aux(game, have_not, alpha);
    s = H.find(snapg(*game))->second.first;
    game->removeCard(info);
    have_not[id] &= ~c;

    if (sco(s, id) >= alpha[id]) {
      if (sco(s, id) > alpha[id]) {
        H.erase(g);
        alpha[id] = sco(s, id);
      }
      H.insert({g, {s, c}});
    }
    for (int p = 0; p < N_PLAYERS; p++)
      if (game->team[p] != game->team[id] && sco(s, p) < alpha[p]) {
        H.erase(g);
        H.insert({g, {s, c}});
        alpha[id] = a_init;
        return;
      }
  }
  alpha[id] = a_init;
  if (H.find(g) == H.end()) H.insert({g, {s, 0}});
}