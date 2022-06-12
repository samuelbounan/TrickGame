// 4 player card playing program using expert knowledge, expert games, belief
// states, and-or search (c) 2022, Stefan Edelkamp

#include "aos.h"

#include <iostream>

#include "cards.h"
#include "game.h"

using namespace std;

OpenCard::OpenCard(GamePlay game) : g(game) {  // int global variables
  proposed = 0;
  for (int i = 0; i < 4; i++) order[i] = i;
}

OpenCard::~OpenCard() {}

void OpenCard::clear_ht() { ht.clear(); }

void OpenCard::table_add(card state, const int best) {
#ifdef TAROT
  unsigned long long hi = state >> 64;
  unsigned long long lo = state;
  ht[hi | lo] = best;
#else
  ht[state] = best;
#endif
}

void OpenCard::table_add(card state) {
#ifdef TAROT
  unsigned long long hi = state >> 64;
  unsigned long long lo = state;
  ht[hi | lo] = decl_points;
#else
  ht[state] = decl_points;
#endif
}

int OpenCard::table_lookup(card state, int as) {
#ifdef TAROT
  unsigned long long hi = state >> 64;
  unsigned long long lo = state;

  auto f = ht.find(hi | lo);
  if (f == ht.end()) return -1;
  int p = ht[hi | lo];
  if (p >> 8) {                        // the 7th bit is set
    if (p % 256 <= as) return p >> 8;  // return the bit
  } else {
    if (p % 256 >= as) return p >> 8;
  }
#else
  auto f = ht.find(state);
  if (f == ht.end()) return -1;
  int p = ht[state];
  if (p >> 8) {                        // the 7th bit is set
    if (p % 256 <= as) return p >> 8;  // return the bit
  } else {
    if (p % 256 >= as) return p >> 8;
  }
#endif
  return -1;
}

int OpenCard::winner(int P0, int P1, int P2, int P3) {
  int i1 = i[P0], i2 = i[P1], i3 = i[P2], i4 = i[P3];
  int wincard = g.win(i1, i2, i3, i4, trumpmask);
  //  cout << "wincard " << endl;
  if (wincard == i[0]) return 0;
  if (wincard == i[1]) return 1;
  if (wincard == i[2]) return 2;
  if (wincard == i[3]) return 3;
  return -1;
}

int OpenCard::select(card hand, int player) {
  int turn = count(played) % PLAYER;
  if ((g.preferred_card != -1) && (hand & (ONE << g.preferred_card)))
    return g.preferred_card;
#ifdef BRIDGE
  return g.largest(hand);
#endif
#ifdef DOPPEL
  if (DECKSIZE == 48) {
    if (hand & ACES & ~trumpmask) {
      int r = g.largest(hand & ACES & ~trumpmask);
      return r;
    }
  } else {
    if ((hand & ACES & ~trumpmask) && turn == 0) {
      int r = g.largest(hand & ACES & ~trumpmask);
      return r;
    }
  }
#endif
#ifdef BELOTE
  return g.largest(hand);
#endif
  if (count(trumpmask & hand) >= 2) {
    if ((hand & trumpmask) && team[count(played) % PLAYER] == 0) {
      if (turn > 1)
        return g.largest_trump(hand & trumpmask);
      else
        return g.smallest_trump(hand & trumpmask);
    }
  }

  if ((hand & ~trumpmask) && team[count(played) % PLAYER] == 1)
    return g.largest(hand & ~trumpmask);

  return g.largest(hand);
}

