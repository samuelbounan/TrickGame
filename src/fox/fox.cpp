// 4-player card playing program using expert knowledge, expert games, belief
// states, and-or search (c) 2022, Stefan Edelkamp

#include "fox.h"

#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <string>
#include <utility>

#include "aos.h"
#include "cards.h"
#include "suitplay.h"
#include "util.h"

#ifndef FROMFILE
#include "../rules/contree.h"
#else
#include "base.h"
#endif

using namespace std;

void CardAI::StartGame(int pos, card hand) {
  logging("(fox %d) my position: %d", id, pos);
  logging("(fox %d) my handcards %ud ", id, hand);

  /*
  list<card> to_print = set_cards(hand);
  cout << "HERE" << endl;
  for (card j : to_print) {
    cout << cardname[__builtin_ctzll(j)] << ";";
  }
  cout << endl;
    for (card j : to_print) {
    cout << card_name[__builtin_ctzll(j)] << ";";
  }
      cout << endl;
  */
  //  g.print_vector(hand,0); cout << endl;

  myhand = hand;  // reversed;
  mypos = pos;    // VH=0,MH=1,HH=2
  ALLBITS = 0;
  for (int j = 0; j < DECKSIZE; j++) ALLBITS |= (ONE << j);
  g.ALLBITS = ALLBITS;
  trickround = tricknumber = 0;
  myskat = 0;
  for (int i = 0; i < PLAYER; i++) trickcard[i] = -1;
  g.posdecl = mypos + 1;
  for (int i = 0; i < PLAYER; i++) g.bv[i] = 0;
  g.trumpmask = 0;
  logging("(fox %d) my bidding strength %d, pos %d", id, mystrength, mypos);
}

card CardAI::PlayCard() {
  cout << "[ Stefan ] ";
  if (printing >= 4)
    cout << myplay->kb.decl_score << " vs " << myplay->kb.opp_score;
  long long countrec = 0;
  logging("(fox %d) play card player %d trickround %d, trick %d", id, position,
          trickround, tricknumber);
  myplay->killermove = 0;
  int c = -1, index = -1;
  if (trickcard[1] == -1) {
#ifdef KILLER
    index = -1;
    myplay->killermove = 1;
    index = myplay->choosePlayerCard();
    myplay->killermove = 0;
    logging("(fox %d) killer pos %d card %d trickround %d, trick %d", id,
            position, index, trickround, tricknumber);
    if (index != -1) {
      c = index;
      logging("(fox %d) killer pos %d card %s trickround %d, trick %d", id,
              position, card_name[index], trickround, tricknumber);
    }
#endif
#ifdef SUGGEST
    index = -1;
    if (trickround * 3 + tricknumber > BSDEPTH) {
      countrec = myplay->countBelief();
      if (countrec > 0) {
        if (c == -1) index = myplay->suggestCard();
        logging(
            "(fox %d) suggest player card player %d index %d trickround %d, "
            "trick %d, worlds %lld",
            id, position, index, trickround, tricknumber, countrec);
      }
    }
    if (index != -1) {
      if (c == -1) c = index;
    } else
#endif
        if (c == -1)
      c = myplay->choosePlayerCard();
  } else if (trickcard[1] == -1) {
    logging("(fox %d) play card, trick card on table %s", id,
            card_name[trickcard[0]]);
#ifdef KILLER
    index = -1;
    myplay->killermove = 1;
    index = myplay->choosePlayerCard(trickcard[0]);
    myplay->killermove = 0;
    if (index != -1) {
      c = index;
      logging("(fox %d) killer card %d card %s trickround %d, trick %d", id,
              position, card_name[index], trickround, tricknumber);
    }
#endif
#ifdef SUGGEST
    index = -1;
    if (trickround * 3 + tricknumber > BSDEPTH) {
      countrec = myplay->countBelief();
      if (countrec > 0) {
        if (c == -1) index = myplay->suggestCard();
        logging(
            "(fox %d) suggest player card player %d index %d trickround %d, "
            "trick %d, worlds %lld",
            id, position, index, trickround, tricknumber, countrec);
      }
    }
    if (index != -1) {
      if (c == -1) c = index;
    } else
#endif
        if (c == -1)
      c = myplay->choosePlayerCard(trickcard[0]);
  } else if (trickcard[2] == -1) {
    logging("(fox %d) play card trick card on table %s,%s", id,
            card_name[trickcard[0]], card_name[trickcard[1]]);
#ifdef KILLER
    index = -1;
    myplay->killermove = 1;
    index = myplay->choosePlayerCard(trickcard[0], trickcard[1]);
    myplay->killermove = 0;
    logging("(fox %d) killer pos %d card %d trickround %d, trick %d", id,
            position, index, trickround, tricknumber);
    if (index != -1) {
      c = index;
      logging("(fox %d) killer card %d card %s trickround %d, trick %d", id,
              position, card_name[index], trickround, tricknumber);
    }
#endif
#ifdef SUGGEST
    index = -1;
    if (trickround * 3 + tricknumber > BSDEPTH) {
      countrec = myplay->countBelief();
      if (countrec > 0) {
        if (c == -1) index = myplay->suggestCard();
        logging(
            "(fox %d) suggest player card player %d card %d trickround %d, "
            "trick %d, worlds %lld",
            id, position, index, trickround, tricknumber, countrec);
      }
    }
    if (index != -1) {
      if (c == -1) c = index;
    } else
#endif
        if (c == -1)
      c = myplay->choosePlayerCard(trickcard[0], trickcard[1]);
  } else {
#ifdef KILLER
    index = -1;
    myplay->killermove = 1;
    index = myplay->choosePlayerCard(trickcard[0], trickcard[1], trickcard[2]);
    logging("(fox %d) killer pos %d card %d trickround %d, trick %d", id,
            position, index, trickround, tricknumber);
    myplay->killermove = 0;
    if (index != -1) {
      c = index;
      logging("(fox %d) killer card %d card %s trickround %d, trick %d", id,
              position, card_name[index], trickround, tricknumber);
    }
#endif
#ifdef SUGGEST
    index = -1;
    if (trickround * 3 + tricknumber > BSDEPTH) {
      countrec = myplay->countBelief();
      if (countrec > 0) {
        if (c == -1) index = myplay->suggestCard();
        logging(
            "(fox %d) suggest player card player %d index %d trickround %d, "
            "trick %d, worlds %lld",
            id, position, index, trickround, tricknumber, countrec);
      }
    }
    if (index != -1) {
      if (c == -1) c = index;
    } else
#endif
        if (c == -1)
      c = myplay->choosePlayerCard(trickcard[0], trickcard[1]);
  }
  logging(
      "(fox %d) trickround %d, trick %d: card %s chosen by player %d decl %d "
      "opp %d",
      id, trickround, tricknumber, card_name[c], (position), myplay->decl_score,
      myplay->opp_score);
  return (ONE << c);
}

int CardAI::PlayBid(int bidval) { return true; }

void CardAI::SetTeams(int* teams, int* bids) {
  cout << "TEAMS ";
  for (int i = 0; i < PLAYER; i++) g.team[i] = 1 - teams[i];
  for (int i = 0; i < PLAYER; i++) cout << g.team[i] << ";";
  cout << endl;
  for (int i = 0; i < PLAYER; i++) g.bv[i] = bids[i];
}

void CardAI::DeclareGame(card skat, GameDeclaration* gameDeclaration) {
  myskat = skat;
  // no feedback as game is determined by server before skat taking
}

