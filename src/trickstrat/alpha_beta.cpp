#include "alpha_beta.h"

hash_game H;

card ab(Game game, int id, card hand, card *have_not) {
  int alpha[N_PLAYERS];
  for (int x : alpha) x = INT32_MIN;
  ab_aux(&game, have_not, alpha);
  pair<int[N_PLAYERS], card> res = H.find(snapg(game))->second;
  return res.second;
}

llu snapg(Game g) {
  llu res = g.points[g.turn] * N_PLAYERS + g.turn;
  res = (res << N_CARDS);
  return res + g.remaining;
}

void ab_aux(Game *game, card *have_not, int *alpha) {
  int s[N_PLAYERS];
  copy_n(score(*game), N_PLAYERS, s);
  int id = game->turn;

  llu g = snapg(*game);
  if (H.find(g) == H.end()) return;

  for (card c : set_cards(playable(~have_not[id], *game))) {
    update_card(game, c);
    have_not[id] |= c;

    ab_aux(game, have_not, alpha);
    copy_n(H.find(g)->first, N_PLAYERS, s);

    game->removeCard();
    have_not[id] &= ~c;

    if (s[id] >= alpha[id]) {
      if (s[id] > alpha[id]) H.erase(g);
      H.emplace(make_pair(g, make_pair(s, c)));
    }

    for (int p = 0; p < N_PLAYERS; p++)
      if (game->team[p] == game->team[id] && s[p] > alpha[p])
        alpha[p] = s[p];
      else if (game->team[p] != game->team[id] && s[p] < alpha[p]) {
        H.erase(g);
        H.emplace(make_pair(g, make_pair(s, c)));
        return;
      }
  }
  if (H.find(g) == H.end()) H.emplace(make_pair(g, make_pair(s, 0)));
}