int OpenCard::AND1(card playable) {  // P0 moves
  generations++;
  //    if (generations == 100) exit(1); g.print_vector(playable,trumpmask);
  //    std::cout  << "P0 (and1) to move: " << decl_points << " vs " <<
  //    opp_points << std::endl;
  //    g.print_hands(hands[0],hands[1],hands[2],hands[3],0,trumpmask);
  int decl = 1 - team[0];
  int opp = team[0];
  int best =
      table_lookup(hands[0] | hands[1] | hands[2] | hands[3], decl_points);
  if (best != -1) return best;

  if (generations > 10000000) playable = ONE << g.any(playable);
  while (playable) {
    int index = select(
        playable, 0);  // selectttt(playable); // select0, select playing card
    card bitindex = (ONE << index);
    /*
    if (!equivalent_card(hands[0],index)) {
      playable &= ~bitindex;
      continue;
    }
    */
    hands[0] &= ~bitindex;
    played |= bitindex;
    i[0] = index;
    //        cout << "selecting " << card_name[i[0]] << " cards played " <<
    //        count(played) << endl;
    int rval = -1;
    if (count(played) % PLAYER == 0) {
      int turn = winner(1, 2, 3, 0);
      int score = COST(i[0], i[1], i[2], i[3]);
#ifdef BELOTE
      if (count(played) == PLAYSIZE) {
        score += 10;
      }
#endif
      //      cout << "trick completed "<< score << " next turn " << turn <<
      //      endl;
      if (team[turn] == 1)
        opp_points += score;
      else
        decl_points += score;
      int t1 = i[1], t2 = i[2], t3 = i[3];
      i[0] = i[1] = i[2] = i[3] = -1;
      //      cout << " as " << decl_points << " > " << limit << " opp " <<
      //      opp_points << " >= " << sumvalues-limit << endl;
      rval = (opp_points >= sumvalues - limit) ? 0
             : (decl_points > limit)           ? 1
             : (turn == 0) ? AND1(g.playable_cards(hands[0], played))
             : (turn == 1) ? OR1(g.playable_cards(hands[1], played))
             : (turn == 2) ? AND2(g.playable_cards(hands[2], played))
                           : OR2(g.playable_cards(hands[3], played));
      i[1] = t1;
      i[2] = t2;
      i[3] = t3;
      if (team[turn] == 1)
        opp_points -= score;
      else
        decl_points -= score;
    } else  // you do not know suit two cases MH,HH
      rval =
          OR1(g.playable_cards(hands[1], issuer(1), played, i));  // try to obey
    i[0] = -1;
    played &= ~bitindex;
    hands[0] |= bitindex;
    playable &= ~bitindex;

    if (rval == decl) {
      if (count(played) == ao_depth) {  // root node
        proposed_set |= ONE << index;
        proposed_card = index;
        proposed = 1;
        if (full == 0 || (count(played) % PLAYER) !=
                             PLAYER - 1)  // no sets if no card on table
          return decl;
      } else {
        if ((count(played) % PLAYER) == 0)
          table_add(hands[0] | hands[1] | hands[2] | hands[3],
                    decl_points + decl * 256);
        return decl;
      }
    }
  }
  if ((count(played) % PLAYER) == 0)
    table_add(hands[0] | hands[1] | hands[2] | hands[3],
              decl_points + opp * 256);
  return opp;
}

int OpenCard::cardontable(int player) {
  player = (player + 1) % PLAYER;
  if (i[player] != -1) return i[player];
  player = (player + 1) % PLAYER;
  if (i[player] != -1) return i[player];
  player = (player + 1) % PLAYER;
  if (i[player] != -1) return i[player];
  cout << "should not happen" << endl;
  return i[(player + 1) % PLAYER];
}

int OpenCard::issuer(int player) {
  player = (player + 1) % PLAYER;
  if (i[player] != -1) return player;
  player = (player + 1) % PLAYER;
  if (i[player] != -1) return player;
  player = (player + 1) % PLAYER;
  if (i[player] != -1) return player;
  return (player + 1) % PLAYER;
}

int OpenCard::OR1(card playable) {  // P1 move
  generations++;
  //    if (generations == 100) exit(1); g.print_vector(playable,trumpmask);
  //    std::cout  << "P1 (or1) to move: " << decl_points << " vs " <<
  //    opp_points << std::endl;
  //    g.print_hands(hands[0],hands[1],hands[2],hands[3],1,trumpmask);
  int decl = 1 - team[1];
  int opp = team[1];
  int best =
      table_lookup(hands[0] | hands[1] | hands[2] | hands[3], decl_points);
  if (best != -1) return best;
  if (generations > 10000000) playable = ONE << g.any(playable);
  while (playable) {
    int index = select(
        playable, 1);  // selecttt(playable); // select1, select playing card
    card bitindex = (ONE << index);
    /*
    if (!equivalent_card(hands[1],index)) {
      playable &= ~bitindex;
      continue;
    }
    */
    hands[1] &= ~(bitindex);
    played |= bitindex;
    i[1] = index;
    int rval = -1;
    //        cout << "selecting " << card_name[i[1]] << " cards played " <<
    //        count(played) << endl;
    if ((count(played) % PLAYER) == 0) {
      int turn = winner(2, 3, 0, 1);
      int score = COST(i[0], i[1], i[2], i[3]);
#ifdef BELOTE
      if (count(played) == PLAYSIZE) {
        score += 10;
      }
#endif
      //                  cout << "trick completed "<< score << " next turn " <<
      //                  turn << endl;
      if (team[turn] == 1)
        opp_points += score;
      else
        decl_points += score;
      int t0 = i[0], t2 = i[2], t3 = i[3];
      i[0] = i[1] = i[2] = i[3] = -1;
      //      cout << " as " << decl_points << " > " << limit << " op " <<
      //      opp_points << " >= " << sumvalues-limit << endl;
      rval = (opp_points >= sumvalues - limit) ? 0
             : (decl_points > limit)           ? 1
             : (turn == 0) ? AND1(g.playable_cards(hands[0], played))
             : (turn == 1) ? OR1(g.playable_cards(hands[1], played))
             : (turn == 2) ? AND2(g.playable_cards(hands[2], played))
                           : OR2(g.playable_cards(hands[3], played));
      i[0] = t0;
      i[2] = t2;
      i[3] = t3;
      if (team[turn] == 1)
        opp_points -= score;
      else
        decl_points -= score;
    } else
      rval = AND2(g.playable_cards(hands[2], issuer(2), played, i));
    i[1] = -1;
    played &= ~bitindex;
    hands[1] |= bitindex;
    playable &= ~bitindex;
    if (rval == decl) {
      if (count(played) == ao_depth) {  // root node
        proposed_set |= ONE << index;
        proposed_card = index;
        proposed = 1;
        if (full == 0 || (count(played) % PLAYER) !=
                             PLAYER - 1)  // no sets if no card on table
          return decl;
      } else {
        if ((count(played) % PLAYER) == 0)
          table_add(hands[0] | hands[1] | hands[2] | hands[3],
                    decl_points + decl * 256);
        return decl;
      }
    }
  }
  if ((count(played) % PLAYER) == 0)
    table_add(hands[0] | hands[1] | hands[2] | hands[3],
              decl_points + opp * 256);
  return opp;
}

