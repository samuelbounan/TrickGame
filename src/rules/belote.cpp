#ifdef RULES_BELOTE
#include "belote.h"

/** N_SUITS = 4
 * N_CARDS = 32
 */

card clubs = (ONE << (N_CARDS / N_SUITS)) - 1;
card diamo = clubs << (N_CARDS / N_SUITS);
card heart = diamo << (N_CARDS / N_SUITS);
card spade = heart << (N_CARDS / N_SUITS);
card suits[N_SUITS] = {clubs, diamo, heart, spade};

char cardname[N_CARDS][80] = {
    "\u001b[32m\u2663\u001b[0m7", "\u001b[32m\u2663\u001b[0m8",
    "\u001b[32m\u2663\u001b[0m9", "\u001b[32m\u2663\u001b[0mJ",
    "\u001b[32m\u2663\u001b[0mQ", "\u001b[32m\u2663\u001b[0mK",
    "\u001b[32m\u2663\u001b[0mT", "\u001b[32m\u2663\u001b[0mA",
    "\u001b[33m\u25C6\u001b[0m7", "\u001b[33m\u25C6\u001b[0m8",
    "\u001b[33m\u25C6\u001b[0m9", "\u001b[33m\u25C6\u001b[0mJ",
    "\u001b[33m\u25C6\u001b[0mQ", "\u001b[33m\u25C6\u001b[0mK",
    "\u001b[33m\u25C6\u001b[0mT", "\u001b[33m\u25C6\u001b[0mA",
    "\u001b[31m\u2665\u001b[0m7", "\u001b[31m\u2665\u001b[0m8",
    "\u001b[31m\u2665\u001b[0m9", "\u001b[31m\u2665\u001b[0mJ",
    "\u001b[31m\u2665\u001b[0mQ", "\u001b[31m\u2665\u001b[0mK",
    "\u001b[31m\u2665\u001b[0mT", "\u001b[31m\u2665\u001b[0mA",
    "\u001b[34m\u2660\u001b[0m7", "\u001b[34m\u2660\u001b[0m8",
    "\u001b[34m\u2660\u001b[0m9", "\u001b[34m\u2660\u001b[0mJ",
    "\u001b[34m\u2660\u001b[0mQ", "\u001b[34m\u2660\u001b[0mK",
    "\u001b[34m\u2660\u001b[0mT", "\u001b[34m\u2660\u001b[0mA"};

bool end_bidding(Game *game) {
  game->trump = clubs;
  for (int i = 0; i < N_PLAYERS; i++) game->team[i] = i % 2;
  game->declarer = game->leader;
  game->turn = game->leader;
  return true;
}

card sort(card hand, const card &trump) {
  sort_high(&hand, 2, trump);
  sort_high(&hand, 2, trump);
  return hand;
}

card unsort(card hand, const card &trump) {
  unsort_high(&hand, 2, trump);
  unsort_high(&hand, 2, trump);
  return hand;
}

card legal(const card &hand, const Game &game) {
  if (game.trick.empty()) return hand;  // if leader everything is legal
  card first_played = game.trick.front();
  for (card suit : suits)
    if (first_played & suit) {
      if (hand & suit) {                   // hand has suit
        if (suit & game.trump) {           // suit required is trump
          card highs = (hand & suit);      // you should play higher if possible
          for (card c : game.trick)        // for each card played
            if (c & suit)                  // that is trump
              highs &= (~((c << 1) - 1));  // try to play higner
          if (highs)                       // if can play higher trump
            return highs;                  // play higher
        }
        return hand & suit;  // play suit
      }
      if ((hand & game.trump) &&  // if no suit but has trump
          (game.team[winner_trick(game)] !=
           game.team[game.turn]))    // and partner don't
                                     // win
        return (hand & game.trump);  // forced to cut
    }
  return hand;  // piss
}

int winner_trick(const Game &game) {
  if (game.trick.empty()) return game.leader;

  // init best_card and best_suit with the first card played
  card best_card = game.trick.front();
  card best_suit = 0;
  for (card suit : suits) {
    if (suit & best_card) {
      best_suit = suit;
      break;
    }
  }
  // for each card in the trick actualize the best vars
  int winner = 0;
  int p = 0;
  for (card c : game.trick) {
    if (c & best_suit &&
        c >= best_card) {  // best_suit is followed with higher value
      best_card = c;
      winner = p;
    }
    if ((best_suit != game.trump) && (c & game.trump)) {  // suit is cut
      best_card = c;
      best_suit = game.trump;  // best_suit becomes trump
      winner = p;
    }
    p++;
  }
  return (winner + game.leader) % N_PLAYERS;
}

int tab_points_card[N_CARDS] = {0,  0,  3, 4,  10, 11, 14, 20, 0,  0, 0,
                                2,  3,  4, 10, 11, 0,  0,  0,  2,  3, 4,
                                10, 11, 0, 0,  0,  2,  3,  4,  10, 11};

int points_card(const card &c) { return tab_points_card[CTZ(c)]; }

int points_trick(const Game &game) {
  int res = 0;
  for (card c : game.trick) res += points_card(c);
  if (game.round == N_ROUNDS - 1) res += 10;
  return res;
}

void sort_high(card *hand, int idx, const card &suit) {
  if (suit == 0) return;
  card c = ONE << (CTZ(suit << idx));
  card new_c = ONE << (sizeof(card) * 8 - 1 - __builtin_clzll(suit));
  card greater = higher(c) & suit;
  card shifted = (*hand & greater) >> 1;
  if (*hand & c) {
    *hand = (*hand & ~c & ~greater) | shifted | new_c;
  } else
    *hand = (*hand & ~greater) | shifted;
}

void unsort_high(card *hand, int idx, const card &suit) {
  if (suit == 0) return;
  card c = ONE << (sizeof(card) * 8 - 1 - __builtin_clzll(suit));
  card new_c = ONE << (CTZ(suit << idx));
  card greater = higher(new_c >> 1) & suit;
  card shifted = (*hand & (suit - c) & greater) << 1;
  if (*hand & c) {
    *hand = (*hand & ~c & ~((suit - c) & greater)) | shifted | new_c;
  } else
    *hand = (*hand & ~((suit - c) & greater)) | shifted;
}

bool end_trickgame(Game *game) { return (game->round >= N_ROUNDS); }

#endif