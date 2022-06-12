// 4-player card playing program using expert knowledge, expert games, belief states, and-or search
// (c) 2022, Stefan Edelkamp 

#include <iostream>
#include "cards.h"
#include "game.h"
#include "suitplay.h"

using namespace std;

Suitplay::Suitplay(GamePlay game, card hand, int position) : Play(game,hand,position) {
  if (game.game == 0) trumpsuit = -1;
  if (game.game == 1) trumpsuit = -1;
  if (game.game == 2) trumpsuit = -1;
  if (game.game == 3) trumpsuit = -1;
  if (game.game == 4) trumpsuit = -1;
  if (game.game == 9) trumpsuit = 3; 
  if (game.game == 10) trumpsuit = 2;
  if (game.game == 11) trumpsuit = 1;
  if (game.game == 12) trumpsuit = 0;
  
  if (game.game == 0) trumpmask = 0; 
  if (game.game == 1) trumpmask = ~0;
  if (game.game == 2) trumpmask = JACKS;
  if (game.game == 3) trumpmask = QUEENS;
  if (game.game == 4) trumpmask = (HT|JACKS|QUEENS|DIAMONDS);
  if (game.game == 9) trumpmask = DIAMONDS;
  if (game.game == 10) trumpmask = HEARTS; 
  if (game.game == 11) trumpmask = SPADES; 
  if (game.game == 12) trumpmask = CLUBS;
  total = new int[DECKSIZE];
  inverse = new int[DECKSIZE];
  for (int i=0;i<DECKSIZE;i++) inverse[i] = g.inverse[i];
  for (int i=0;i<DECKSIZE;i++) total[i] = g.total[i];
  //  for(int i=0;i<DECKSIZE;i++) { cout << card_name[total[i]] << ";"; }
  //  cout << endl;
}