int OpenCard::AND2(card playable) {  // P0 moves
  generations++;
  //    if (generations == 100) exit(1); g.print_vector(playable,trumpmask);
  //    std::cout << "P2 (and2) to move: " << decl_points << " vs " <<
  //    opp_points << std::endl;
  //    g.print_hands(hands[0],hands[1],hands[2],hands[3],2,trumpmask);
  int decl = 1 - team[2];
  int opp = team[2];
  int best =
      table_lookup(hands[0] | hands[1] | hands[2] | hands[3], decl_points);
  if (best != -1) return best;
  if (generations > 10000000) playable = ONE << g.any(playable);
  while (playable) {
    int index = select(
        playable, 2);  // selectttt(playable); // select0, select playing card
    card bitindex = (ONE << index);
    /*
    if (!equivalent_card(hands[2],index)) {
      playable &= ~bitindex;
      continue;
    }
    */
    hands[2] &= ~bitindex;
    played |= bitindex;
    i[2] = index;
    //       cout << "selecting " << card_name[i[2]] << " cards played " <<
    //       count(played) << endl;
    int rval = -1;
    if ((count(played) % PLAYER) == 0) {
      int turn = winner(3, 0, 1, 2);
      int score = COST(i[0], i[1], i[2], i[3]);
#ifdef BELOTE
      if (count(played) == PLAYSIZE) {
        score += 10;
      }
#endif
      //      cout << "trick completed "<< score << " next turn " << turn <<
      //      endl;
      if (team[turn] == 1)
        opp_points += score;
      else
        decl_points += score;
      int t0 = i[0], t1 = i[1], t3 = i[3];
      i[0] = i[1] = i[2] = i[3] = -1;
      //      cout << " as " << decl_points << " > " << limit << " opp " <<
      //      opp_points << " >= " << sumvalues-limit << endl;
      rval = (opp_points >= sumvalues - limit) ? 0
             : (decl_points > limit)           ? 1
             : (turn == 0) ? AND1(g.playable_cards(hands[0], played))
             : (turn == 1) ? OR1(g.playable_cards(hands[1], played))
             : (turn == 2) ? AND2(g.playable_cards(hands[2], played))
                           : OR2(g.playable_cards(hands[3], played));
      i[0] = t0;
      i[1] = t1;
      i[3] = t3;
      if (team[turn] == 1)
        opp_points -= score;
      else
        decl_points -= score;
    } else
      rval =
          OR2(g.playable_cards(hands[3], issuer(3), played, i));  // try to obey
    i[2] = -1;
    played &= ~bitindex;
    hands[2] |= bitindex;
    playable &= ~bitindex;
    if (rval == decl) {
      if (count(played) == ao_depth) {  // root node
        proposed_set |= ONE << index;
        proposed_card = index;
        proposed = 1;
        if (full == 0 || count(played) % PLAYER !=
                             PLAYER - 1)  // no sets if no card on table
          return decl;
      } else {
        if (count(played) % PLAYER == 0)
          table_add(hands[0] | hands[1] | hands[2] | hands[3],
                    decl_points + decl * 256);
        return decl;
      }
    }
  }
  if (count(played) % PLAYER == 0)
    table_add(hands[0] | hands[1] | hands[2] | hands[3],
              decl_points + opp * 256);
  return opp;
}

