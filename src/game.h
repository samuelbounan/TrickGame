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
             [N_PLAYERS];  // cards played by each player at each round
  card remaining = deck;   // cards not played yet
  int min_points = 0;
  int max_points = MAX_SCORE;
  int round = 0;  // index of the round
  int leader;     // player at the beginning of current trick
  int turn;       // player that has to play

  Game(int first = 0) : leader(first){};

  inline bool operator==(const Game& other) const {
    return (remaining == other.remaining && leader == other.leader &&
            min_points == other.min_points && max_points == other.max_points);
  }

  /**
   * @brief ajust game parameters when a card is played
   *
   * @param c card played
   */
  inline void newTurn(const card& c) {
    trick[turn] = c;
    remaining &= ~c;
    turn = (turn + 1) % N_PLAYERS;
  }

  /**
   * @brief ajust game parameters when a round is over
   *
   * @param winner of the trick played
   * @param pts points in the trick going to winning team
   */
  inline void newRound(int winner, int pts) {
    if (team[winner] == team[declarer])
      min_points += pts;
    else
      max_points -= pts;
    copy_n(trick, N_PLAYERS, played[round]);
    round++;
    leader = winner;
    turn = leader;
  }

  inline void removeCard(const pair<int, int>& info) {
    if (turn == leader) {
      round--;
      if (team[leader] == team[declarer])
        min_points -= info.first;
      else
        max_points += info.first;
      turn = info.second;
      leader = (turn + 1) % N_PLAYERS;
      copy_n(played[round], N_PLAYERS, trick);
    } else
      turn = (turn - 1 + N_PLAYERS) % N_PLAYERS;
    remaining |= trick[turn];
  }

  void print() {
    cout << "GAME: dec " << declarer;
    cout << "/ tru ";
    print_vector(trump);
    cout << "points [" << min_points << ", " << max_points << "]/ ";
    cout << "rnd " << round;
    cout << "/ lea " << leader;
    cout << "/ trn " << turn;
    cout << "/ rem ";
    print_vector(remaining);
    cout << "trick:" << endl;
    for (int i = 0; i < N_PLAYERS; i++) print_vector(trick[i]);
    cout << endl;
  }
};

template <>
inline struct std::hash<Game> {
  std::size_t operator()(const Game& g) const {
    using std::hash;
    card x = (g.leader * (MAX_SCORE + 1) + g.min_points) * (MAX_SCORE + 1) +
             g.max_points;
    return (hash<card>()(g.remaining ^ x));
  }
};

#endif