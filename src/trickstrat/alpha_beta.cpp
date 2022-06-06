#include "alpha_beta.h"

unordered_map<llu, pair<llu, card>> H_game;
unordered_map<llu, llu> H_equi;
int rec;

std::chrono::duration<double, std::milli> order_t, hash_t, playable_t, update_t,
    other_t;

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

llu snapeq(Game g, int n_playable) {
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
  res *= SIZE_HAND;
  res += n_playable;
  return res;
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
  if (x % 2) return -((x - 1) / 2);
  return (x / 2);
}

void order(list<card> *possible, Game game) {
  list<card> res;
  llu equi = snapeq(game, possible->size());
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
  int alpha[N_PLAYERS];
  for (int i = 0; i < N_PLAYERS; i++) alpha[i] = INT32_MIN;
  rec = 0;

  auto start = std::chrono::high_resolution_clock::now();
  alpha_beta_aux(&game, have_not, alpha);
  auto end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double, std::milli> total_t = end - start;

  cout << "rec " << rec << endl;
  cout << sco(H_game[snapg(game)].first, 0) << " - "
       << sco(H_game[snapg(game)].first, 1) << endl;
  card possible = H_game[snapg(game)].second;
  cout << "possible ";
  print_card(possible, game.trump);
  cout << "PERFORMANCE EVALUATION" << endl;
  cout << "ordering time: " << order_t.count() << " ms" << endl;
  cout << "hash time: " << hash_t.count() << " ms" << endl;
  cout << "playable time: " << playable_t.count() << " ms" << endl;
  cout << "update time: " << update_t.count() << " ms" << endl;
  cout << "other time: " << other_t.count() << " ms" << endl;
  cout << "total time: " << total_t.count() << " ms" << endl;

  return set_cards(possible).front();
}

void alpha_beta_aux(Game *game, card *have_not, int *alpha) {
  rec++;

  int hj = game->round * N_PLAYERS + game->trick.size();

  llu g = snapg(*game);

  auto luke = std::chrono::high_resolution_clock::now();
  auto vador = std::chrono::high_resolution_clock::now();

  if (H_game.find(g) != H_game.end()) {
    vador = std::chrono::high_resolution_clock::now();
    hash_t += vador - luke;
    return;
  }

  vador = std::chrono::high_resolution_clock::now();
  hash_t += vador - luke;

  llu s;
  int id = game->turn;
  int a_init = alpha[id];
  int sco_id;

  luke = std::chrono::high_resolution_clock::now();
  other_t += luke - vador;

  card playable_cards = playable(~have_not[id], *game);

  vador = std::chrono::high_resolution_clock::now();
  playable_t += vador - luke;

  list<card> possible = set_cards(playable_cards);

  luke = std::chrono::high_resolution_clock::now();
  other_t += luke - vador;

  order(&possible, *game);
  if (hj == 0) {
    cout << possible.size() << " playable cards" << endl;
  }

  vador = std::chrono::high_resolution_clock::now();
  order_t += vador - luke;

  if (end_trickgame(game))
    s = snaps(*game);
  else {
    for (card c : possible) {
      if (hj == 0) cout << "|" << endl;

      luke = std::chrono::high_resolution_clock::now();
      other_t += luke - vador;

      auto info = update_card(game, c);
      have_not[id] |= c;

      vador = std::chrono::high_resolution_clock::now();
      update_t += vador - luke;

      alpha_beta_aux(game, have_not, alpha);

      luke = std::chrono::high_resolution_clock::now();
      s = H_game[snapg(*game)].first;
      vador = std::chrono::high_resolution_clock::now();
      hash_t += vador - luke;

      game->removeCard(info);
      have_not[id] &= ~c;

      luke = std::chrono::high_resolution_clock::now();
      update_t += luke - vador;

      sco_id = sco(s, id);
      if (sco_id >= alpha[id]) {
        if (sco_id > alpha[id]) {
          vador = std::chrono::high_resolution_clock::now();
          other_t += vador - luke;

          H_game[g] = {s, c};

          luke = std::chrono::high_resolution_clock::now();
          hash_t += luke - vador;
        } else {
          vador = std::chrono::high_resolution_clock::now();
          other_t += vador - luke;

          card setc = H_game[g].second;
          H_game[g] = {s, setc | c};

          luke = std::chrono::high_resolution_clock::now();
          hash_t += luke - vador;
        }
      }

      for (int p = 0; p < N_PLAYERS; p++)
        if (game->team[p] != game->team[id]) {
          if (sco(s, p) < alpha[p]) {
            vador = std::chrono::high_resolution_clock::now();
            other_t += vador - luke;

            H_game.erase(g);

            luke = std::chrono::high_resolution_clock::now();
            hash_t += luke - vador;
            goto prune_beta;
          }
        } else if (sco_id > alpha[p])
          alpha[p] = sco_id;
    }

    vador = std::chrono::high_resolution_clock::now();
    llu equi = snapeq(*game, __builtin_popcountll(playable_cards));

    luke = std::chrono::high_resolution_clock::now();
    other_t += luke - vador;
    H_equi[equi] = g;

    vador = std::chrono::high_resolution_clock::now();
    hash_t += vador - luke;
  }
prune_beta:
  vador = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N_PLAYERS; i++)
    if (game->team[i] == game->team[id]) alpha[i] = a_init;

  luke = std::chrono::high_resolution_clock::now();
  other_t += luke - vador;
  if (H_game.find(g) == H_game.end()) H_game[g] = {s, 0};

  vador = std::chrono::high_resolution_clock::now();
  hash_t += vador - luke;

  if (hj == 0) cout << endl;
}