void CardAI::SetGame(GameDeclaration* gameDeclaration) {
  std::mt19937 gen32(0);
  for (int i = 0; i < SUITCOUNT; i++) {
    g.Mask[i] = 0;
    for (int j = 0; j < DECKSIZE; j++) {
      if (Suit[j] == i) g.Mask[i] |= ONE << j;
    }
  }
  g.game = gameDeclaration->game;
  g.game = 12;  // hard coded  // SE

  g.trumpmask = gameDeclaration->trumpmask;
  g.limit = gameDeclaration->limit;
  g.sumvalues = gameDeclaration->total;

  if (g.game == 0) {
    trumpmask = 0;
  }
  if (g.game == 1) {
    trumpmask = ~0;
  }

  if (g.game == 2) {
    trumpmask = JACKS;
  }
  if (g.game == 3) {
    trumpmask = QUEENS;
  }
  if (g.game == 4) {
    trumpmask = HT | JACKS | QUEENS | DIAMONDS;
  }
#ifdef TAROT
  if (g.game == 5) {
    trumpmask = TRUMPS;
  }
  if (g.game == 6) {
    trumpmask = TRUMPS;
  }
  if (g.game == 7) {
    trumpmask = TRUMPS;
  }
  if (g.game == 8) {
    trumpmask = TRUMPS;
  }
#endif
#ifndef DOPPEL
  if (g.game == 9) {
    trumpmask = DIAMONDS;
  }
  if (g.game == 10) {
    trumpmask = HEARTS;
  }
  if (g.game == 11) {
    trumpmask = SPADES;
  }
  if (g.game == 12) {
    trumpmask = CLUBS;
  }
#else
  if (g.game == 9) {
    trumpmask = HT | JACKS | QUEENS | DIAMONDS;
  }
  if (g.game == 10) {
    trumpmask = HT | JACKS | QUEENS | HEARTS;
  }
  if (g.game == 11) {
    trumpmask = HT | JACKS | QUEENS | SPADES;
  }
  if (g.game == 12) {
    trumpmask = HT | JACKS | QUEENS | CLUBS;
  }
#endif
  g.trumpmask = trumpmask;
  cout << "trumpmask ";
  g.print_vector(g.trumpmask, g.trumpmask);
  cout << endl;

#ifdef TAROT
  trumpmask = TRUMPS;
  for (int i = 0; i < DECKSIZE; i++) g.total[i] = i;
#endif

#ifdef SPADE
  g.limit = 60 + 10 * (gen32() % 5);
  g.game = 11;
#endif
#ifdef BRIDGE
  g.sumvalues = 13;
  g.limit = 6 + gen32() % 4;
  g.posdecl = 1 + gen32() % 4;
#endif
#ifdef TAROT
  g.sumvalues = 18;
  g.limit = 10;
  g.posdecl = 1 + gen32() % 4;
#endif
#ifdef BELOTE
  g.sumvalues = 162;
  g.limit = 80 + 10 * (gen32() % 4);
  g.posdecl = 1 + gen32() % 4;
#endif
  for (int i = 0; i < DECKSIZE; i++) g.Suit[i] = Suit[i];
    //    for (int i=0;i<SUITCOUNT;i++)
    //      g.Mask[i] = mask[i];
#ifdef DOPPEL
  g.sumvalues = 240;
  g.limit = 120;  // +30*(gen32()%3);
  g.game = 4;
  /*
  if (g.game == 4) {
    for (int i=0;i<PLAYER;i++)
      if (CQ & hands[i]) g.posdecl = i+1;
  }
  int maxj = 0, maxq = 0;
  for(int i=0;i<PLAYER;i++) {
    if (maxj < count(hands[i] & JACKS)) {
      maxj = count(hands[i] & JACKS);
      if (maxj >=4 && count(hands[i] & ACES) >= 3 && count(hands[i] & CJ) >= 1
  || maxj >=3 && count(hands[i] & ACES) >= 3 && count(hands[i] & CJ) == 2) {
        g.game = 2;
        g.posdecl = i+1;
      }
    }
    if (maxq < count(hands[i] & QUEENS)) {
      maxq = count(hands[i] & QUEENS);
      if (maxq >=4 && count(hands[i] & ACES) >= 3 && count(hands[i] & CQ) >= 1
  || maxq >=3 && count(hands[i] & ACES) >= 3 && count(hands[i] & CQ) == 2) {
        g.game = 3;
        g.posdecl = i+1;
      }
    }
  }
  */
  if (g.game == 9) g.game = 4;
  if (g.game == 2) {
    for (int j = 0; j < DECKSIZE; j++) g.Suit[j] = SuitJacks[j];
  }
  if (g.game == 3) {
    for (int j = 0; j < DECKSIZE; j++) g.Suit[j] = SuitQueens[j];
  }
  if (g.game == 10) {
    for (int j = 0; j < DECKSIZE; j++) g.Suit[j] = SuitHearts[j];
  }
  if (g.game == 11) {
    for (int j = 0; j < DECKSIZE; j++) g.Suit[j] = SuitSpades[j];
  }
  if (g.game == 12) {
    for (int j = 0; j < DECKSIZE; j++) g.Suit[j] = SuitClubs[j];
  }

  for (int i = 0; i < SUITCOUNT; i++) {
    g.Mask[i] = 0;
    for (int j = 0; j < DECKSIZE; j++) {
      if (g.Suit[j] == i) {
        g.Mask[i] |= (ONE << j);
      }
    }
  }
#endif

#ifdef DOPPEL
  for (int i = 0; i < DECKSIZE; i++) {
    if (g.game == 2)
      g.total[i] = jacksorder[i];
    else if (g.game == 3)
      g.total[i] = queensorder[i];
    else
      g.total[i] = i;
  }
#else
#ifdef TAROT
  //    g.game = 5+gen32()%4;
  for (int i = 0; i < DECKSIZE; i++) {
    g.total[i] = i;
  }
#else
  for (int i = 0; i < DECKSIZE; i++) {
    if ((ONE << i) & trumpmask) {
      int j = trumporder[i];
      g.total[i] = j;
    } else {
      int j = suitorder[i];
      g.total[i] = j;
    }
  }
  int offset = 0;
  for (int i = 0; i < SUITSIZE; i++) {
    if (g.game == 12) {
    }
    if (g.game == 11) {
      std::swap(g.total[i + SUITSIZE], g.total[i]);
    }
    if (g.game == 10) {
      std::swap(g.total[i + 2 * SUITSIZE], g.total[i + 1 * SUITSIZE]);
      std::swap(g.total[i + 1 * SUITSIZE], g.total[i]);
    }
    if (g.game == 9) {
      std::swap(g.total[i + 3 * SUITSIZE], g.total[i + 2 * SUITSIZE]);
      std::swap(g.total[i + 2 * SUITSIZE], g.total[i + 1 * SUITSIZE]);
      std::swap(g.total[i + 1 * SUITSIZE], g.total[i]);
    }
  }
#endif
#endif
  //    cout << "trumpmask"; g.print_vector(trumpmask,trumpmask); cout << " game
  //    " << g.game << endl;
  // for(int i=0;i<4;i++) { cout << "suitmask ";
  // g.print_vector(g.Mask[i],trumpmask); } cout << " game " << g.game << endl;

  //    cout << "ORDERING" << endl;   for(int i=0;i<DECKSIZE;i++)  cout <<
  //    card_name[g.total[i]]; cout << endl;

  for (int i = 0; i < DECKSIZE; i++) {
    g.lower[i] = g.higher[i] = 0;
  }
  for (int i = 0; i < DECKSIZE; i++) {
    g.inverse[g.total[i]] = i;
  }
  for (int i = 0; i < DECKSIZE; i++) {
    for (int j = 0; j < DECKSIZE; j++) {
      if (((ONE << i) | (ONE << j)) && (g.Suit[i] == g.Suit[j]) &&
          Card[i] != Card[j]) {
        if (g.inverse[i] < g.inverse[j] && Card[i] != Card[j]) {
          g.lower[i] |= (ONE << j);
          g.higher[j] |= (ONE << i);
        }
      }
    }
  }
  for (int i = 0; i < DECKSIZE; i++) {
    for (int j = 0; j < DECKSIZE; j++) {
      if (((ONE << i) & ~trumpmask) && ((ONE << j) & trumpmask)) {
        g.higher[i] |= (ONE << j);
        g.lower[j] |= (ONE << i);
      }
    }
  }
  /*
    for(int i=0;i<DECKSIZE;i++) {
    cout << card_name[i] << " lower "; g.print_vector(g.lower[i],trumpmask);
    cout << " higher "; g.print_vector(g.higher[i],trumpmask);
    cout << endl;
    }
  */
  for (int i = 0; i < DECKSIZE; i++) {
    for (int j = 0; j < DECKSIZE; j++) {
      g.equiv[i][j] = 0;
    }
  }
  for (int j = 0; j < DECKSIZE; j++) {
    for (int i = 0; i < DECKSIZE; i++) {
      if (g.Suit[g.total[i]] == g.Suit[g.total[j]] &&
          VALUE(g.total[i]) == VALUE(g.total[j]) &&
          (g.total[i] == g.total[j] + 1)) {
        g.equiv[g.total[i]][g.total[j]] = 1;
      }
    }
  }
  for (int k = 0; k < DECKSIZE; k++) {
    for (int i = 0; i < DECKSIZE; i++) {
      for (int j = 0; j < DECKSIZE; j++) {
        if (i - 1 >= 0 && g.Suit[g.total[i]] == g.Suit[g.total[j]] &&
            VALUE(g.total[i]) == VALUE(g.total[j]) &&
            (g.equiv[g.total[i - 1]][g.total[j]]))
          g.equiv[g.total[i]][g.total[j]] = 1;
      }
    }
  }
  /*
    cout << "   ";
    for(int i=0;i<DECKSIZE;i++)
    cout << card_name[i];
    cout << endl;
    for(int i=0;i<DECKSIZE;i++) {
    cout << card_name[i] << " ";
    for(int j=0;j<DECKSIZE;j++) {
    if (g.equiv[i][j] == true) cout << " 1"; else cout << " 0";
    }
    cout << endl;
    }
    */
  /*
  for(int i=0;i<DECKSIZE;i++) cout << card_name[g.total[i]] <<"(" << g.total[i]
  << ";" << i << ";" <<g.inverse[i] <<")"<< ";"; cout << endl; for(int
  i=0;i<DECKSIZE;i++) cout << card_name[i] <<"(" << i << ";" << g.inverse[i]
  <<")"<< ";"; cout << endl;
  */

  // int start = g.posdecl-1; // position of issuer, given P[0] is NS

  // tsize = 0; // globally storing tricks, resetting to empty
  //  rotate_pos(hands,start,g.posdecl);

  logging("(fox %d) game declared: %d contract %d total %d", id,
          gameDeclaration->game, gameDeclaration->limit,
          gameDeclaration->total);
  if (myplay) delete myplay;
  myplay = new Suitplay(g, myhand, position);

  logging("(fox %d) trick-based player initialized", id);
}

void CardAI::CardPlayed(int pos_player, card c) {
  int pcard = g.smallest(c);
  logging("(fox %d) card played trickround %d, trick %d: card %s played by %d",
          id, trickround, tricknumber, card_name[pcard], pos_player);
  trickcard[tricknumber] = pcard;
  int dist = ((pos_player + PLAYER) - mypos) % PLAYER;
  int pos = (position + dist) % PLAYER;
  if (dist != 0) {
    logging("(fox %d) inform player %d card %s played by player %d", id,
            position, card_name[pcard], pos);
    myplay->informPlayerCard(pos, pcard);
  } else {
  }
  if (tricknumber == 0) {
    int issuer = pos;
    // update called for all player tricky if card played is called first.
    logging(
        "(fox %d) update belief/1 of player %d based on card %s trick issued "
        "by player %d",
        id, position, card_name[pcard], issuer);
    myplay->updateBelief(issuer, pcard);
  } else if (tricknumber == 1) {
    int issuer = (pos + 3) % PLAYER;
    logging(
        "(fox %d) update belief/2 of player %d based on card %s trick issued "
        "by player %d",
        id, position, card_name[pcard], issuer);
    myplay->updateBelief(issuer, trickcard[0], pcard);
  } else if (tricknumber == 2) {
    int issuer = (pos + 2) % PLAYER;
    logging(
        "(fox %d) update belief/3 of player %d based on card %s issued by "
        "player %d",
        id, position, card_name[pcard], issuer);
    myplay->updateBelief(issuer, trickcard[0], trickcard[1], pcard);
  } else if (tricknumber == 3) {
    int issuer = (pos + 1) % PLAYER;
    logging(
        "(fox %d) update belief/4 of player %d based on card %s issued by "
        "player %d",
        id, position, card_name[pcard], issuer);
    myplay->updateBelief(issuer, trickcard[0], trickcard[1], trickcard[2],
                         pcard);
    trickcard[0] = trickcard[1] = trickcard[2] = -1;
  }
  tricknumber = (tricknumber + 1) % PLAYER;
  if (tricknumber == 0) trickround++;
  logging("(fox %d) trick card on table %s:%s:%s", id,
          trickcard[0] == -1 ? "X" : card_name[trickcard[0]],
          trickcard[1] == -1 ? "X" : card_name[trickcard[1]],
          trickcard[2] == -1 ? "X" : card_name[trickcard[2]]);
}

