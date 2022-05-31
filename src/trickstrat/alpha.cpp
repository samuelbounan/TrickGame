#include "alpha.h"

// function computing the alpha-beta pruning of solutions tree

card alpha(Game game, int id, card hand, card *have_not) {
  int res[N_PLAYERS];
  int alpha[N_PLAYERS];
  for (int i = 0; i < N_PLAYERS; i++) {
    alpha[i] = -9;
  }
  return alpha_search(res, game, id, hand, have_not, alpha);
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
  if (end_trickgame(&game)) {
    copy_n(game.points, N_PLAYERS, res);
    return deck;
  }

  // allocate space for parameters
  Game v_game;
  card v_have_not[N_PLAYERS];
  int alpha_local[N_PLAYERS];
  copy_n(alpha, N_PLAYERS, alpha_local);
  int score_obt;
  int points_obt[N_PLAYERS];
  card best_card = 0;

  // for each card c possible to play
  for (card c : set_cards(playable(hand, game))) {
    // update the card in the virtual game
    v_game = game;
    update_card(&v_game, c);
    copy_n(have_not, N_PLAYERS, v_have_not);
    v_have_not[id] |= c;

    // compute the points obtained in this situation
    alpha_search(points_obt, v_game, v_game.turn, ~v_have_not[v_game.turn],
                 v_have_not, alpha_local);
    copy_n(points_obt, N_PLAYERS, v_game.points);

    // update alpha and prune the tree
    for (int p = 0; p < N_PLAYERS; p++) {
      score_obt = score(v_game, p);

      // if same team check if better than alpha
      if (game.team[p] == game.team[id]) {
        if (score_obt > alpha_local[p]) {
          alpha_local[p] = score_obt;
          copy_n(points_obt, N_PLAYERS, res);
          best_card = c;
        }

        // if different team, check if less than alpha
      } else if (score_obt <= alpha_local[p]) {
        goto prune_branch;
      }
    }
  }
prune_branch:
  return best_card;
}