int Suitplay::propose_declarers_card(card hand, card played) { // declarer issues card
  //  cout << " TM "; g.print_vector(trumpmask,trumpmask); cout << ";decl;" << endl;
  //  cout << " HAND "; g.print_vector(hand,trumpmask); cout << ";decl;" << endl;
  card reduced = g.playable_cards(hand,played); // only playable cards considered
  int top = g.largest(reduced);
  int flop = g.smallest(reduced);
  //  cout << "pdc/0:"; g.print_vector(reduced,trumpmask);cout << endl;
  if (count(reduced) == 0) { cout << "no card left pdc/0" << endl; exit(1); }
  //  cout << " HERE "; g.print_vector(reduced,trumpmask); cout << ";decl;" << endl;
  //    cout << " top " << card_name[top] << "/" << top << endl;  cout << " flop " << card_name[flop] << "/" << flop << endl;
  if (g.team[1] == 1 && g.team[2] == 1 && g.team[3] == 1) {
    //        cout << "issuing in solo" << endl;
    if (count(trumpmask & ~hand & ~played) > 0) {
      //      cout << "other have trump too" << endl;
      if (g.has_largest_trump(hand,reduced,played)) {
	//	cout << "have largest trump playing it" << endl;
	return g.largest_trump(reduced);
      }
    }  
#ifdef TAROT
    if (count(~trumpmask & reduced) > 0) {
      if (count(~trumpmask & reduced & KINGS) > 0)
	return g.smallest(~trumpmask & reduced & KINGS);
      for (int i=0;i<SUITCOUNT;i++) {
	if (count(~trumpmask & reduced & QUEENS & g.Mask[i]) > 0 &&
	    count((played|g.skat) & ~hand & reduced & KINGS & g.Mask[i]) == 1)
	  return g.smallest(~trumpmask & reduced & QUEENS & g.Mask[i]);
	if (count(~trumpmask & reduced & g.Mask[i]) == 1) {
	  return g.smallest(~trumpmask & reduced & g.Mask[i]);
	}
      }
      return g.smallest(~trumpmask & reduced);
    }
#endif
  }
#ifndef TAROT
  if (count(~trumpmask & reduced & ACES) > 0) {
    int s = g.smallest(reduced & ~trumpmask & ACES);
    int suit = Suit[s];
    
    if (count(reduced & ~trumpmask & g.Mask[suit]) > 3 ||
	count(trumpmask & ~hand & ~played) == 0)
      return g.smallest(reduced & ~trumpmask & ACES);
  }
#endif
  if (count(trumpmask & ~hand & ~played) == 0) {
    if (reduced & ~trumpmask) {
      card vo = g.fromabovesuit(reduced,played);
      if (vo & reduced)
	return g.largest(vo & reduced);
    }
  }

  if (reduced & trumpmask) {
    card vo = g.fromabovetrump(reduced,played);
    if (count(vo) > count(~hand & ~played & trumpmask)) {
      if (reduced & vo)
	return g.largest_trump(reduced & vo);
    }
    return g.small_trump(reduced & trumpmask);
  }
  return g.largest(reduced); 
}
int Suitplay::propose_declarers_card(card hand, int trick1, card played) {
  int winner = trick1;
  card table = (ONE<<trick1);
  int i[4] = {-1,-1,-1,-1};
  int issuer = (playerpos+3) % PLAYER; 
  i[issuer] = trick1;
  //  cout << " HERE "; g.print_vector(hand,trumpmask); cout << "; issuer" << issuer << endl;
  //  cout << " TABLE "; g.print_vector(table,trumpmask); cout << "; issuer's card " << card_name[issuer] << endl;
  card reduced = g.playable_cards(hand,issuer,played,table,i); // only playable cards considered
  //  cout << " HERE "; g.print_vector(reduced,trumpmask); cout << "; issuer " << issuer << endl;
  //  cout << " top " << card_name[top] << "/" << top << endl;  cout << " flop " << card_name[flop] << "/" << flop << endl;  cout << "winner " << card_name[winner] << "/" << winner << endl;  cout << "win with me " << card_name[winwithme] << "/" << winwithme << endl;
  int top = g.largest(reduced);
  int flop = g.smallest(reduced);
  int winwithme = g.win(trick1,top,trumpmask);
  if (count(reduced) == 0) { cout << "no card left pdc/1" << endl; exit(1); }
  //  cout << "pdc/1:" << card_name[trick1] << endl; g.print_vector(reduced,trumpmask);cout << endl;  if (count(reduced) == 0) { cout << "no card left pdc/1" << endl; exit(1); }
  //  g.print_vector(reduced,trumpmask); cout << ";decl;" << endl;
  //  cout << " HERE "; g.print_vector(reduced,trumpmask); cout << ";decl;" << endl;
  //  cout << " top " << card_name[top] << "/" << top << endl;  cout << " flop " << card_name[flop] << "/" << flop << endl;  cout << "winner " << card_name[winner] << "/" << winner << endl;  cout << "win with me " << card_name[winwithme] << "/" << winwithme << endl;
  if ((ONE<<trick1) & trumpmask) { // have to obey trump
    if (reduced & trumpmask) {
      if (winwithme == top) {
	card h = g.higher_value(winner,trumpmask) & reduced;
	int s = g.smallest(h);
	if (VALUE(s)<10) 
	  return s;
	else {
	  if (h & ~(ONE<<s)) {
	    h &= ~(ONE<<s);
	    return g.smallest(h);
	  }
	}
	return g.smallest(g.higher_value(trick1,trumpmask) & reduced);
      }
      return g.smallest_trump(reduced & trumpmask);
    }
    return g.smallest(reduced);
  }
  else {
    int suit = g.Suit[trick1];
    if (reduced & g.Mask[suit]) { // have to obey
      if (winwithme == top) {
	return g.smallest(g.higher_value(trick1,trumpmask) & reduced);
      }
      return g.smallest(reduced);
    }
    else { // can cut
      if (reduced & trumpmask) 
	return g.smallest_trump(reduced & trumpmask);
      else
	return g.smallest(reduced);
    }
  }
  return g.smallest(reduced);
}