int OpenCard::OR2(card playable) {  // P2 to move
  generations++;
  //    if (generations == 100) exit(1); g.print_vector(playable,trumpmask);
  //    std::cout  << "P3 (or2) to move: " << decl_points << " vs " <<
  //    opp_points << std::endl;
  //  g.print_hands(hands[0],hands[1],hands[2],hands[3],3,trumpmask);
  int decl = 1 - team[3];
  int opp = team[3];
  int best =
      table_lookup(hands[0] | hands[1] | hands[2] | hands[3], decl_points);
  if (best != -1) return best;
  if (generations > 10000000) playable = ONE << g.any(playable);
  while (playable) {
    int index = select(playable, 3);
    card bitindex = (ONE << index);
    /*
    if (!equivalent_card(hands[3],index)) {
      playable &= ~bitindex;
      continue;
    }
    */
    hands[3] &= ~bitindex;
    played |= bitindex;
    i[3] = index;
    int rval = -1;
    //        cout << "selecting " << card_name[i[3]] << " cards played " <<
    //        count(played) << endl;
    if (count(played) % PLAYER == 0) {
      int turn = winner(0, 1, 2, 3);
      int score = COST(i[0], i[1], i[2], i[3]);
#ifdef BELOTE
      if (count(played) == PLAYSIZE) {
        score += 10;
      }
#endif
      if (team[turn] == 1)
        opp_points += score;
      else
        decl_points += score;
      int t0 = i[0], t1 = i[1], t2 = i[2];
      i[0] = i[1] = i[2] = i[3] = -1;
      //       cout << " as " << decl_points << " > " << limit << " opp " <<
      //       opp_points << " >= " << sumvalues-limit << endl;
      rval = (decl_points > limit)               ? 1
             : (opp_points >= sumvalues - limit) ? 0
             : (turn == 0) ? AND1(g.playable_cards(hands[0], played))
             : (turn == 1) ? OR1(g.playable_cards(hands[1], played))
             : (turn == 2) ? AND2(g.playable_cards(hands[2], played))
                           : OR2(g.playable_cards(hands[3], played));
      i[0] = t0;
      i[1] = t1;
      i[2] = t2;
      if (team[turn] == 1)
        opp_points -= score;
      else
        decl_points -= score;
    } else
      rval = AND1(g.playable_cards(hands[0], issuer(0), played, i));
    i[3] = -1;
    played &= ~bitindex;
    hands[3] |= bitindex;
    playable &= ~bitindex;
    if (rval == decl) {
      if (count(played) == ao_depth) {  // root node
        proposed_set |= ONE << index;
        proposed_card = index;
        proposed = 1;
        if (full == 0 || count(played) % PLAYER !=
                             PLAYER - 1)  // no sets if no card on table
          return decl;
      } else {  // other nodes
        if (count(played) % PLAYER == 0)
          table_add(hands[0] | hands[1] | hands[2] | hands[3],
                    decl_points + decl * 256);
        return decl;
      }
    }
  }
  if (count(played) % PLAYER == 0)
    table_add(hands[0] | hands[1] | hands[2] | hands[3],
              decl_points + opp * 256);
  return opp;
}

int OpenCard::solve(GamePlay g, card hand0, card hand1, card hand2, card hand3,
                    card play, int P0, int P1, int P2, int P3, int score_decl,
                    int score_opp, int start) {
  int left = score_decl - 1, right = g.sumvalues;  // binsearch
  //  return 0;
  //  right = 50;

  int pc = -1;
  proposed_card = -1;
  proposed_set = 0;
  proposed = 0;

  while (true) {
    //           cout << "Search Interval [" << left << "," << right << "]" <<
    //           endl;
    if (left == right - 1) {
      proposed = 1;
      if (pc == -1) return right;
      if (g.team[start] == 0)
        g.limit = left;
      else
        g.limit = right;
      int y = fullrun(g, hand0, hand1, hand2, hand3, play, P0, P1, P2, P3,
                      score_decl, score_opp, start);
      proposed = 1;
      //      cout <<" proposing "<< card_name[proposed_card] << endl;
      return right;
    }
    g.limit = (left + right) / 2;
    int x = run(g, hand0, hand1, hand2, hand3, play, P0, P1, P2, P3, score_decl,
                score_opp, start);
    if (!x) {  // cout << "...too big" << endl;
      right = g.limit;
      // if (start > 0) pc = proposed_card;
    } else {  // cout << "...too small" << endl;
      left = g.limit;
      // if (start == 0)
      pc = proposed_card;
    }
  }
}

