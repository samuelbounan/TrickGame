#ifdef RULES_CONTREE
#include "contree.h"

card clubs = (ONE << (N_CARDS / N_SUITS)) - 1;
card diamo = clubs << (N_CARDS / N_SUITS);
card heart = diamo << (N_CARDS / N_SUITS);
card spade = heart << (N_CARDS / N_SUITS);
card suits[N_SUITS] = {clubs, diamo, heart, spade};

char cardname[N_CARDS][80] = {
    "\u26637", "\u26638", "\u26639", "\u2663J", "\u2663Q", "\u2663K", "\u2663T",
    "\u2663A", "\u25C67", "\u25C68", "\u25C69", "\u25C6J", "\u25C6Q", "\u25C6K",
    "\u25C6T", "\u25C6A", "\u26657", "\u26658", "\u26659", "\u2665J", "\u2665Q",
    "\u2665K", "\u2665T", "\u2665A", "\u26607", "\u26608", "\u26609", "\u2660J",
    "\u2660Q", "\u2660K", "\u2660T", "\u2660A"};

card playable(card hand, Game game) {
  if (game.trick.empty()) return hand;  // if leader everything is playable
  card first_played = game.trick.front();
  for (card suit : suits)
    if (first_played & suit) {
      if (hand & suit) {               // hand has suit
        if (suit & game.trump) {       // suit required is trump
          card highs = (hand & suit);  // you should play higher if possible
          for (card c : game.trick)    // for each card played
            if (c & suit)              // that is trump
              highs &= higher(c);      // try to play higner
          if (highs)                   // if can play higher trump
            return highs;              // play higher
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

card sort(card *hand, card trump) {
  sort_high(hand, 2, trump);
  sort_high(hand, 2, trump);
  return *hand;
}

card unsort(card *hand, card trump) {
  unsort_high(hand, 2, trump);
  unsort_high(hand, 2, trump);
  return *hand;
}
// TODO
void sort_high(card *hand, int idx, card suit) {
  if (suit == 0) return;
  card c = _card(__builtin_ctzll(suit << idx));
  card new_c = (card)((long long unsigned)suit + 1) >> 1;
  card greater = higher(c) & suit;
  card shifted = (*hand & greater) >> 1;
  if (*hand & c) {
    *hand = (*hand & ~c & ~greater) | shifted | new_c;
  } else
    *hand = (*hand & ~greater) | shifted;
}

void unsort_high(card *hand, int idx, card suit) {
  if (suit == 0) return;
  card c = (card)((long long unsigned)suit + 1) >> 1;
  card new_c = _card(__builtin_ctzll(suit << idx));
  card greater = higher(new_c >> 1) & suit;
  card shifted = (*hand & (suit - c) & greater) << 1;
  if (*hand & c) {
    *hand = (*hand & ~c & ~((suit - c) & greater)) | shifted | new_c;
  } else
    *hand = (*hand & ~((suit - c) & greater)) | shifted;
}

int winner_trick(Game game) {
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

int points_trick(Game game) { return 1; }

void finish(Game *game) {}

// void forfeit(Game *game) {
//   for (int p = 0; p < N_PLAYERS; p++) {
//     if (game->team[p] == game->team[game->turn])
//       game->points[p] = -1;
//     else
//       game->points[p] = game->contract;
//   }
// }

bool won(int p, Game game) {
  if (game.points[game.declarer] >= game.contract)
    return game.team[p] == game.team[game.declarer];
  return game.team[p] != game.team[game.declarer];
}

void trivial_bid(Game *game) {
  game->trump = suits[0];
  game->leader = 0;
  for (int i = 0; i < N_PLAYERS; i++) {
    game->team[i] = i % 2;
  }
  game->contract = N_ROUNDS / 2;
  game->declarer = 0;
  game->turn = 0;
}

#endif