void CardAI::rotate_pos(card hand[PLAYER], int& start, int pos) {
  //  cout << "rotate pos" << endl;
  start = 0;  // VH fixed in data to be mentioned first, no with pos 1
  for (int i = 1; i < PLAYER; i++) {
    if (pos == i + 1) {
      for (int j = 0; j < i; j++) {
        card temp = hand[0];
        for (int k = 1; k < PLAYER; k++) hand[k - 1] = hand[k];
        hand[PLAYER - 1] = temp;
      }
      start = (start + (PLAYER + 1 - (pos))) % PLAYER;
    }
  }
  //  cout << "end of rotate pos" << endl;
}

int CardAI::choose(int cards[PLAYER], int start) {
  //  cout << "choose " << start;
  for (int i = 0; i < PLAYER; i++) cout << card_name[cards[i]];
  int wincard = -1;
  for (int i = 0; i < PLAYER; i++) {
    if (start == i)
      wincard =
          g.win(cards[(0 + i) % PLAYER], cards[(1 + i) % PLAYER],
                cards[(2 + i) % PLAYER], cards[(3 + i) % PLAYER], trumpmask);
  }
  //  cout << "wincard " << card_name[wincard] << endl;
  for (int i = 0; i < PLAYER; i++)
    if (wincard == cards[i]) return i;
  return -1;
}

void CardAI::inittrickgame(card hands[PLAYER], int start) {
  card hand[PLAYER];
  for (int i = 0; i < PLAYER; i++) hand[i] = hands[i];
  cout << "AI player... " << endl;  // game being played is set globally!
  if (g.game == 0) {
    g.trumpmask = 0;
  } else if (g.game == 1) {
    g.trumpmask = ~0;
  } else if (g.game == 2) {
    g.trumpmask = JACKS;
  } else if (g.game == 3) {
    g.trumpmask = QUEENS;
  } else if (g.game == 4) {
    g.trumpmask = HT | JACKS | QUEENS | DIAMONDS;
  }
#ifdef TAROT
  else if (g.game == 5) {
    g.trumpmask = TRUMPS;
  } else if (g.game == 6) {
    g.trumpmask = TRUMPS;
  } else if (g.game == 7) {
    g.trumpmask = TRUMPS;
  } else if (g.game == 8) {
    g.trumpmask = TRUMPS;
  }
#endif
  else if (g.game == 9) {
    g.trumpmask = DIAMONDS;
  } else if (g.game == 10) {
    g.trumpmask = HEARTS;
  } else if (g.game == 11) {
    g.trumpmask = SPADES;
  } else if (g.game == 12) {
    g.trumpmask = CLUBS;
  }
  for (int i = 0; i < PLAYER; i++) player[i] = new Suitplay(g, hand[i], i);

  //  for(int i=0;i<PLAYER;i++)
  //    if (INTERACTPLAYER == i)
}