int Suitplay::propose_declarers_card(card hand, int trick1, int trick2, card played) {
  int winner = g.win(trick1,trick2,trumpmask);
  card table = (ONE<<trick1)|(ONE<<trick2);
  int i[PLAYER] = {-1,-1,-1,-1};
  i[(playerpos+3) % PLAYER] = trick2; i[(playerpos+2) % PLAYER] = trick1;
  int issuer = (playerpos+2) % PLAYER; 
  card reduced = g.playable_cards(hand,issuer,played,table,i); // only playable cards considered
  int top = g.largest(reduced);
  int flop = g.smallest(reduced);
  int winwithme = g.win(trick1,trick2,top,trumpmask);
  //  cout << "pdc/2:" << card_name[trick1]<< card_name[trick2] << endl; g.print_vector(reduced,trumpmask);cout << endl;
  if (count(reduced) == 0) { cout << "no card left pdc/2" << endl; exit(1); }

  if ((ONE<<trick1) & trumpmask) { // have to obey trump
    if (reduced & trumpmask) {
      if ((winner == trick2) && (winwithme == top)) {
	card h = g.higher_value(winner,trumpmask) & reduced;
	int s = g.smallest(h);
	if (VALUE(s)<10) 
	  return s;
	else {
	  if (h & ~(ONE<<s)) {
	    h &= ~(ONE<<s);
	    return g.smallest(h);
	  }
	}
	return g.smallest(g.higher_value(winner,trumpmask) & reduced);
      }
      if ((ONE<<winner) & ~trumpmask)
	return g.smallest_trump(reduced & trumpmask);
      return g.smallest(reduced);
    }
  }
  else {
    int suit = g.Suit[trick1];
    if (reduced & g.Mask[suit]) { // have to obey
      if ((winner == trick2) && (winwithme == top)) {
	return g.smallest(g.higher_value(winner,trumpmask) & reduced);
      }
      return g.smallest(reduced);
    }
    else { // can cut
      if (reduced & trumpmask) 
	return g.smallest_trump(reduced & trumpmask);
      else
	return g.smallest(reduced);
    }
  }
  return g.smallest(reduced);
}

int Suitplay::propose_declarers_card(card hand, int trick1, int trick2, int trick3, card played) {
  int winner = g.win(trick1,trick2,trick3,trumpmask);
  card table = (ONE<<trick1)|(ONE<<trick2)|(ONE<<trick3);
  int i[PLAYER] = {-1,-1,-1,-1};
  i[(playerpos+3) % PLAYER] = trick3; i[(playerpos+2) % PLAYER] = trick2; i[(playerpos+1) % PLAYER] = trick1;
  int issuer = (playerpos+1) % PLAYER;
  card reduced = g.playable_cards(hand,issuer,played,table,i); // only playable cards considered
  int top = g.largest(reduced);
  int flop = g.smallest(reduced);
  int winwithme = g.win(trick1,trick2,trick3,top,trumpmask);
  //  cout << "pdc/3:" << card_name[trick1]<< card_name[trick2] <<card_name[trick3]<< endl; g.print_vector(reduced,trumpmask);cout << endl;  if (count(reduced) == 0) { cout << "no card left pdc/3" << endl; exit(1); }
  
  if ((ONE<<trick1) & trumpmask) {
    if (reduced & trumpmask) { // have to obey trump
      if ((winner == trick1 || winner == trick3) && (winwithme == top)) {
	return g.smallest(g.higher_value(winner,trumpmask) & hand);
      }
      return g.smallest_trump(reduced & trumpmask);
    }
    else { // cannot obey trump
      if ((winner == trick1 || winner == trick3) && (winwithme == top)) { // can overtake opp
	return g.smallest(g.higher_value(winner,trumpmask) & reduced); // take
      }
      else {
	if (winner == trick2)  // can overtake opp
	  return g.largest(reduced);
	else
	  return g.smallest(reduced);
      }
    }
  }
  else { // non trump being played
    int suit = g.Suit[trick1];
    if (reduced & g.Mask[suit]) { // have to obey 
      if ((winner == trick1 || winner == trick3) && (winwithme == top)) {
	return g.smallest(g.higher_value(winner,trumpmask) & hand);
      }
      else {
	if (winner == trick2)  
	  return g.largest(reduced);
	else
	  return g.smallest(reduced);
      }
    }
    else { // dont have to obey
      if (reduced & trumpmask) 
	return g.smallest_trump(reduced & trumpmask);
      else
	return g.smallest(reduced);
    }
  }
  return g.smallest(reduced); // default
}

