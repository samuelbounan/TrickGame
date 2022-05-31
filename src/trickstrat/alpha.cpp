#include "alpha.h"

int rec2;

// function computing the alpha-beta pruning of solutions tree

card alpha(Game game, int id, card hand, card *have_not) {
  rec2 = 0;
  int res[N_PLAYERS];
  int alpha[N_PLAYERS];
  for (int i = 0; i < N_PLAYERS; i++) {
    alpha[i] = INT32_MIN;
  }
  card c = alpha_search(res, game, id, hand, have_not, alpha);
  cout << "rec " << rec2 << endl;
  return c;
}

void print_points(int *points) {
  cout << "[";
  for (int i = 0; i < N_PLAYERS; i++) cout << points[i] << "; ";
  cout << "]" << endl;
}

// save in res the best points you can get from the current game knowing that
// each player p can enforce a score of alpha[p]
card alpha_search(int *res, Game game, int id, card hand, card *have_not,
                  int *alpha) {
  rec2++;
  bool printing = false;
  int hj = game.round * N_PLAYERS + game.trick.size();

  if (printing) {
    for (int i = 0; i < hj; i++) cout << "  ";
    cout << "alpha_search(" << id << ") " << endl;
    for (int i = 0; i < hj; i++) cout << "  ";
    cout << "alpha: ";
    print_points(alpha);
  }

  if (end_trickgame(&game)) {
    if (printing) {
      for (int i = 0; i < hj; i++) cout << "  ";
      cout << "game points: ";
      print_points(game.points);
    }
    copy_n(game.points, N_PLAYERS, res);

    if (printing) {
      for (int i = 0; i < hj; i++) cout << "  ";
      cout << "end_game res: ";
      print_points(res);
    }
    return deck;
  }

  // allocate space for parameters
  Game v_game;
  card v_have_not[N_PLAYERS];
  int alpha_local[N_PLAYERS];
  copy_n(alpha, N_PLAYERS, alpha_local);
  int points_obt[N_PLAYERS];
  int points_max[N_PLAYERS];
  card best_card;
  int score_obt;
  bool init = true;

  list<card> possible = set_cards(playable(hand, game));
  // possible.reverse();

  // for each card c possible to play
  for (card c : possible) {
    if (printing) {
      for (int i = 0; i < hj; i++) cout << "  ";
      cout << "test: ";
      print_card(c, 0);
    }

    // update the card in the virtual game
    v_game = game;
    update_card(&v_game, c);
    copy_n(have_not, N_PLAYERS, v_have_not);
    v_have_not[id] |= c;

    // compute the points obtained in this situation
    alpha_search(points_obt, v_game, v_game.turn, ~v_have_not[v_game.turn],
                 v_have_not, alpha_local);

    if (printing) {
      for (int i = 0; i < hj; i++) cout << "  ";
      cout << "points_obt: ";
      print_points(points_obt);
    }
    if (init) {
      copy_n(points_obt, N_PLAYERS, points_max);
      init = false;
    }
    copy_n(points_obt, N_PLAYERS, v_game.points);

    // update alpha and prune the tree
    for (int p = 0; p < N_PLAYERS; p++) {
      score_obt = score(v_game, p);
      // if same team check if better than alpha
      if (game.team[p] == game.team[id]) {
        if (score_obt > alpha_local[p]) {
          alpha_local[p] = score_obt;

          if (printing) {
            for (int i = 0; i < hj; i++) cout << "  ";
            cout << "update alpha: ";
            print_points(alpha_local);
          }

          copy_n(points_obt, N_PLAYERS, points_max);
          best_card = c;
        }

        // if different team, check if less than alpha
      } else {
        if (score_obt <= alpha_local[p]) {
          if (printing) {
            for (int i = 0; i < hj; i++) cout << "  ";
            cout << "prune" << endl;
          }
          copy_n(points_obt, N_PLAYERS, points_max);
          goto prune_branch;
        }
      }
    }
  }
prune_branch:
  copy_n(points_max, N_PLAYERS, res);
  return best_card;
}