int CardAI::playtrickgame(card hands[PLAYER], int start) {
  card hand[PLAYER];
  int P[PLAYER];
  for (int i = 0; i < PLAYER; i++) hand[i] = hands[i];
  for (int i = 0; i < PLAYER; i++) P[i] = -1;
  int round = 0;
  int decl_score = 0, opp_score = 0;
  int s = sum(g.skat, g.trumpmask);
  if (g.game == 8)
    opp_score += s;
  else
    decl_score += s;

  for (int i = 0; i < PLAYER; i++) player[i]->interactive = false;

  /*
#ifdef BELOTE
  if ((trumpmask & QUEENS & hand0) && (trumpmask & KINGS & hand0)) { decl_score
+= 20; } if ((trumpmask & QUEENS & hand[1]) && (trumpmask & KINGS & hand[1])) {
decl_score += 20; } if ((trumpmask & QUEENS & hand[2]) && (trumpmask & KINGS &
hand[2])) { decl_score += 20; } if ((trumpmask & QUEENS & hand[3]) && (trumpmask
& KINGS & hand[3])) { decl_score += 20; } #endif
  */
  int table[PLAYER][MAXTRICKS];
  card played = 0;
  tsize = 0;
  int index = -1;
  countrec = 0;

  card allhands = 0;
  for (int i = 0; i < PLAYER; i++) allhands = hand[i] | allhands;

  while (allhands) {  // game still being played
    cout << "Truth;" << endl;
    g.print_hands(hand, start, trumpmask);
    cout << "Declarer team score " << decl_score << "  Opponent team score "
         << opp_score;
    if (decl_score > g.limit) cout << " [ declarer won ]";
    if (opp_score > g.sumvalues - g.limit) cout << " [ opponents won ]";
    cout << endl;
    index = -1;
    if (start == 0) {
      int c = -1;  // cout << "worlds " << countrec << endl;
#ifdef KILLER
      index = -1;
      player[0]->killermove = 1;
      index = player[0]->choosePlayerCard();
      logging("(fox %d) killer pos %d card %d trickround %d, trick %d", id,
              start, index, round, tsize % PLAYER);
      player[0]->killermove = 0;
      if (index != -1) {
        c = index;
        logging("(fox %d) killer pos %d card %s trickround %d, trick %d", id,
                start, card_name[index], round, tsize % PLAYER);
      }
#endif
#ifdef SUGGEST
      index = -1;
      if (tsize > BSDEPTH) {
        countrec = player[0]->countBelief();
        // cout << "try " << countrec << endl;
        if (countrec > 0) {
          // cout << "suggesting " << countrec << endl;
          if (c == -1) index = player[0]->suggestCard();
        }
      }
      if (index != -1) {
        if (c == -1) c = index;
      } else
#endif
          if (c == -1) {
        c = player[0]->choosePlayerCard();
      }
      P[0] = c;
      player[1]->informPlayerCard(0, P[0]);
      player[2]->informPlayerCard(0, P[0]);
      player[3]->informPlayerCard(0, P[0]);
      tricks[tsize++] = P[0];
      player[0]->updateBelief(start, tricks[tsize - 1]);
      player[1]->updateBelief(start, tricks[tsize - 1]);
      player[2]->updateBelief(start, tricks[tsize - 1]);
      player[3]->updateBelief(start, tricks[tsize - 1]);

      c = -1;  // cout << "worlds " << countrec << endl;
#ifdef KILLER
      index = -1;
      player[1]->killermove = 1;
      index = player[1]->choosePlayerCard(P[0]);
      logging("(fox %d) killer pos %d card %d trickround %d, trick %d", id,
              start + 1, index, round, tsize % PLAYER);
      player[1]->killermove = 0;
      if (index != -1) {
        c = index;
        logging("(fox %d) killer pos %d card %s trickround %d, trick %d", id,
                start + 1, card_name[index], round, tsize % PLAYER);
      }
#endif
#ifdef SUGGEST
      index = -1;
      if (tsize > BSDEPTH) {
        countrec = player[1]->countBelief();
        // cout << "try " << countrec << endl;
        if (countrec > 0) {
          // cout << "suggesting " << countrec << endl;
          if (c == -1) index = player[1]->suggestCard();
        }
      }
      if (index != -1) {
        if (c == -1) c = index;
      } else
#endif
          if (c == -1) {
        c = player[1]->choosePlayerCard(P[0]);
      }
      P[1] = c;
      player[0]->informPlayerCard(1, P[1]);
      player[2]->informPlayerCard(1, P[1]);
      player[3]->informPlayerCard(1, P[1]);
      tricks[tsize++] = P[1];
      player[0]->updateBelief(start, tricks[tsize - 2], tricks[tsize - 1]);
      player[1]->updateBelief(start, tricks[tsize - 2], tricks[tsize - 1]);
      player[2]->updateBelief(start, tricks[tsize - 2], tricks[tsize - 1]);
      player[3]->updateBelief(start, tricks[tsize - 2], tricks[tsize - 1]);

      c = -1;  // cout << "worlds " << countrec << endl;
#ifdef KILLER
      index = -1;
      player[2]->killermove = 1;
      index = player[2]->choosePlayerCard(P[0], P[1]);
      logging("(fox %d) killer pos %d card %d trickround %d, trick %d", id,
              start + 2, index, round, tsize % PLAYER);
      player[2]->killermove = 0;
      if (index != -1) {
        c = index;
        logging("(fox %d) killer pos %d card %s trickround %d, trick %d", id,
                start + 2, card_name[index], round, tsize % PLAYER);
      }
#endif
#ifdef SUGGEST
      index = -1;
      if (tsize > BSDEPTH) {
        countrec = player[2]->countBelief();
        // cout << "try " << countrec << endl;
        if (countrec > 0) {
          // cout << "suggesting " << countrec << endl;
          if (c == -1) index = player[2]->suggestCard();
        }
      }
      if (index != -1) {
        if (c == -1) c = index;
      } else
#endif
          if (c == -1)
        c = player[2]->choosePlayerCard(P[0], P[1]);
      P[2] = c;
      player[0]->informPlayerCard(2, P[2]);
      player[1]->informPlayerCard(2, P[2]);
      player[3]->informPlayerCard(2, P[2]);
      tricks[tsize++] = P[2];
      player[0]->updateBelief(start, tricks[tsize - 3], tricks[tsize - 2],
                              tricks[tsize - 1]);
      player[1]->updateBelief(start, tricks[tsize - 3], tricks[tsize - 2],
                              tricks[tsize - 1]);
      player[2]->updateBelief(start, tricks[tsize - 3], tricks[tsize - 2],
                              tricks[tsize - 1]);
      player[3]->updateBelief(start, tricks[tsize - 3], tricks[tsize - 2],
                              tricks[tsize - 1]);

      c = -1;  // cout << "worlds " << countrec << endl;
#ifdef KILLER
      index = -1;
      player[3]->killermove = 1;
      index = player[3]->choosePlayerCard(P[0], P[1], P[2]);
      logging("(fox %d) killer pos %d card %d trickround %d, trick %d", id,
              start + 3, index, round, tsize % PLAYER);
      player[3]->killermove = 0;
      if (index != -1) {
        c = index;
        logging("(fox %d) killer pos %d card %s trickround %d, trick %d", id,
                start + 3, card_name[index], round, tsize % PLAYER);
      }
#endif
#ifdef SUGGEST
      index = -1;
      if (tsize > BSDEPTH) {
        countrec = player[3]->countBelief();
        if (countrec > 0) {
          if (c == -1) index = player[3]->suggestCard();
        }
      }
      if (index != -1) {
        if (c == -1) c = index;
      } else
#endif
          if (c == -1)
        c = player[3]->choosePlayerCard(P[0], P[1], P[2]);
      P[3] = c;
      player[0]->informPlayerCard(3, P[3]);
      player[1]->informPlayerCard(3, P[3]);
      player[2]->informPlayerCard(3, P[3]);
      tricks[tsize++] = P[3];
      player[0]->updateBelief(start, tricks[tsize - 4], tricks[tsize - 3],
                              tricks[tsize - 2], tricks[tsize - 1]);
      player[1]->updateBelief(start, tricks[tsize - 4], tricks[tsize - 3],
                              tricks[tsize - 2], tricks[tsize - 1]);
      player[2]->updateBelief(start, tricks[tsize - 4], tricks[tsize - 3],
                              tricks[tsize - 2], tricks[tsize - 1]);
      player[3]->updateBelief(start, tricks[tsize - 4], tricks[tsize - 3],
                              tricks[tsize - 2], tricks[tsize - 1]);
    }

    if (start == 1) {
      int c = -1;  // cout << "worlds " << countrec << endl;
#ifdef KILLER
      index = -1;
      player[1]->killermove = 1;
      index = player[1]->choosePlayerCard();
      logging("(fox %d) killer pos %d card %d trickround %d, trick %d", id,
              start + 1, index, round, tsize % PLAYER);
      player[1]->killermove = 0;
      if (index != -1) {
        c = index;
        logging("(fox %d) killer pos %d card %s trickround %d, trick %d", id,
                start + 1, card_name[index], round, tsize % PLAYER);
      }
#endif
#ifdef SUGGEST
      index = -1;
      if (tsize > BSDEPTH) {
        countrec = player[1]->countBelief();
        // cout << "try xxxx" << countrec << endl;
        if (countrec > 0) {
          // cout << "suggesting xxxx " << countrec << endl;
          if (c == -1) index = player[1]->suggestCard();
        }
      }
      if (index != -1) {
        if (c == -1) c = index;
      } else
#endif
          if (c == -1)
        c = player[1]->choosePlayerCard();
      P[1] = c;
      //            cout << " after selecting card player 1 " << c<< endl;
      player[0]->informPlayerCard(1, P[1]);
      player[2]->informPlayerCard(1, P[1]);
      player[3]->informPlayerCard(1, P[1]);
      tricks[tsize++] = P[1];
      player[0]->updateBelief(start, tricks[tsize - 1]);
      player[1]->updateBelief(start, tricks[tsize - 1]);
      player[2]->updateBelief(start, tricks[tsize - 1]);
      player[3]->updateBelief(start, tricks[tsize - 1]);

      c = -1;  // cout << "worlds " << countrec << endl;
#ifdef KILLER
      index = -1;
      player[2]->killermove = 1;
      //      cout << " before killing card player 2 " << c<< endl;
      index = player[2]->choosePlayerCard(P[1]);
      logging("(fox %d) killer pos %d card %d trickround %d, trick %d", id,
              start + 1, index, round, tsize % PLAYER);
      player[2]->killermove = 0;
      if (index != -1) {
        c = index;
        logging("(fox %d) killer pos %d card %s trickround %d, trick %d", id,
                start + 1, card_name[index], round, tsize % PLAYER);
      }
#endif
#ifdef SUGGEST
      index = -1;
      if (tsize > BSDEPTH) {
        countrec = player[2]->countBelief();
        // cout << "try " << countrec << endl;
        if (countrec > 0) {
          // cout << "suggesting " << countrec << endl;
          //	  cout << " before selecting card player 2 " << c<< endl;
          if (c == -1) index = player[2]->suggestCard();
          //	  cout << " after selecting card player 2 " << index<< endl;
        }
      }
      if (index != -1) {
        if (c == -1) c = index;
      } else
#endif
          if (c == -1)
        c = player[2]->choosePlayerCard(P[1]);
      //      cout << " after selecting card player 2 " << c << " tsize " <<
      //      tsize << endl;

      P[2] = c;
      player[0]->informPlayerCard(2, P[2]);
      player[1]->informPlayerCard(2, P[2]);
      player[3]->informPlayerCard(2, P[2]);
      tricks[tsize++] = P[2];
      player[0]->updateBelief(start, tricks[tsize - 2], tricks[tsize - 1]);
      player[1]->updateBelief(start, tricks[tsize - 2], tricks[tsize - 1]);
      player[2]->updateBelief(start, tricks[tsize - 2], tricks[tsize - 1]);
      player[3]->updateBelief(start, tricks[tsize - 2], tricks[tsize - 1]);

      c = -1;  // cout << "worlds " << countrec << endl;
#ifdef KILLER
      index = -1;
      player[3]->killermove = 1;
      index = player[3]->choosePlayerCard(P[1], P[2]);
      logging("(fox %d) killer pos %d card %d trickround %d, trick %d", id,
              start - 1, index, round, tsize % PLAYER);
      player[3]->killermove = 0;
      if (index != -1) {
        c = index;
        logging("(fox %d) killer pos %d card %s trickround %d, trick %d", id,
                start - 1, card_name[index], round, tsize % PLAYER);
      }
#endif
#ifdef SUGGEST
      index = -1;
      if (tsize > BSDEPTH) {
        countrec = player[3]->countBelief();
        // cout << "try " << countrec << endl;
        if (countrec > 0) {
          // cout << "suggesting " << countrec << endl;
          if (c == -1) index = player[3]->suggestCard();
        }
      }
      if (index != -1) {
        if (c == -1) c = index;
      } else
#endif
          if (c == -1)
        c = player[3]->choosePlayerCard(P[1], P[2]);
      P[3] = c;
      player[0]->informPlayerCard(3, P[3]);
      player[1]->informPlayerCard(3, P[3]);
      player[2]->informPlayerCard(3, P[3]);
      tricks[tsize++] = P[3];
      player[0]->updateBelief(start, tricks[tsize - 3], tricks[tsize - 2],
                              tricks[tsize - 1]);
      player[1]->updateBelief(start, tricks[tsize - 3], tricks[tsize - 2],
                              tricks[tsize - 1]);
      player[2]->updateBelief(start, tricks[tsize - 3], tricks[tsize - 2],
                              tricks[tsize - 1]);
      player[3]->updateBelief(start, tricks[tsize - 3], tricks[tsize - 2],
                              tricks[tsize - 1]);

      c = -1;  // cout << "worlds " << countrec << endl;
#ifdef KILLER
      index = -1;
      player[0]->killermove = 1;
      index = player[0]->choosePlayerCard(P[1], P[2], P[3]);
      logging("(fox %d) killer pos %d card %d trickround %d, trick %d", id,
              start - 1, index, round, tsize % PLAYER);
      player[0]->killermove = 0;
      if (index != -1) {
        c = index;
        logging("(fox %d) killer pos %d card %s trickround %d, trick %d", id,
                start - 1, card_name[index], round, tsize % PLAYER);
      }
#endif
#ifdef SUGGEST
      index = -1;
      if (tsize > BSDEPTH) {
        countrec = player[0]->countBelief();
        // cout << "try " << countrec << endl;
        if (countrec > 0) {
          // cout << "suggesting " << countrec << endl;
          if (c == -1) index = player[0]->suggestCard();
        }
      }
      if (index != -1) {
        if (c == -1) c = index;
      } else
#endif
          if (c == -1)
        c = player[0]->choosePlayerCard(P[1], P[2], P[3]);
      P[0] = c;
      player[1]->informPlayerCard(0, P[0]);
      player[2]->informPlayerCard(0, P[0]);
      player[3]->informPlayerCard(0, P[0]);
      tricks[tsize++] = P[0];
      player[0]->updateBelief(start, tricks[tsize - 4], tricks[tsize - 3],
                              tricks[tsize - 2], tricks[tsize - 1]);
      player[1]->updateBelief(start, tricks[tsize - 4], tricks[tsize - 3],
                              tricks[tsize - 2], tricks[tsize - 1]);
      player[2]->updateBelief(start, tricks[tsize - 4], tricks[tsize - 3],
                              tricks[tsize - 2], tricks[tsize - 1]);
      player[3]->updateBelief(start, tricks[tsize - 4], tricks[tsize - 3],
                              tricks[tsize - 2], tricks[tsize - 1]);
    }

    if (start == 2) {
      int c = -1;  // cout << "worlds " << countrec << endl;
#ifdef KILLER
      index = -1;
      player[2]->killermove = 1;
      //      cout << " before killing card player 2 " << c<< endl;
      index = player[2]->choosePlayerCard();
      //      cout << " after killing card player 2 " << index << endl;
      logging("(fox %d) killer pos %d card %d trickround %d, trick %d", id,
              start, index, round, tsize % PLAYER);
      player[2]->killermove = 0;
      if (index != -1) {
        c = index;
        logging("(fox %d) killer pos %d card %s trickround %d, trick %d", id,
                start, card_name[index], round, tsize % PLAYER);
      }
#endif
#ifdef SUGGEST
      index = -1;
      if (tsize > BSDEPTH) {
        countrec = player[2]->countBelief();
        // cout << "try " << countrec << endl;
        if (countrec > 0) {
          // cout << "suggesting " << countrec << endl;
          //	  cout << " before suggsting card player 2 " << c<< endl;
          if (c == -1) index = player[2]->suggestCard();
          //	  cout << " after suggesting card player 2 " << index << endl;
        }
      }
      if (index != -1) {
        if (c == -1) {
          c = index;
          //	  cout << "setting c" << card_name[c] << endl;
        }
      } else
#endif
          if (c == -1)
        c = player[2]->choosePlayerCard();
      P[2] = c;
      /*
      cout << player[0] << "," << player[1] << "," << player[2] << "," <<
      player[3]
           << "," << P[0] << "," << P[1] << "," << P[2] << "," << P[3]
           << "," << tsize << endl; */
      player[0]->informPlayerCard(2, P[2]);
      player[1]->informPlayerCard(2, P[2]);
      player[3]->informPlayerCard(2, P[2]);
      tricks[tsize++] = P[2];
      player[0]->updateBelief(start, tricks[tsize - 1]);
      player[1]->updateBelief(start, tricks[tsize - 1]);
      player[2]->updateBelief(start, tricks[tsize - 1]);
      player[3]->updateBelief(start, tricks[tsize - 1]);

      c = -1;  //  cout << "next card worlds " << countrec << endl;
#ifdef KILLER
      index = -1;
      player[3]->killermove = 1;
      //      cout << "before choosing" << endl;
      index = player[3]->choosePlayerCard(P[2]);
      //      cout << "after choosing "<< endl;
      logging("(fox %d) killer pos %d card %d trickround %d, trick %d", id,
              start - 2, index, round, tsize % PLAYER);
      player[3]->killermove = 0;
      if (index != -1) {
        c = index;
        logging("(fox %d) killer pos %d card %s trickround %d, trick %d", id,
                start - 2, card_name[index], round, tsize % PLAYER);
      }
#endif
#ifdef SUGGEST
      index = -1;
      if (tsize > BSDEPTH) {
        //	cout << "before counting " << countrec << endl;
        countrec = player[3]->countBelief();
        //	 cout << "try " << countrec << endl;
        if (countrec > 0) {
          //	   cout << "suggesting " << countrec << endl;
          if (c == -1) index = player[3]->suggestCard();
        }
      }
      if (index != -1) {
        if (c == -1) c = index;
      } else
#endif
          if (c == -1)
        c = player[3]->choosePlayerCard(P[2]);
      P[3] = c;

      player[0]->informPlayerCard(3, P[3]);
      player[1]->informPlayerCard(3, P[3]);
      player[2]->informPlayerCard(3, P[3]);
      tricks[tsize++] = P[3];
      player[0]->updateBelief(start, tricks[tsize - 2], tricks[tsize - 1]);
      player[1]->updateBelief(start, tricks[tsize - 2], tricks[tsize - 1]);
      player[2]->updateBelief(start, tricks[tsize - 2], tricks[tsize - 1]);
      player[3]->updateBelief(start, tricks[tsize - 2], tricks[tsize - 1]);
      c = -1;  // cout << "next card worlds " << countrec << endl;
#ifdef KILLER
      index = -1;
      player[0]->killermove = 1;
      index = player[0]->choosePlayerCard(P[2], P[3]);
      logging("(fox %d) killer pos %d card %d trickround %d, trick %d", id,
              start - 2, index, round, tsize % PLAYER);
      player[0]->killermove = 0;
      if (index != -1) {
        c = index;
        logging("(fox %d) killer pos %d card %s trickround %d, trick %d", id,
                start - 2, card_name[index], round, tsize % PLAYER);
      }
#endif
#ifdef SUGGEST
      index = -1;
      if (tsize > BSDEPTH) {
        countrec = player[0]->countBelief();
        // cout << "try " << countrec << endl;
        if (countrec > 0) {
          // cout << "suggesting " << countrec << endl;
          if (c == -1) index = player[0]->suggestCard();
        }
      }
      if (index != -1) {
        if (c == -1) c = index;
      } else
#endif
          if (c == -1)
        c = player[0]->choosePlayerCard(P[2], P[3]);
      P[0] = c;

      player[1]->informPlayerCard(0, P[0]);
      player[2]->informPlayerCard(0, P[0]);
      player[3]->informPlayerCard(0, P[0]);
      tricks[tsize++] = P[0];
      player[0]->updateBelief(start, tricks[tsize - 3], tricks[tsize - 2],
                              tricks[tsize - 1]);
      player[1]->updateBelief(start, tricks[tsize - 3], tricks[tsize - 2],
                              tricks[tsize - 1]);
      player[2]->updateBelief(start, tricks[tsize - 3], tricks[tsize - 2],
                              tricks[tsize - 1]);
      player[3]->updateBelief(start, tricks[tsize - 3], tricks[tsize - 2],
                              tricks[tsize - 1]);

      c = -1;
      cout << "last card, worlds " << countrec << endl;
#ifdef KILLER
      index = -1;
      player[1]->killermove = 1;
      index = player[1]->choosePlayerCard(P[2], P[3], P[0]);
      logging("(fox %d) killer pos %d card %d trickround %d, trick %d", id,
              start - 1, index, round, tsize % PLAYER);
      player[1]->killermove = 0;
      if (index != -1) {
        c = index;
        logging("(fox %d) killer pos %d card %s trickround %d, trick %d", id,
                start - 1, card_name[index], round, tsize % PLAYER);
      }
#endif
#ifdef SUGGEST
      index = -1;
      if (tsize > BSDEPTH) {
        countrec = player[1]->countBelief();
        // cout << "try " << countrec << endl;
        if (countrec > 0) {
          // cout << "suggesting " << countrec << endl;
          if (c == -1) index = player[1]->suggestCard();
        }
      }
      if (index != -1) {
        if (c == -1) c = index;
      } else
#endif
          if (c == -1)
        c = player[1]->choosePlayerCard(P[2], P[3], P[0]);
      P[1] = c;
      player[0]->informPlayerCard(1, P[1]);
      player[2]->informPlayerCard(1, P[1]);
      player[3]->informPlayerCard(1, P[1]);
      tricks[tsize++] = P[1];
      player[0]->updateBelief(start, tricks[tsize - 4], tricks[tsize - 3],
                              tricks[tsize - 2], tricks[tsize - 1]);
      player[1]->updateBelief(start, tricks[tsize - 4], tricks[tsize - 3],
                              tricks[tsize - 2], tricks[tsize - 1]);
      player[2]->updateBelief(start, tricks[tsize - 4], tricks[tsize - 3],
                              tricks[tsize - 2], tricks[tsize - 1]);
      player[3]->updateBelief(start, tricks[tsize - 4], tricks[tsize - 3],
                              tricks[tsize - 2], tricks[tsize - 1]);
    }
    if (start == 3) {
      int c = -1;  // cout << "worlds " << countrec << endl;
#ifdef KILLER
      index = -1;
      player[3]->killermove = 1;
      //      cout << " before killing card player 3 " << c<< endl;
      index = player[3]->choosePlayerCard();
      //      cout << " after killing card player 3 " << index << endl;
      logging("(fox %d) killer pos %d card %d trickround %d, trick %d", id,
              start, index, round, tsize % PLAYER);
      player[3]->killermove = 0;
      if (index != -1) {
        c = index;
        logging("(fox %d) killer pos %d card %s trickround %d, trick %d", id,
                start, card_name[index], round, tsize % PLAYER);
      }
#endif
#ifdef SUGGEST
      index = -1;
      if (tsize > BSDEPTH) {
        countrec = player[3]->countBelief();
        // cout << "try " << countrec << endl;
        if (countrec > 0) {
          // cout << "suggesting " << countrec << endl;
          //	  cout << " before suggesting card player 3 " << c<< endl;
          if (c == -1) index = player[3]->suggestCard();
          //	  cout << " after suggesting card player 3 " << index << endl;
        }
      }
      if (index != -1) {
        if (c == -1) c = index;
      } else
#endif
          if (c == -1)
        c = player[3]->choosePlayerCard();
      //      cout << " after selecting card player 3 " << c << endl;
      //      cout << " after selecting card player 3 " << card_name[c] << endl;
      P[3] = c;
      player[0]->informPlayerCard(3, P[3]);
      player[1]->informPlayerCard(3, P[3]);
      player[2]->informPlayerCard(3, P[3]);
      tricks[tsize++] = P[3];
      player[0]->updateBelief(start, tricks[tsize - 1]);
      player[1]->updateBelief(start, tricks[tsize - 1]);
      player[2]->updateBelief(start, tricks[tsize - 1]);
      player[3]->updateBelief(start, tricks[tsize - 1]);

      c = -1;
#ifdef KILLER
      index = -1;  // cout << "worlds " << countrec << endl;
      player[0]->killermove = 1;
      //      cout << " before killing card player 3/1 " << c<< endl;
      index = player[0]->choosePlayerCard(P[3]);
      //      cout << " before killing card player 3/1 " << c<< endl;
      logging("(fox %d) killer pos %d card %d trickround %d, trick %d", id,
              start - 2, index, round, tsize % PLAYER);
      player[0]->killermove = 0;
      if (index != -1) {
        c = index;
        logging("(fox %d) killer pos %d card %s trickround %d, trick %d", id,
                start - 2, card_name[index], round, tsize % PLAYER);
      }
#endif
#ifdef SUGGEST
      index = -1;
      if (tsize > BSDEPTH) {
        countrec = player[0]->countBelief();
        if (countrec > 0) {
          if (c == -1) index = player[0]->suggestCard();
        }
      }
      if (index != -1) {
        if (c == -1) c = index;
      } else
#endif
          if (c == -1)
        c = player[0]->choosePlayerCard(P[3]);
      P[0] = c;

      player[1]->informPlayerCard(0, P[0]);
      player[2]->informPlayerCard(0, P[0]);
      player[3]->informPlayerCard(0, P[0]);
      tricks[tsize++] = P[0];
      player[0]->updateBelief(start, tricks[tsize - 2], tricks[tsize - 1]);
      player[1]->updateBelief(start, tricks[tsize - 2], tricks[tsize - 1]);
      player[2]->updateBelief(start, tricks[tsize - 2], tricks[tsize - 1]);
      player[3]->updateBelief(start, tricks[tsize - 2], tricks[tsize - 1]);

      c = -1;
#ifdef KILLER
      index = -1;  // cout << "worlds " << countrec << endl;
      player[1]->killermove = 1;
      index = player[1]->choosePlayerCard(P[3], P[0]);
      logging("(fox %d) killer pos %d card %d trickround %d, trick %d", id,
              start - 2, index, round, tsize % PLAYER);
      player[1]->killermove = 0;
      if (index != -1) {
        c = index;
        logging("(fox %d) killer pos %d card %s trickround %d, trick %d", id,
                start - 2, card_name[index], round, tsize % PLAYER);
      }
#endif
#ifdef SUGGEST
      index = -1;
      if (tsize > BSDEPTH) {
        countrec = player[1]->countBelief();
        if (countrec > 0) {
          if (c == -1) index = player[1]->suggestCard();
        }
      }
      if (index != -1) {
        if (c == -1) c = index;
      } else
#endif
          if (c == -1)
        c = player[1]->choosePlayerCard(P[3], P[0]);
      P[1] = c;
      player[0]->informPlayerCard(1, P[1]);
      player[2]->informPlayerCard(1, P[1]);
      player[3]->informPlayerCard(1, P[1]);
      tricks[tsize++] = P[1];
      player[0]->updateBelief(start, tricks[tsize - 3], tricks[tsize - 2],
                              tricks[tsize - 1]);
      player[1]->updateBelief(start, tricks[tsize - 3], tricks[tsize - 2],
                              tricks[tsize - 1]);
      player[2]->updateBelief(start, tricks[tsize - 3], tricks[tsize - 2],
                              tricks[tsize - 1]);
      player[3]->updateBelief(start, tricks[tsize - 3], tricks[tsize - 2],
                              tricks[tsize - 1]);

      c = -1;  // cout << "worlds " << countrec << endl;
#ifdef KILLER
      index = -1;
      player[2]->killermove = 1;
      index = player[2]->choosePlayerCard(P[3], P[0], P[1]);
      logging("(fox %d) killer pos %d card %d trickround %d, trick %d", id,
              start - 1, index, round, tsize % PLAYER);
      player[2]->killermove = 0;
      if (index != -1) {
        c = index;
        logging("(fox %d) killer pos %d card %s trickround %d, trick %d", id,
                start - 1, card_name[index], round, tsize % PLAYER);
      }
#endif
#ifdef SUGGEST
      index = -1;
      if (tsize > BSDEPTH) {
        countrec = player[2]->countBelief();
        if (countrec > 0) {
          if (c == -1) index = player[2]->suggestCard();
        }
      }
      if (index != -1) {
        if (c == -1) c = index;
      } else
#endif
          if (c == -1)
        c = player[2]->choosePlayerCard(P[3], P[0], P[1]);
      P[2] = c;
      player[0]->informPlayerCard(2, P[2]);
      player[1]->informPlayerCard(2, P[2]);
      player[3]->informPlayerCard(2, P[2]);
      tricks[tsize++] = P[2];
      player[0]->updateBelief(start, tricks[tsize - 4], tricks[tsize - 3],
                              tricks[tsize - 2], tricks[tsize - 1]);
      player[1]->updateBelief(start, tricks[tsize - 4], tricks[tsize - 3],
                              tricks[tsize - 2], tricks[tsize - 1]);
      player[2]->updateBelief(start, tricks[tsize - 4], tricks[tsize - 3],
                              tricks[tsize - 2], tricks[tsize - 1]);
      player[3]->updateBelief(start, tricks[tsize - 4], tricks[tsize - 3],
                              tricks[tsize - 2], tricks[tsize - 1]);
    }
    //    cout << "got here " << endl;
    for (int i = 0; i < PLAYER; i++) table[i][round] = P[i];
    round++;

    for (int i = 0; i < PLAYER; i++) hand[i] &= ~(ONE << P[i]);

    start = choose(P, start);  // who is next
    //    cout << "next start P" << start << endl;
    int winner = (start == 0)   ? P[0]
                 : (start == 1) ? P[1]
                 : (start == 2) ? P[2]
                                : P[3];

    decl_score += (g.team[start] == 0) ? COST(P[0], P[1], P[2], P[3]) : 0;
    opp_score += (g.team[start] == 1) ? COST(P[0], P[1], P[2], P[3]) : 0;

    if (round == MAXTRICKS) {
#ifdef BELOTE
      if (g.team[start] == 0) decl_score += 10;
      if (g.team[start] == 1) opp_score += 10;
#endif
#ifdef TAROT
        //      if (g.team[start] == 0 && winner ==) decl_score += 10;
#endif
    }
    cout << endl
         << "Trick " << round << " " << card_name[tricks[tsize - 4]] << "("
         << tricks[tsize - 4] << ")"
         << " " << card_name[tricks[tsize - 3]] << "(" << tricks[tsize - 3]
         << ")"
         << " " << card_name[tricks[tsize - 2]] << "(" << tricks[tsize - 2]
         << ")"
         << " " << card_name[tricks[tsize - 1]] << "(" << tricks[tsize - 1]
         << ")"
         << " winner " << card_name[winner] << " value "
         << COST(P[0], P[1], P[2], P[3]) << " start P" << start << " score "
         << decl_score << " vs " << opp_score << endl;

    if (round == MAXTRICKS) {
      cout << "End of Tricks " << endl;
      cout << "Declarer team scored " << decl_score << " Opponent team scored "
           << opp_score << endl;
      if (decl_score > g.limit) cout << "Declarer won" << endl;
    }

    for (int i = 0; i < PLAYER; i++) P[i] = -1;

    played = ALLBITS;
    for (int i = 0; i < PLAYER; i++) played &= ~hand[i];
    played &= ~g.skat;

    if (count(played) > 16 &&
        count(played) < MAXTRICKS) {  // takes too long if played early
      int recommend =
          (g.team[start] == 0)
              ? player[start]->propose_declarers_card(hand[start], played)
              : player[start]->propose_opponents_card(hand[start], played);
      g.preferred_card = recommend;
      cout << " recommended card expert " << card_name[g.preferred_card]
           << std::endl;
      int won = AOS->solve(g, hand[0], hand[1], hand[2], hand[3], played, -1,
                           -1, -1, -1, decl_score, opp_score, start);
      std::cout << "computer open-card solver won " << won
                << " search tree size " << AOS->generations << std::endl;
      cout << " recommended card " << card_name[AOS->proposed_card]
           << std::endl;
    }
    allhands = 0;
    for (int i = 0; i < PLAYER; i++) allhands = hand[i] | allhands;
  }
  delete player[1];
  delete player[2];
  delete player[0];
  delete player[3];
  return decl_score;
}