int OpenCard::execute(GamePlay game, card hand0, card hand1, card hand2,
                      card hand3, card play, int P0, int P1, int P2, int P3,
                      int score_decl, int score_opp, int player, int testcard) {
  // generating AND-OR tree, P0 AND-Player, P1/P2 OR-Players
  // leaf evaluates to 0 for P0 wins and to 1 for P1/P2 win
  // game won for P0 on AND node if all succs are 1
  // game won for P1/P2 on OR node if one succ is 0
  // game lost for P0 on OR node if one succ is 0

  //  if (score_decl > g.limit) return 1;
  //  if (score_opp >= g.sumvalues-g.limit) return 0;

  generations = 0;
  full = 0;
  g = game;

  if (score_decl > g.limit) {
    if (team[player] == 1)
      return 0;
    else
      return 1;
  }
  if (score_opp >= g.sumvalues - g.limit) {
    if (team[player] == 1)
      return 1;
    else
      return 0;
  }

  card hand = (player == 0) ? hand0 : (player == 1) ? hand1 : hand2;
  int len[PLAYER];
  for (int i = 0; i < PLAYER; i++) order[i] = i;
  for (int i = 0; i < PLAYER; i++)
    len[i] = count(~play & ~hand & ~JACKS & g.Mask[i]);
  for (int i = 0; i < PLAYER; i++) {
    for (int j = 0; j < i; j++) {
      if (i < j && len[j] > len[i]) {
        std::swap(order[j], order[i]);
        std::swap(len[j], len[i]);
      }
    }
  }

  trumpmask = g.trumpmask;
  sumvalues = g.sumvalues;
  ;
  limit = g.limit;
  for (int i = 0; i < PLAYER; i++) team[i] = g.team[i];

  //  std::cout << order[0] << order[1] << order[2] << order[3] << std::endl;

  ; /*
     std::cout << "aos execute @ " << g.limit << " " << " P" << player << "
     table "; if (P3 != -1) std::cout << card_name[P3]; if (P2 != -1) std::cout
     << card_name[P2]; if (P0 != -1) std::cout << card_name[P0]; if (P1 != -1)
     std::cout << card_name[P1]; std::cout << " P0 ";
     g.print_vector(hand0,trumpmask); std::cout << " P1 ";
     g.print_vector(hand1,trumpmask); std::cout << " P2 ";
     g.print_vector(hand2,trumpmask); std::cout << " P3 ";
     g.print_vector(hand3,trumpmask); std::cout << "; as :" << score_decl;
     std::cout << "; gs :" << score_opp << ";";

     std::cout << "; testcard :" << testcard << ";";
     //  g.print_vector(play,trumpmask);
     std::cout << std::endl;
     */
  if (player == 0) {
    hands[0] = hand0 & ~play;
    hands[1] = hand1 & ~play & ~hand0;
    hands[2] = hand2 & ~hand1 & ~hand0 & ~play;
    hands[3] = hand3 & ~hand2 & ~hand1 & ~hand0 & ~play;
  }
  if (player == 1) {
    hands[1] = hand1 & ~play;
    hands[2] = hand2 & ~play & ~hand1;
    hands[3] = hand3 & ~hand2 & ~hand1 & ~play;
    hands[0] = hand0 & ~hand3 & ~hand2 & ~hand1 & ~play;
  }
  if (player == 2) {
    hands[2] = hand2 & ~play;
    hands[3] = hand3 & ~play & ~hand2;
    hands[0] = hand0 & ~hand1 & ~hand2 & ~play;
    hands[1] = hand1 & ~hand2 & ~hand3 & ~hand0 & ~play;
  }
  if (player == 3) {
    hands[3] = hand3 & ~play;
    hands[2] = hand2 & ~play & ~hand3;
    hands[1] = hand1 & ~play & ~hand2 & ~hand3;
    hands[0] = hand0 & ~hand2 & ~hand1 & ~hand3 & ~play;
  }
  /*
  std::cout << " P0 "; g.print_vector(hands[0],trumpmask);
  std::cout << " P1 "; g.print_vector(hands[1],trumpmask);
  std::cout << " P2 "; g.print_vector(hands[2],trumpmask);
  std::cout << " P3 "; g.print_vector(hands[3],trumpmask);
  */
  i[0] = P0;
  i[1] = P1;
  i[2] = P2;
  i[3] = P3;
  played = play;
  decl_points = score_decl;
  opp_points = score_opp;
  proposed = 0;
  proposed_set = 0;
  ao_depth = count(played);
  clear_ht();

  int result = -1;
  card playc = 0;
  if (player == 0) {
    int start = issuer(0);
    if (player == start)
      playc = g.playable_cards(hands[0], played);
    else {
      //      int issued = cardontable(0);
      playc = g.playable_cards(hands[0], start, played, i);
    }
    if (((ONE << testcard) & playc) == 0) {
      std::cout << " testcard failed and1 " << std::endl;
      g.print_vector(hands[0], trumpmask);
      cout << " testcard " << card_name[testcard] << ":";
      g.print_vector(playc, trumpmask);
      cout << endl;
      exit(1);
    }

    playc = (ONE << testcard);
    result = AND1(playc);
  }
  if (player == 1) {
    int start = issuer(1);
    if (player == start)
      playc = g.playable_cards(hands[1], played);
    else {
      //      int issued = cardontable(1);
      playc = g.playable_cards(hands[1], start, played, i);
    }
    if (((ONE << testcard) & playc) == 0) {
      std::cout << " testcard failed or1" << std::endl;
      g.print_vector(hands[1], trumpmask);
      cout << " testcard " << card_name[testcard] << ":";
      g.print_vector(playc, trumpmask);
      cout << endl;
      exit(1);
    }
    playc = (ONE << testcard);
    result = OR1(playc);
  }
  if (player == 2) {
    int start = issuer(2);
    if (player == start)
      playc = g.playable_cards(hands[2], played);
    else {
      //      int issued = cardontable(2);
      playc = g.playable_cards(hands[2], start, played, i);
    }
    if (((ONE << testcard) & playc) == 0) {
      std::cout << " testcard failed and2" << std::endl;
      g.print_vector(hands[2], trumpmask);
      cout << " testcard " << card_name[testcard] << ":";
      g.print_vector(playc, trumpmask);
      cout << endl;
      exit(1);
    }
    playc = (ONE << testcard);
    result = AND2(playc);
  }
  if (player == 3) {
    int start = issuer(3);
    if (player == start)
      playc = g.playable_cards(hands[3], played);
    else {
      //      int issued = cardontable(3);
      playc = g.playable_cards(hands[3], start, played, i);
    }
    if (((ONE << testcard) & playc) == 0) {
      std::cout << " testcard failed or2" << std::endl;
      g.print_vector(hands[3], trumpmask);
      cout << " testcard " << card_name[testcard] << ":";
      g.print_vector(playc, trumpmask);
      cout << endl;
      exit(1);
    }
    playc = (ONE << testcard);
    result = OR2(playc);
  }
  P0 = i[0];
  P1 = i[1];
  P2 = i[2];
  P3 = i[3];
  played = play;
  if (!proposed) {
    proposed_card =
        g.smallest(playc);  // any_playable_card(hands[player],player);
    proposed = 1;
  }
  //  std::cout << "aos finished, number of generations " << generations <<
  //  std::endl;
  return result;
}

