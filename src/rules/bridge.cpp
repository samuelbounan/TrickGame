#ifdef RULES_BRIDGE

#include "../rules.h"

/*
  N_SUITS = 4
  N_CARDS = 52
*/

card clubs = (ONE << (N_CARDS / N_SUITS)) - 1;
card diamo = clubs << (N_CARDS / N_SUITS);
card heart = diamo << (N_CARDS / N_SUITS);
card spade = heart << (N_CARDS / N_SUITS);
card suits[N_SUITS] = {clubs, diamo, heart, spade};

char cardname[N_CARDS][80] = {"\u001b[32m\u2663\u001b[0m2", "\u001b[32m\u2663\u001b[0m3", "\u001b[32m\u2663\u001b[0m4", "\u001b[32m\u2663\u001b[0m5",
                              "\u001b[32m\u2663\u001b[0m6", "\u001b[32m\u2663\u001b[0m7", "\u001b[32m\u2663\u001b[0m8", "\u001b[32m\u2663\u001b[0m9",
                              "\u001b[32m\u2663\u001b[0mT",
                              "\u001b[32m\u2663\u001b[0mJ", "\u001b[32m\u2663\u001b[0mQ", "\u001b[32m\u2663\u001b[0mK",
                              "\u001b[32m\u2663\u001b[0mA", "\u001b[33m\u25C6\u001b[0m2", "\u001b[33m\u25C6\u001b[0m3", "\u001b[33m\u25C6\u001b[0m4",
                              "\u001b[33m\u25C6\u001b[0m5", "\u001b[33m\u25C6\u001b[0m6", "\u001b[33m\u25C6\u001b[0m7", "\u001b[33m\u25C6\u001b[0m8",
                              "\u001b[33m\u25C6\u001b[0m9", "\u001b[33m\u25C6\u001b[0mT", "\u001b[33m\u25C6\u001b[0mJ", "\u001b[33m\u25C6\u001b[0mQ", "\u001b[33m\u25C6\u001b[0mK",
                              "\u001b[33m\u25C6\u001b[0mA", "\u001b[31m\u2665\u001b[0m2", "\u001b[31m\u2665\u001b[0m3",
                              "\u001b[31m\u2665\u001b[0m4", "\u001b[31m\u2665\u001b[0m5", "\u001b[31m\u2665\u001b[0m6", "\u001b[31m\u2665\u001b[0m7",
                              "\u001b[31m\u2665\u001b[0m8", "\u001b[31m\u2665\u001b[0m9", "\u001b[31m\u2665\u001b[0mT", "\u001b[31m\u2665\u001b[0mJ", "\u001b[31m\u2665\u001b[0mQ",
                              "\u001b[31m\u2665\u001b[0mK", "\u001b[31m\u2665\u001b[0mA", "\u001b[34m\u2660\u001b[0m2",
                              "\u001b[34m\u2660\u001b[0m3", "\u001b[34m\u2660\u001b[0m4", "\u001b[34m\u2660\u001b[0m5", "\u001b[34m\u2660\u001b[0m6",
                              "\u001b[34m\u2660\u001b[0m7", "\u001b[34m\u2660\u001b[0m8", "\u001b[34m\u2660\u001b[0m9", "\u001b[34m\u2660\u001b[0mT", "\u001b[34m\u2660\u001b[0mJ",
                              "\u001b[34m\u2660\u001b[0mQ", "\u001b[34m\u2660\u001b[0mK", "\u001b[34m\u2660\u001b[0mA"};

bool end_bidding(Game &game) {
  game.trump = clubs;
  for (int i = 0; i < N_PLAYERS; i++) game.team[i] = i % 2;
  game.declarer = game.leader;
  game.leader = (game.leader + N_PLAYERS - 1) % N_PLAYERS;
  game.turn = game.leader;
  return true;
}

card sort(card hand, const card &trump) {
  return hand;
}

card unsort(card hand, const card &trump) { return hand; }

card legal(const card &hand, const Game &game) {
  if (game.turn == game.leader) return hand;  // if leader everything is legal
  card first_played = game.trick[game.leader];
  for (card suit : suits)
    if (suit & first_played) {
      card res = suit & hand;
      if (res) return res;  // play suit asked
    }
  return hand;  // piss
}

card reduce_legal(const card &h, const Game &g) {
  // init
  card possible = 0;
  card previous = 0;
  card rem_eq = g.remaining;
  card h_cp = h;

  // main loop
  while (h_cp) {
    card c = ONE << CTZ(h_cp);
    possible |= c;
    if (previous != 0 && ((higher(previous) & lower(c) & rem_eq) == 0))
      for (auto suit : suits)
        if ((suit & c) && (suit & previous)) {
          rem_eq &= ~c;
          possible &= ~c;
        }
    h_cp &= ~c;
    previous = c;
  }
  return possible;
}

int winner_trick(const Game &game) {
  // init best_card and best_suit with the first card played
  card best_card = game.trick[game.leader];
  card best_suit = 0;
  for (card suit : suits) {
    if (suit & best_card) {
      best_suit = suit;
      break;
    }
  }
  // for each card in the trick actualize the best vars
  int winner = game.leader;
  for (int i = (game.leader + 1) % N_PLAYERS; i != game.turn;
       i = ((i + 1) % N_PLAYERS)) {
    card c = game.trick[i];
    if (c & best_suit &&
        c >= best_card) {  // best_suit is followed with higher value
      best_card = c;
      winner = i;
    } else if (best_suit != game.trump && c & game.trump) {  // suit is cut
      best_card = c;
      best_suit = game.trump;  // best_suit becomes trump
      winner = i;
    }
  }
  return winner;
}

int points_trick(const Game &game) { return 1; }

bool end_trickgame(Game &game) { return (game.round >= N_ROUNDS); }

#endif