CardAI::CardAI() {
  //  gamenr = 0;
  //  player1id = player2id = player[3]id = player3id = 0;
  //  tsize = trickround = tricknumber = 0;
  tsize = trickround = tricknumber = 0;
  srand(time(NULL));
  logging("(fox %d) started", id);
}

/*
void CardAI::read_samples(char* filename) {
  // Paranoia* paranoia = new Paranoia();
  ifstream myfile (filename);

  if (myfile.is_open()) {
    while (!myfile.eof()) {
      card North = 0, West = 0, South = 0, East = 0;
      int playcards[DECKSIZE];
      int value;
      bool early = false;
      myfile >> g.id;
      myfile >> g.posdecl;
      for (int i=0;i<HANDSIZE;i++) { myfile >> value; North |= (ONE<<value); }
      for (int i=0;i<HANDSIZE;i++) { myfile >> value; East |= (ONE<<value); }
      for (int i=0;i<HANDSIZE;i++) { myfile >> value; South |= (ONE<<value); }
      for (int i=0;i<HANDSIZE;i++) { myfile >> value; West |= (ONE<<value); }

      myfile >> g.bv[0] >> g.bv[1] >> g.bv[2] >> g.game >> g.limit >> g.humanwon
>> g.humanscore;

      for (int i=0;i<DECKSIZE;i++) { myfile >> value; playcards[i] = value; }
#ifdef BIDDING
      cout << "start bidding -- not implemented !" << g.id << endl;
#endif
      // g.biddingvalue = compute_bidding_value(g.rwvh,g.rwmh,g.rwhh);
      if (g.game == 0) { trumpmask = 0; }
      else if (g.game == 9) { trumpmask = DIAMONDS; }
      else if (g.game == 10) { trumpmask = HEARTS; }
      else if (g.game == 11) { trumpmask = SPADES; }
      else if (g.game == 12) { trumpmask = CLUBS; }
      card hand[0] = North, hand[1] = East, hand[2] = South, hand[3] = West;
      int start = 0; // position of issuer, given P[0] is NS
      tsize = 0; // globally storing tricks, resetting to empty
      rotate_pos(hand[0],hand[1],hand[2],hand[3],start,g.posdecl); // NS in P[0]
      AOS = new OpenCard(g);
      int won =
AOS->solve(g,hand[0],hand[1],hand[2],hand[3],0,-1,-1,-1,-1,0,0,start); #ifdef
ONLYGLAS continue; #endif int w =
trickgame(hand[0],hand[1],hand[2],hand[3],start); cout << g.id << " " <<
g.posdecl << " " << g.game << " "  << g.bv[0] << " " << g.bv[1] << " " <<
g.bv[2] << " " << g.bv[3] << " "
           << " "  << g.humanwon << " "
           << won << " "; // << " " << SHOW(AOS->proposed_card)
      cout << (int) (g.limit<w)<< " " << w << " " << g.limit; // << " "; //
replay for (int i=0;i<tsize;i++) cout << " " << SHOW(tricks[i]); cout << endl;
    }
    myfile.close();
  }
  else cout << "Unable to open file" << endl;
}
*/