int Suitplay::propose_opponents_card(card hand, card played) {
  card reduced = g.playable_cards(hand,played); // only playable cards considered
  if (count(reduced) == 0) { cout << "no card left poc/0" << endl; exit(1); }
  int top = g.largest(reduced);
  int flop = g.smallest(reduced);
  //  cout << "poc/0:"; g.print_vector(reduced,trumpmask);cout << endl;
  if (reduced & trumpmask) {
    card vo = g.fromabovetrump(hand,played);
    if (count(vo) > count(~hand & ~played & trumpmask))
      return g.largest_trump(reduced & trumpmask);
  }
  if (reduced & ~trumpmask) {
    int largest = g.largest(reduced & ~trumpmask);
    int suit = g.Suit[largest];
    if (g.ALLBITS & ~hand & ~played & g.Mask[suit] & ~trumpmask) {
      int otherlargest = g.largest(g.ALLBITS & ~hand & g.Mask[suit] & ~hand & ~trumpmask);
      if (g.win(largest,otherlargest,trumpmask) == largest)
	return largest;
      else 
	return g.smallest(reduced & ~trumpmask);
    }
    return g.largest(reduced & ~trumpmask);
  }
  return g.smallest(reduced);
}

int Suitplay::propose_opponents_card(card hand, card played, int trick1) {
  int winner = trick1;
  card table = (ONE<<trick1);
  int i[PLAYER] = {-1,-1,-1,-1};
  i[(playerpos+3) % PLAYER] = trick1; 
  int issuer = (playerpos+3) % PLAYER;
  card reduced = g.playable_cards(hand,issuer,played,table,i); // only consider valid cards
  int top = g.largest(reduced);
  int flop = g.smallest(reduced);
  cout << "poc/1:" << card_name[trick1]  << endl; g.print_vector(reduced,trumpmask); cout << endl;
  if (count(reduced) == 0) { cout << "no card left poc/1" << endl; exit(1); }
  int winwithme = g.win(trick1,top,trumpmask);
  //  g.print_vector(reduced,trumpmask); cout << ";opp;" << choose_smallest(reduced) << endl;
  //  g.print_vector(reduced,trumpmask); cout << ";opp;" << card_name[smallest(reduced)] << endl;

  if ((ONE<<trick1) & trumpmask) { // have to obey trump
    if (reduced & trumpmask) {
      if (winwithme == top) {
	card h = g.higher_value(winner,trumpmask) & reduced;
	int s = g.smallest(h);
	if (VALUE(s)<10) 
	  return s;
	else {
	  if (h & ~(ONE<<s)) {
	    h &= ~(ONE<<s);
	    return g.smallest(h);
	  }
	}
	return g.smallest(g.higher_value(trick1,trumpmask) & reduced);
      }
      return g.smallest_trump(reduced & trumpmask);
    }
    return g.smallest(reduced);
  }
  else { // non-trump being played
    int suit = g.Suit[trick1];
    if (reduced & g.Mask[suit]) { // have to obey
      if (winwithme == top) {
	card h = g.higher_value(trick1,trumpmask) & reduced;
	int s = g.smallest(h);
	if (VALUE(s)<10) 
	  return s;
	else {
	  if (h & ~(ONE<<s)) {
	    h &= ~(ONE<<s);
	    return g.smallest(h);
	  }
	}
	return g.smallest(g.higher_value(trick1,trumpmask) & reduced);
      }
      return g.smallest(reduced);
    }
    else { // can cut
      if (reduced & trumpmask) 
	return g.smallest_trump(reduced & trumpmask);
      else
	return g.smallest(reduced);
    }
  }
  return g.smallest(reduced);
}