int OpenCard::fullrun(GamePlay game, card hand0, card hand1, card hand2,
                      card hand3, card play, int P0, int P1, int P2, int P3,
                      int score_decl, int score_opp, int player) {
  // generating AND-OR tree, P0 AND-Player, P1/P2 OR-Players
  // leaf evaluates to 0 for P0 wins and to 1 for P1/P2 win
  // game won for P0 on AND node if all succs are 1
  // game won for P1/P2 on OR node if one succ is 0
  // game lost for P0 on OR node if one succ is 0

  //  if (score_decl > g.limit) return 1;
  //  if (score_opp >= g.sumvalues-g.limit) return 0;

  g = game;

  if (score_decl > g.limit) {
    if (team[player] == 1)
      return 0;
    else
      return 1;
  }
  if (score_opp >= g.sumvalues - g.limit) {
    if (team[player] == 1)
      return 1;
    else
      return 0;
  }

  generations = 0;
  full = 1;
  card hand = (player == 0) ? hand0 : (player == 1) ? hand1 : hand2;
  int len[PLAYER];
  for (int i = 0; i < PLAYER; i++) order[i] = i;
  for (int i = 0; i < PLAYER; i++)
    len[i] = count(~play & ~hand & ~JACKS & g.Mask[i]);
  for (int i = 0; i < PLAYER; i++) {
    for (int j = 0; j < i; j++) {
      if (i < j && len[j] > len[i]) {
        std::swap(order[j], order[i]);
        std::swap(len[j], len[i]);
      }
    }
  }

  trumpmask = g.trumpmask;
  sumvalues = g.sumvalues;
  limit = g.limit;
  for (int i = 0; i < PLAYER; i++) team[i] = g.team[i];

  //  std::cout << order[0] << order[1] << order[2] << order[3] << std::endl;
  /*
  std::cout << "full aos @ " << g.limit << " ";
  if (P3 != -1) std::cout << card_name[P3];
  if (P2 != -1) std::cout << card_name[P2];
  if (P0 != -1) std::cout << card_name[P0];
  if (P1 != -1) std::cout << card_name[P1];
  std::cout << " P0 "; g.print_vector(hand0,trumpmask);
  std::cout << " P1 "; g.print_vector(hand1,trumpmask);
  std::cout << " P2 "; g.print_vector(hand2,trumpmask);
  std::cout << " P3 "; g.print_vector(hand3,trumpmask);
  std::cout << ";P" << player << " ";
  std::cout << "; as :" << score_decl;
  std::cout << "; gs :" << score_opp << ";";
  //  g.print_vector(play,trumpmask);
  std::cout << std::endl;
  */
  if (player == 0) {
    hands[0] = hand0 & ~play;
    hands[1] = hand1 & ~play & ~hand0;
    hands[2] = hand2 & ~hand1 & ~hand0 & ~play;
    hands[3] = hand3 & ~hand2 & ~hand1 & ~hand0 & ~play;
  }
  if (player == 1) {
    hands[1] = hand1 & ~play;
    hands[2] = hand2 & ~play & ~hand1;
    hands[3] = hand3 & ~hand2 & ~hand1 & ~play;
    hands[0] = hand0 & ~hand3 & ~hand2 & ~hand1 & ~play;
  }
  if (player == 2) {
    hands[2] = hand2 & ~play;
    hands[3] = hand3 & ~play & ~hand2;
    hands[0] = hand0 & ~hand1 & ~hand2 & ~play;
    hands[1] = hand1 & ~hand2 & ~hand3 & ~hand0 & ~play;
  }
  if (player == 3) {
    hands[3] = hand3 & ~play;
    hands[2] = hand2 & ~play & ~hand3;
    hands[1] = hand1 & ~play & ~hand2 & ~hand3;
    hands[0] = hand0 & ~hand2 & ~hand1 & ~hand3 & ~play;
  }

  i[0] = P0;
  i[1] = P1;
  i[2] = P2;
  i[3] = P3;
  played = play;
  decl_points = score_decl;
  opp_points = score_opp;
  proposed = 0;
  proposed_set = 0;
  ao_depth = count(played);

  clear_ht();

  int result = -1;
  card playc = 0;
  if (player == 0) {
    int start = issuer(0);
    if (player == start)
      playc = g.playable_cards(hands[0], played);
    else {
      //      int issued = cardontable(0);
      playc = g.playable_cards(hands[0], start, played, i);
    }
    result = AND1(playc);
    if (!proposed) result = 0;
  }
  if (player == 1) {
    int start = issuer(1);
    if (player == start)
      playc = g.playable_cards(hands[1], played);
    else {
      //      int issued = cardontable(1);
      playc = g.playable_cards(hands[1], start, played, i);
    }
    result = OR1(playc);
    if (!proposed) result = 1;
  }
  if (player == 2) {
    int start = issuer(2);
    if (player == start)
      playc = g.playable_cards(hands[2], played);
    else {
      //      int issued = cardontable(2);
      playc = g.playable_cards(hands[2], start, played, i);
    }
    result = AND2(playc);
    if (!proposed) result = 0;
  }
  if (player == 3) {
    int start = issuer(3);
    if (player == start)
      playc = g.playable_cards(hands[3], played);
    else {
      //      int issued = cardontable(3);
      playc = g.playable_cards(hands[3], start, played, i);
    }
    result = OR2(playc);
    if (!proposed) result = 1;
  }
  P0 = i[0];
  P1 = i[1];
  P2 = i[2];
  P3 = i[3];
  played = play;
  if (!proposed) {
    proposed_card =
        g.smallest(playc);         // any_playable_card(hands[player],player);
    proposed_set = proposed_card;  // any_playable_card(hands[player],player);
    proposed = 1;
  }
  if (count(played) == 0)
    std::cout << "full aos finished, number of generations " << generations
              << " result " << result << std::endl;
  return result;
}

