#ifndef __GAME
#define __GAME

#include <algorithm>

#include "card.h"

/* represents the public current state of a game */
class Game {
 public:
  /* constants of a game */
  int team[N_PLAYERS];  // all players have a team
  int declarer;         // the one who took the contract
  card trump = 0;       // suit of the contract

  /* modified during the game */
  card trick[N_PLAYERS];  // list (queue) of cards played during the round
  card played[N_ROUNDS]
             [N_PLAYERS];     // cards played by each player at each round
  card remaining = deck;      // cards not played yet
  int points[N_TEAMS] = {0};  // points of each team
  int round = 0;              // index of the round
  int leader;                 // player at the beginning of current trick
  int turn;                   // player that has to play

  Game(int first = 0) : leader(first){};

  bool operator==(const Game& other) const {
    bool res = (remaining == other.remaining && leader == other.leader &&
                turn == other.turn);
    for (int i = 0; i < N_TEAMS; i++) {
      if (points[i] != other.points[i]) res = false;
    }
    return res;
  }

  /**
   * @brief ajust game parameters when a card is played
   *
   * @param c card played
   */
  void newTurn(const card& c);

  /**
   * @brief ajust game parameters when a round is over
   *
   * @param winner of the trick played
   * @param pts points in the trick going to winning team
   */
  void newRound(int winner, int pts);

  void removeCard(const pair<int, int>& info);

  void print();
};

template <>
struct std::hash<Game> {
  std::size_t operator()(const Game& g) const {
    using std::hash;
    unsigned x = g.turn * N_PLAYERS + g.leader;
    for (int i = 0; i < N_TEAMS; i++) {
      x *= (MAX_SCORE + 1);
      x += g.points[i];
    }
    return (hash<card>()(g.remaining) ^ hash<unsigned>()(x));
  }
};

#endif