int Suitplay::propose_opponents_card(card hand, card played, int trick1, int trick2) {
  card table = (ONE<<trick1)|(ONE<<trick2);
  int winner = g.win(trick1,trick2,trumpmask);
  int i[PLAYER] = {-1,-1,-1,-1};
  i[(playerpos+3) % PLAYER] = trick2; i[(playerpos+2) % PLAYER] = trick1;
  int issuer = (playerpos+2) % PLAYER;
  card reduced = g.playable_cards(hand,issuer,played,table,i);
  int top = g.largest(reduced);
  int flop = g.smallest(reduced); 
  int winwithme = g.win(trick1,trick2,top,trumpmask);
  //  cout << "poc/2:" << card_name[trick1]<< card_name[trick2] << endl; g.print_vector(reduced,trumpmask); cout << endl;
  if (count(reduced) == 0) { cout << "no card left poc/2" << endl; exit(1); }
  //  cout << " top " << card_name[top] << "/" << top << endl;  cout << " flop " << card_name[flop] << "/" << flop << endl;  cout << "winner " << card_name[winner] << "/" << winner << endl;  cout << "win with me " << card_name[winwithme] << "/" << winwithme << endl;
  if ((ONE<<trick1) & trumpmask) { // trump being played
    if (reduced & trumpmask) {
      if ((winner == trick2) && (winwithme == top)) {
	return g.smallest(g.higher_value(winner,trumpmask) & reduced);
      }
      if ((ONE<<winner) & ~trumpmask)
	return g.smallest_trump(reduced & trumpmask);
    }
    return g.smallest(reduced);
  }
  else { // non-trump played
    if (winner == trick2) {
      if (winwithme == top) {
	card h = g.higher_value(winner,trumpmask) & reduced;
	int s = g.smallest(h);
	if (VALUE(s)<10) 
	  return s;
	else {
	  if (h & ~(ONE<<s)) {
	    h &= ~(ONE<<s);
	    return g.smallest(h);
	  }
	}
	return g.smallest(g.higher_value(winner,trumpmask) & reduced);
      }
      return g.largest(reduced);
    }
    if (winner == trick1) {
      if (winwithme == top) {
	return top;
	//	card h = g.higher_value(winner,trumpmask) & reduced;
	//	return g.smallest(h);
      }
      return g.smallest(reduced);
    }
  }
  return g.smallest(reduced);
}

int Suitplay::propose_opponents_card(card hand, card played, int trick1, int trick2, int trick3) {
  card table = (ONE<<trick1)|(ONE<<trick2)|(ONE<<trick3);
  int winner = g.win(trick1,trick2,trick3,trumpmask);
  int i[PLAYER] = {-1,-1,-1,-1};
  i[(playerpos+3) % PLAYER] = trick3; i[(playerpos+2) % PLAYER] = trick2; i[(playerpos+1) % PLAYER] = trick1;
  int issuer = (playerpos+1) % PLAYER;
  card reduced = g.playable_cards(hand,issuer,played,table,i);
  int top = g.largest(reduced);
  int flop = g.smallest(reduced);
  int winwithme = g.win(trick1,trick2,trick3,top,trumpmask);
  //  cout << "poc/3:" << card_name[trick1] << trick1 << card_name[trick2] << trick2 << card_name[trick3] << trick3 << endl; g.print_vector(reduced,trumpmask); cout << endl;
  //  cout << "top/flop/winwithme:" << card_name[top] << top << card_name[flop] << flop << card_name[winwithme]<< winwithme<< endl; cout << endl;
  if (count(reduced) == 0) { cout << "no card left poc/3" << endl; exit(1); }

  if ((ONE<<trick1) & trumpmask) {
    if (reduced & trumpmask) { // have to obey trump
      if ((winner == trick1 || winner == trick3) && (winwithme == top)) {
	int s = g.smallest(g.higher_value(winner,trumpmask) & reduced);
	//	cout << "trying to beat, selecting "<< card_name[s] << endl;
	return g.smallest(g.higher_value(winner,trumpmask) & reduced);
      }
      return g.smallest_trump(reduced & trumpmask);
    }
    else { // cannot obey trump
      if ((winner == trick1 || winner == trick3) && (winwithme == top)) { // can overtake opp
	return g.smallest(g.higher_value(winner,trumpmask) & reduced); // take
      }
      else {
	if (winner == trick2)  // can overtake opp
	  return g.largest(reduced);
	else
	  return g.smallest(reduced);
      }
    }
  }
  else { // non trump being played
    int suit = g.Suit[trick1];
    if (reduced & g.Mask[suit]) { // have to obey 
      if ((winner == trick1 || winner == trick3) && (winwithme == top)) {
	return g.smallest(g.higher_value(winner,trumpmask) & hand);
      }
      else {
	if (winner == trick2)  
	  return g.largest(reduced);
	else
	  return g.smallest(reduced);
      }
    }
    else { // dont have to obey
      if (reduced & trumpmask) 
	return g.smallest_trump(reduced & trumpmask);
      else
	return g.smallest(reduced);
    }
  }
  return g.smallest(reduced);
}