int OpenCard::run(GamePlay game, card hand0, card hand1, card hand2, card hand3,
                  card play, int P0, int P1, int P2, int P3, int score_decl,
                  int score_opp, int player) {
  // generating AND-OR tree, P0 AND-Player, P1/P2 OR-Players
  // leaf evaluates to 0 for P0 wins and to 1 for P1/P2 win
  // game won for P0 on AND node if all succs are 1
  // game won for P1/P2 on OR node if one succ is 0
  // game lost for P0 on OR node if one succ is 0

  g = game;

  if (score_decl > g.limit) {
    if (team[player] == 1)
      return 0;
    else
      return 1;
  }
  if (score_opp >= g.sumvalues - g.limit) {
    if (team[player] == 1)
      return 1;
    else
      return 0;
  }

  generations = 0;
  full = 0;

  card hand = (player == 0) ? hand0 : (player == 1) ? hand1 : hand2;
  int len[PLAYER];
  for (int i = 0; i < PLAYER; i++) order[i] = i;
  for (int i = 0; i < PLAYER; i++)
    len[i] = count(~play & ~hand & ~JACKS & g.Mask[i]);
  for (int i = 0; i < PLAYER; i++) {
    for (int j = 0; j < i; j++) {
      if (i < j && len[j] > len[i]) {
        std::swap(order[j], order[i]);
        std::swap(len[j], len[i]);
      }
    }
  }

  trumpmask = g.trumpmask;
  //  cout << "trumpmask "; print_128(trumpmask); cout << endl;
  sumvalues = g.sumvalues;
  limit = g.limit;
  for (int i = 0; i < PLAYER; i++) team[i] = g.team[i];

  if (count(played) == -1) {
    std::cout << "aos@" << g.limit << " player " << player;
    if (g.preferred_card != -1)
      std::cout << " preferred " << card_name[g.preferred_card] << "("
                << g.preferred_card << ")"
                << " table ";
    if (P3 != -1) std::cout << card_name[P3] << " ";
    if (P2 != -1) std::cout << card_name[P2] << " ";
    if (P0 != -1) std::cout << card_name[P0] << " ";
    if (P1 != -1) std::cout << card_name[P1] << " ";
    std::cout << " P0 ";
    g.print_vector(hand0, trumpmask);
    std::cout << " P1 ";
    g.print_vector(hand1, trumpmask);
    std::cout << " P2 ";
    g.print_vector(hand2, trumpmask);
    std::cout << " P3 ";
    g.print_vector(hand3, trumpmask);
    std::cout << "; decl score :" << score_decl;
    std::cout << "; opp score :" << score_opp << ";" << std::endl;
    //  g.print_vector(play,trumpmask);
  }

  if (player == 0) {
    hands[0] = hand0 & ~play;
    hands[1] = hand1 & ~play & ~hand0;
    hands[2] = hand2 & ~hand1 & ~hand0 & ~play;
    hands[3] = hand3 & ~hand2 & ~hand1 & ~hand0 & ~play;
  }
  if (player == 1) {
    hands[1] = hand1 & ~play;
    hands[2] = hand2 & ~play & ~hand1;
    hands[3] = hand3 & ~hand2 & ~hand1 & ~play;
    hands[0] = hand0 & ~hand3 & ~hand2 & ~hand1 & ~play;
  }
  if (player == 2) {
    hands[2] = hand2 & ~play;
    hands[3] = hand3 & ~play & ~hand2;
    hands[0] = hand0 & ~hand1 & ~hand2 & ~play;
    hands[1] = hand1 & ~hand2 & ~hand3 & ~hand0 & ~play;
  }
  if (player == 3) {
    hands[3] = hand3 & ~play;
    hands[2] = hand2 & ~play & ~hand3;
    hands[1] = hand1 & ~play & ~hand2 & ~hand3;
    hands[0] = hand0 & ~hand2 & ~hand1 & ~hand3 & ~play;
  }

  i[0] = P0;
  i[1] = P1;
  i[2] = P2;
  i[3] = P3;
  played = play;
  decl_points = score_decl;
  opp_points = score_opp;
  proposed = 0;
  proposed_set = 0;
  ao_depth = count(played);
  clear_ht();

  //    std::cout << " table ";  g.print_vector(table,trumpmask);  std::cout <<
  //    std::endl;

  int result = -1;
  card playc = 0;
  if (player == 0) {
    int start = issuer(0);
    if (player == start)
      playc = g.playable_cards(hands[0], played);
    else {
      //      int issued = cardontable(0);
      playc = g.playable_cards(hands[0], start, played, i);
    }
    //    cout << "before and1 "; g.print_vector(playc,trumpmask); cout << endl;
    result = AND1(playc);
    //    cout << "after and1 " << proposed << card_name[proposed_card] << endl;
    if (!proposed) result = 0;
  }
  if (player == 1) {
    int start = issuer(1);
    if (player == start)
      playc = g.playable_cards(hands[1], played);
    else {
      //      int issued = cardontable(1);
      playc = g.playable_cards(hands[1], start, played, i);
    }
    //        cout << "before or1 "; g.print_vector(playc,trumpmask); cout <<
    //        endl;
    result = OR1(playc);
    //        cout << "after or1 " << proposed << card_name[proposed_card] <<
    //        endl;
    //    if (!proposed) result = 1;
  }
  if (player == 2) {
    int start = issuer(2);
    if (player == start)
      playc = g.playable_cards(hands[2], played);
    else {
      //      int issued = cardontable(2);
      playc = g.playable_cards(hands[2], start, played, i);
    }
    //        cout << "before and2 "; g.print_vector(playc,trumpmask); cout <<
    //        endl;
    result = AND2(playc);
    //        cout << "after and2 " << proposed << card_name[proposed_card] <<
    //        endl;
    //    if (!proposed) result = 0;
  }
  if (player == 3) {
    int start = issuer(3);
    if (player == start)
      playc = g.playable_cards(hands[3], played);
    else {
      //      int issued = cardontable(3);
      playc = g.playable_cards(hands[3], start, played, i);
    }
    //  cout << "before or2 "; g.print_vector(playc,trumpmask); cout << endl;
    result = OR2(playc);
    //  cout << "after or2 " << proposed << card_name[proposed_card] << endl;
    //    if (!proposed) result = 1;
  }

  P0 = i[0];
  P1 = i[1];
  P2 = i[2];
  P3 = i[3];
  played = play;
  if (!proposed) {
    proposed_card =
        g.smallest(playc);  // any_playable_card(hands[player],player);
    proposed = 1;
  }
  if (count(played) == 0)
    std::cout << "aos finished, number of generations " << generations
              << " result " << result << std::endl;
  return result;
}