//#include "paranoia.h"

#include <cassert>
#include <iostream>
#include <random>

void CardAI::take_skat(card& hand, card& skat) {
#ifdef TAROT
  card newhand = hand | skat;
  card nexthand = hand | skat;
  card newskat = 0;
  int c = count(skat);
  while (count(nexthand) > count(hand)) {
    card newhand = nexthand;
    int droppinggain = -10000;
    int ind = -1;
    while (newhand) {
      int index = g.smallest(newhand);
      int gain = g.strength(nexthand & ~(ONE << index));
      //      cout << card_name[index] << " gain " << gain << endl;
      if (gain > droppinggain) {
        droppinggain = gain;
        ind = index;
      }
      newhand &= ~(ONE << index);
    }
    //    cout << "found " << card_name[ind] << endl;
    for (int i = 0; i < SUITCOUNT; i++) {
      if (count(~trumpmask & newhand & g.Mask[i] & ~KINGS) == 1) {
        ind = g.smallest(~trumpmask & newhand & g.Mask[i] & ~KINGS);
      }
    }

    nexthand &= ~(ONE << ind);
    newskat |= (ONE << ind);
  }

  hand = nexthand;
  skat = newskat;
#endif
  /*
  card newhand = hand | skat;

  card newskat = 0;
  for (int i=0;i<c;i++) {
    int s = g.smallest_value(newhand);
#ifdef TAROT
    for (int i=0;i<SUITCOUNT;i++) {
      if (count(~trumpmask & newhand & g.Mask[i] & ~KINGS) == 1) {
        s = g.smallest(~trumpmask & newhand & g.Mask[i] &~KINGS);
      }
    }
    if (s == fool) s = g.smallest(newhand & ~FOOL);
#endif
    newhand &= ~(ONE<<s);
    newskat |= (ONE<<s);
  }
  hand = newhand;
  skat = newskat;
  */
}

