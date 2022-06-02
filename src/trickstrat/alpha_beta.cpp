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

void print_a(int *a) {
  cout << "a[";
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
  if (x % 2) return -((x - 1) / 2);
  return (x / 2);
}

void order(list<card> *possible, Game game) {
  card previous = 0;
  list<card> res;
  llu snapequi = snapeq(game, possible->size());
  card recom = 0;
  if (H_equi.find(snapequi) != H_equi.end())
    recom = H_game[H_equi[snapequi]].second;
  card masks[2] = {recom, ~recom};
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

card ab(Game game, int id, card hand, card *have_not) {
  H_game.clear();
  H_equi.clear();
  int alpha[N_PLAYERS];
  for (int i = 0; i < N_PLAYERS; i++) {
    if (game.team[i] == game.team[0])
      alpha[i] = INT32_MIN;
    else
      alpha[i] = INT32_MIN;
  }
  rec = 0;
  ab_aux(&game, have_not, alpha);
  cout << "rec " << rec << endl;
  cout << sco(H_game[snapg(game)].first, 0) << " - "
       << sco(H_game[snapg(game)].first, 1) << endl;
  card possible = H_game[snapg(game)].second;
  cout << "possible ";
  print_card(possible, game.trump);
  return set_cards(possible).front();
}

void ab_aux(Game *game, card *have_not, int *alpha) {
  string blank;
  int hj = game->round * N_PLAYERS + game->trick.size();
  for (int i = 0; i < hj; i++) blank.append("  ");
  bool printing = false;
  rec++;

  llu g = snapg(*game);
  int id = game->turn;
  if (printing) {
    cout << blank << "ab" << id << " g" << g << " ";
    print_a(alpha);
  }
  if (H_game.find(g) != H_game.end()) {
    if (printing)
      cout << blank << "game g" << g << " already reg "
           << sco(H_game[g].first, 0) << " - " << sco(H_game[g].first, 1)
           << endl;
    return;
  }
  llu s;
  int a_init = alpha[id];
  int sco_id;

  card plybl = playable(~have_not[id], *game);
  list<card> possible = set_cards(plybl);
  card res1 = 0;
  card res2 = 0;
  for (auto x : possible) res1 |= x;
  order(&possible, *game);
  for (auto x : possible) res2 |= x;
  if (printing && res1 != res2) {
    cout << blank << "possible: ";
    print_card(res1, game->trump);
    cout << blank << "after or: ";
    print_card(res2, game->trump);
  }

  if (end_trickgame(game)) {
    s = snaps(*game);
    if (printing) {
      cout << blank << "game over round" << game->round << " "
           << game->points[0] << "-" << game->points[1] << endl;
      cout << blank << sco(s, 0) << " - " << sco(s, 1) << endl;
    }
  } else {
    for (card c : possible) {
      if (printing) {
        cout << blank << "test card ";
        print_card(c, game->trump);
      }

      auto info = update_card(game, c);
      have_not[id] |= c;
      ab_aux(game, have_not, alpha);

      if (printing) cout << blank << "finding g" << snapg(*game) << endl;

      s = H_game[snapg(*game)].first;
      game->removeCard(info);
      have_not[id] &= ~c;

      sco_id = sco(s, id);
      if (sco_id >= alpha[id]) {
        if (sco_id > alpha[id]) {
          H_game[g] = {s, c};
        } else {
          card setc = H_game[g].second;
          H_game[g] = {s, setc | c};
        }
        if (printing) {
          cout << blank << sco_id << " >= ";
          print_a(alpha);
        }
      }
      for (int p = 0; p < N_PLAYERS; p++) {
        if (game->team[p] != game->team[id]) {
          if (sco(s, p) < alpha[p]) {
            H_game.erase(g);
            if (printing) cout << blank << "prune" << endl;
            goto prune_beta;
          }
        } else if (sco_id > alpha[p])
          alpha[p] = sco_id;
      }
      if (printing) {
        cout << blank << "after actu ";
        print_a(alpha);
      }
    }
    llu snapeqrder = snapeq(*game, __builtin_popcountll(plybl));
    H_equi[snapeqrder] = g;
  }
prune_beta:
  if (printing) cout << blank << "end g" << g << endl;
  for (int i = 0; i < N_PLAYERS; i++)
    if (game->team[i] == game->team[id]) alpha[i] = a_init;
  if (H_game.find(g) == H_game.end()) {
    H_game[g] = {s, 0};
  }
}