#ifndef __PLAY
#define __PLAY

#include "player.h"

/**
 * @brief generates random hands of size_hand, update players
 *
 * @param player
 */
void deal_hands(Player *player, int seed);

/**
 * @brief performs the bidding phase
 *
 * @param game played
 * @param player playing
 */
void bidding(Game &game, Player *player, int printing);

/**
 * @brief add a card c to the trick
 *
 * @param game
 * @param c
 * @return {eventual points, former turn}
 */
inline pair<int, int> update_card(Game &game, const card &c) {
  // forfeiting
  if (c == 0) {
    cout << "forfeit" << endl;
    exit(0);
    game.round = N_ROUNDS;
    return {0, game.turn};
  }
  int pts = 0;
  int p = game.turn;
  game.newTurn(c);
  // end of a trick
  if (game.turn == game.leader) {
    int winner = winner_trick(game);
    pts = points_trick(game);
    game.newRound(winner, pts);
  }
  return {pts, p};
}

/**
 * @brief tricktaking game played until round = N_ROUNDS
 *
 * @param game
 * @param player
 * @param printing to print the game played
 */
void trickgame(Game &game, Player *player, int printing);

/**
 * @brief prints the game, and the players hands
 *
 * @param game
 * @param player
 */
void print(const Game &game, Player *player);

#endif