void CardAI::mersenne(int seed) {
  std::mt19937 gen32(seed);
  for (int k = 0; k < 1000; k++) {
    int cards[DECKSIZE];
    card c = 0, bitindex = 0, hands[PLAYER];
    for (int i = 0; i < DECKSIZE; i++) {
      do {
        cards[i] = gen32() % DECKSIZE;
        bitindex = ONE << cards[i];
      } while ((bitindex | c) == c);
      c = c | bitindex;
    }
    for (int i = 0; i < PLAYER; i++) hands[i] = 0;
    ALLBITS = 0;
    for (int j = 0; j < DECKSIZE; j++) ALLBITS |= (ONE << j);
    g.ALLBITS = ALLBITS;
#ifdef TAROT
    trumpmask = TRUMPS;
    for (int i = 0; i < DECKSIZE; i++) g.total[i] = i;
#endif
    for (int i = 0; i < SUITCOUNT; i++) {
      g.Mask[i] = 0;
      for (int j = 0; j < DECKSIZE; j++) {
        if (Suit[j] == i) g.Mask[i] |= ONE << j;
      }
    }
    for (int i = 0; i < DECKSIZE; i++) {
      card allbefore = 0;
      for (int j = 0; j < PLAYER; j++) allbefore |= hands[j];
      int offset = 0;
      for (int j = 0; j < PLAYER; j++) {
        if (i >= j * HANDSIZE && i < (j + 1) * HANDSIZE) {
          hands[j] |= ONE << cards[i];
        }
      }
      card allafter = 0;
      for (int j = 0; j < PLAYER; j++) allafter |= hands[j];
    }
    g.id = 0;
#ifdef SPADE
    g.limit = 60 + 10 * (gen32() % 5);
    g.game = 11;
#endif
#ifdef BRIDGE
    g.sumvalues = 13;
    g.limit = 6 + gen32() % 4;
    g.posdecl = 1 + gen32() % 4;
#endif
#ifdef TAROT
    g.sumvalues = 18;
    g.limit = 10;
    g.posdecl = 1 + gen32() % 4;
#endif
#ifdef BELOTE
    g.sumvalues = 162;
    g.limit = 80 + 10 * (gen32() % 4);
    g.posdecl = 1 + gen32() % 4;
#endif
    for (int i = 0; i < DECKSIZE; i++) g.Suit[i] = Suit[i];
      //    for (int i=0;i<SUITCOUNT;i++)
      //      g.Mask[i] = mask[i];
#ifdef DOPPEL
    g.sumvalues = 240;
    g.limit = 120;  // +30*(gen32()%3);
    g.game = 4;
    if (g.game == 4) {
      for (int i = 0; i < PLAYER; i++)
        if (CQ & hands[i]) g.posdecl = i + 1;
    }
    int maxj = 0, maxq = 0;
    for (int i = 0; i < PLAYER; i++) {
      if (maxj < count(hands[i] & JACKS)) {
        maxj = count(hands[i] & JACKS);
        if (maxj >= 4 && count(hands[i] & ACES) >= 3 &&
                count(hands[i] & CJ) >= 1 ||
            maxj >= 3 && count(hands[i] & ACES) >= 3 &&
                count(hands[i] & CJ) == 2) {
          g.game = 2;
          g.posdecl = i + 1;
        }
      }
      if (maxq < count(hands[i] & QUEENS)) {
        maxq = count(hands[i] & QUEENS);
        if (maxq >= 4 && count(hands[i] & ACES) >= 3 &&
                count(hands[i] & CQ) >= 1 ||
            maxq >= 3 && count(hands[i] & ACES) >= 3 &&
                count(hands[i] & CQ) == 2) {
          g.game = 3;
          g.posdecl = i + 1;
        }
      }
    }
    if (g.game == 9) g.game = 4;
    if (g.game == 2) {
      for (int j = 0; j < DECKSIZE; j++) g.Suit[j] = SuitJacks[j];
    }
    if (g.game == 3) {
      for (int j = 0; j < DECKSIZE; j++) g.Suit[j] = SuitQueens[j];
    }
    if (g.game == 10) {
      for (int j = 0; j < DECKSIZE; j++) g.Suit[j] = SuitHearts[j];
    }
    if (g.game == 11) {
      for (int j = 0; j < DECKSIZE; j++) g.Suit[j] = SuitSpades[j];
    }
    if (g.game == 12) {
      for (int j = 0; j < DECKSIZE; j++) g.Suit[j] = SuitClubs[j];
    }

    for (int i = 0; i < SUITCOUNT; i++) {
      g.Mask[i] = 0;
      for (int j = 0; j < DECKSIZE; j++) {
        if (g.Suit[j] == i) {
          g.Mask[i] |= (ONE << j);
        }
      }
    }
#else
#ifdef TAROT
    int max = 0;
    max = 0;
    for (int i = 0; i < PLAYER; i++) {
      //      cout << "bidding strength " << " player "<< i << ":" <<
      //      g.strength(hands[i]) << endl;
      if (max < g.strength(hands[i])) {
        max = g.strength(hands[i]);
        //	cout << "newmax " <<  max << " player "<< i << endl;
        g.posdecl = i + 1;
      }
      if (max >= 40) g.game = 5;
      if (max >= 56) g.game = 6;
      if (max >= 71) g.game = 7;
      if (max >= 81) g.game = 8;
    }
    if (max < 40) {
      int max = 0;
      //    int maxposn[PLAYER];    for (int i=0;i<PLAYER;i++) maxposn[i] = 0;
      for (int i = 0; i < PLAYER; i++) {
        for (int j = 0; j < SUITCOUNT; j++) {
          if (max < count(hands[i] & TRUMPS)) {
            max = count(hands[i] & TRUMPS);
            //	  cout << "newmax " <<  max << " player "<< i << endl;
            g.posdecl = i + 1;
          }
        }
      }
      g.game = 5;
    }

#else
    int max = 0;
    //    int maxposn[PLAYER];    for (int i=0;i<PLAYER;i++) maxposn[i] = 0;
    for (int i = 0; i < PLAYER; i++) {
      for (int j = 0; j < SUITCOUNT; j++) {
        if (max < count(hands[i] & g.Mask[j])) {
          max = count(hands[i] & g.Mask[j]);
          //	  cout << "newmax " <<  max << " player "<< i << endl;
          g.game = 12 - j;
          g.posdecl = i + 1;
        }
      }
    }
#endif
#endif
    g.humanwon = -1;  // no information

    if (g.game == 0) {
      trumpmask = 0;
    }
    if (g.game == 1) {
      trumpmask = ~0;
    }

    if (g.game == 2) {
      trumpmask = JACKS;
    }
    if (g.game == 3) {
      trumpmask = QUEENS;
    }
    if (g.game == 4) {
      trumpmask = HT | JACKS | QUEENS | DIAMONDS;
    }
#ifdef TAROT
    if (g.game == 5) {
      trumpmask = TRUMPS;
    }
    if (g.game == 6) {
      trumpmask = TRUMPS;
    }
    if (g.game == 7) {
      trumpmask = TRUMPS;
    }
    if (g.game == 8) {
      trumpmask = TRUMPS;
    }
#endif
#ifndef DOPPEL
    if (g.game == 9) {
      trumpmask = DIAMONDS;
    }
    if (g.game == 10) {
      trumpmask = HEARTS;
    }
    if (g.game == 11) {
      trumpmask = SPADES;
    }
    if (g.game == 12) {
      trumpmask = CLUBS;
    }
#else
    if (g.game == 9) {
      trumpmask = HT | JACKS | QUEENS | DIAMONDS;
    }
    if (g.game == 10) {
      trumpmask = HT | JACKS | QUEENS | HEARTS;
    }
    if (g.game == 11) {
      trumpmask = HT | JACKS | QUEENS | SPADES;
    }
    if (g.game == 12) {
      trumpmask = HT | JACKS | QUEENS | CLUBS;
    }
#endif
    g.trumpmask = trumpmask;

#ifdef DOPPEL
    for (int i = 0; i < DECKSIZE; i++) {
      if (g.game == 2)
        g.total[i] = jacksorder[i];
      else if (g.game == 3)
        g.total[i] = queensorder[i];
      else
        g.total[i] = i;
    }
#else
#ifdef TAROT
    //    g.game = 5+gen32()%4;
    for (int i = 0; i < DECKSIZE; i++) {
      g.total[i] = i;
    }
#else
    for (int i = 0; i < DECKSIZE; i++) {
      if ((ONE << i) & trumpmask) {
        int j = trumporder[i];
        g.total[i] = j;
      } else {
        int j = suitorder[i];
        g.total[i] = j;
      }
    }
    int offset = 0;
    for (int i = 0; i < SUITSIZE; i++) {
      if (g.game == 12) {
      }
      if (g.game == 11) {
        std::swap(g.total[i + SUITSIZE], g.total[i]);
      }
      if (g.game == 10) {
        std::swap(g.total[i + 2 * SUITSIZE], g.total[i + 1 * SUITSIZE]);
        std::swap(g.total[i + 1 * SUITSIZE], g.total[i]);
      }
      if (g.game == 9) {
        std::swap(g.total[i + 3 * SUITSIZE], g.total[i + 2 * SUITSIZE]);
        std::swap(g.total[i + 2 * SUITSIZE], g.total[i + 1 * SUITSIZE]);
        std::swap(g.total[i + 1 * SUITSIZE], g.total[i]);
      }
    }
#endif
#endif
    //    cout << "trumpmask"; g.print_vector(trumpmask,trumpmask); cout << "
    //    game " << g.game << endl;
    // for(int i=0;i<4;i++) { cout << "suitmask ";
    // g.print_vector(g.Mask[i],trumpmask); } cout << " game " << g.game <<
    // endl;

    //    cout << "ORDERING" << endl;   for(int i=0;i<DECKSIZE;i++)  cout <<
    //    card_name[g.total[i]]; cout << endl;

    for (int i = 0; i < DECKSIZE; i++) {
      g.lower[i] = g.higher[i] = 0;
    }
    for (int i = 0; i < DECKSIZE; i++) {
      g.inverse[g.total[i]] = i;
    }
    for (int i = 0; i < DECKSIZE; i++) {
      for (int j = 0; j < DECKSIZE; j++) {
        if (((ONE << i) | (ONE << j)) && (g.Suit[i] == g.Suit[j]) &&
            Card[i] != Card[j]) {
          if (g.inverse[i] < g.inverse[j] && Card[i] != Card[j]) {
            g.lower[i] |= (ONE << j);
            g.higher[j] |= (ONE << i);
          }
        }
      }
    }
    for (int i = 0; i < DECKSIZE; i++) {
      for (int j = 0; j < DECKSIZE; j++) {
        if (((ONE << i) & ~trumpmask) && ((ONE << j) & trumpmask)) {
          g.higher[i] |= (ONE << j);
          g.lower[j] |= (ONE << i);
        }
      }
    }
    /*
    for(int i=0;i<DECKSIZE;i++) {
      cout << card_name[i] << " lower "; g.print_vector(g.lower[i],trumpmask);
      cout << " higher "; g.print_vector(g.higher[i],trumpmask);
      cout << endl;
    }
    */
    for (int i = 0; i < DECKSIZE; i++) {
      for (int j = 0; j < DECKSIZE; j++) {
        g.equiv[i][j] = 0;
      }
    }
    for (int j = 0; j < DECKSIZE; j++) {
      for (int i = 0; i < DECKSIZE; i++) {
        if (g.Suit[g.total[i]] == g.Suit[g.total[j]] &&
            VALUE(g.total[i]) == VALUE(g.total[j]) &&
            (g.total[i] == g.total[j] + 1)) {
          g.equiv[g.total[i]][g.total[j]] = 1;
        }
      }
    }
    for (int k = 0; k < DECKSIZE; k++) {
      for (int i = 0; i < DECKSIZE; i++) {
        for (int j = 0; j < DECKSIZE; j++) {
          if (i - 1 >= 0 && g.Suit[g.total[i]] == g.Suit[g.total[j]] &&
              VALUE(g.total[i]) == VALUE(g.total[j]) &&
              (g.equiv[g.total[i - 1]][g.total[j]]))
            g.equiv[g.total[i]][g.total[j]] = 1;
        }
      }
    }
    /*
    cout << "   ";
    for(int i=0;i<DECKSIZE;i++)
      cout << card_name[i];
    cout << endl;
    for(int i=0;i<DECKSIZE;i++) {
      cout << card_name[i] << " ";
      for(int j=0;j<DECKSIZE;j++) {
        if (g.equiv[i][j] == true) cout << " 1"; else cout << " 0";
      }
      cout << endl;
    }
    */
    /*
    for(int i=0;i<DECKSIZE;i++) cout << card_name[g.total[i]] <<"(" <<
    g.total[i] << ";" << i << ";" <<g.inverse[i] <<")"<< ";"; cout << endl;
    for(int i=0;i<DECKSIZE;i++) cout << card_name[i] <<"(" << i << ";" <<
    g.inverse[i] <<")"<< ";"; cout << endl;
    */
    int start = g.posdecl - 1;  // position of issuer, given P[0] is NS

    tsize = 0;  // globally storing tricks, resetting to empty
    rotate_pos(hands, start, g.posdecl);

    for (int i = 0; i < PLAYER; i++) g.team[i] = i % 2;
#ifdef TAROT
    g.team[2] = 1;
#endif

#ifdef SPADE
    g.sumvalues = 130;
#endif
#ifdef HEART
    g.sumvalues = 13;
#endif

#ifdef TAROT
    if (count((T0 | TA | TU) & hands[0]) == 3) g.limit = 72;
    if (count((T0 | TA | TU) & hands[0]) == 2) g.limit = 82;
    if (count((T0 | TA | TU) & hands[0]) == 1) g.limit = 102;
    if (count((T0 | TA | TU) & hands[0]) == 0) g.limit = 112;
    g.sumvalues = 182;
    //    g.limit = 120;
    g.posdecl = 1 + gen32() % 4;
#endif

#ifdef DOPPEL
    for (int i = 0; i < PLAYER; i++) g.team[i] = 1;
    if (g.game == 4) {
      for (int i = 0; i < PLAYER; i++)
        if (hands[i] & CQ) g.team[i] = 0;
    } else {
      g.team[0] = 0;
      start = 0;  // position of issuer, given P[0] is NS
    }
#endif
#ifdef HEART
    for (int i = 0; i < PLAYER; i++)
      if (hands[i] & C2) start = i;
#endif
    g.skat = ALLBITS & ~(hands[0] | hands[1] | hands[2] | hands[3]);
    cout << " before taking skat ";
    cout << endl;
    g.print_hands(hands, start, trumpmask);
    if (g.game == 6 || g.game == 5) {
      take_skat(hands[0], g.skat);
      g.skat = ALLBITS & ~(hands[0] | hands[1] | hands[2] | hands[3]);
    }
    cout << " after taking skat ";
    cout << endl;
    g.print_hands(hands, start, trumpmask);
    int skatval = 0;
    if (g.skat) {
      skatval += sum(g.skat, trumpmask);
    }
    g.print_hands(hands, start, trumpmask);
    std::cout << "starting open-card solver issuer " << start << " limit "
              << g.limit << std::endl;
    AOS = new OpenCard(g);

    inittrickgame(hands, start);
    int recommend =
        (g.team[start] == 0)
            ? player[start]->propose_declarers_card(hands[start], 0)
            : player[start]->propose_opponents_card(hands[start], 0);
    g.preferred_card = recommend;
    cout << " recommended card expert " << card_name[g.preferred_card]
         << std::endl;
#ifdef BELOTE
    int won = AOS->solve(g, hands[0], hands[1], hands[2], hands[3], 0, -1, -1,
                         -1, -1, 0, 0, start);
#else
    int as = 0, gs = 0;
#ifdef TAROT
    if (g.game == 8)
      gs += skatval;
    else
      as += skatval;
#endif
    int won = AOS->run(g, hands[0], hands[1], hands[2], hands[3], 0, -1, -1, -1,
                       -1, as, gs, start);
#endif
    std::cout << "computer open-card solver won " << won << " search tree size "
              << AOS->generations << std::endl;
    cout << " recommended card " << card_name[AOS->proposed_card] << std::endl;
    int w = playtrickgame(hands, start);
    cout << g.id++ << " " << g.posdecl << " " << g.game
         << " "
         //	 << g.bv[0] << " " << g.bv[1] << " " << g.bv[2] << " " <<
         //g.bv[3]
         //	 << " "  << g.humanwon
         << " " << won << " ";  // << " " << SHOW(AOS->proposed_card)
    cout << (int)(g.limit < w) << " " << w << " "
         << g.limit;  // << " "; // replay
    for (int i = 0; i < tsize; i++) cout << " " << SHOW(tricks[i]);
    cout << endl;
  }
}

#ifdef FROMFILE

#define STR(x) x

int main(int argc, char** argv) {
  CardAI cardai;
  char name[1000];
  strcpy(name, STR(COPYRIGHT));
  cout << name << endl;
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " seed" << endl;
    exit(1);
  }
  cardai.mersenne(atoi(argv[1]));
  exit(1);

  //  ai.read_samples(argv[1]);
}

#endif
