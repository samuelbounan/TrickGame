// 4-player card program using expert knowledge, expert games, belief states, and-or search
// (c) 2022, Stefan Edelkamp

#include "util.h"
#include "cards.h"
#include "game.h"
#include <iostream>

using namespace std;

int GamePlay::any(card hand) {
  int c = count(hand);
  int r = rand()%c;
  for (int i=0;i<r;i++) {
    int index = CTZ(hand);
    hand &= ~(ONE & index);
  }
  return CTZ(hand);
}

card GamePlay::lower_value(int index, card trumpmask) {
  return lower[index];
}

card GamePlay::higher_value(int index, card trumpmask) {
  return higher[index];
}

int GamePlay::has_largest_trump(card hand, card reduced, card played) {
  if (count(reduced & trumpmask) == 0)
    return 0;
  int l = largest_trump(reduced & trumpmask);
  if (count(trumpmask & ~hand & ~played) == 0)
    return 1;
  int o = largest_trump(trumpmask & ~hand & ~played);
  if (((ONE << o) & higher_value(l,trumpmask)))
    return 0;
  return 1;
}

int GamePlay::leading(int j, int k, int l, int m, int issuer, card trumpmask) {
  int w = win(j,k,l,m,trumpmask);
  return (w == j) ? issuer: (w == k) ? (issuer+1) % PLAYER : (w == l) ? (issuer+2) % PLAYER : (issuer+3) % PLAYER;
}

int GamePlay::leading(int j, int k, int l, int issuer, card trumpmask) {
  int w = win(j,k,l,trumpmask);
  return (w == j) ? issuer % PLAYER: (w == k) ? (issuer+1) % PLAYER : (issuer+2) % PLAYER;
}

int GamePlay::leading(int j, int k, int issuer, card trumpmask) {
  int w = win(j,k,trumpmask);
  return (w == j) ? issuer : (issuer+1) % PLAYER;
}

int GamePlay::leading(int j, int issuer, card trumpmask) {
  return issuer;
}

int GamePlay::win(int j, int k, int l, int m, card trumpmask) {
#ifdef TAROT
  if (j == fool) return win(k,l,m,trumpmask);
  if (k == fool) return win(j,l,m,trumpmask);
  if (l == fool) return win(j,k,m,trumpmask);
  if (m == fool) return win(j,k,l,trumpmask);
#endif
  if ((((ONE<<j)|(ONE<<k)|(ONE<<l)|(ONE<<m)) & trumpmask)) {
    //    cout << "winning card trmpcard " << card_name[j]<< card_name[k]<<card_name[l]<<card_name[m] << endl;
    int w = ((ONE<<j) & trumpmask) ? m : -1;
    w = ((ONE<<k) & trumpmask) ? l : w;
    w = ((ONE<<l) & trumpmask) ? k : w;
    w = ((ONE<<m) & trumpmask) ? j : w;

    //    cout << " w " << w << endl; // w is trump

    //    cout << "inversed ordering " << inverse [w]<< ":" << inverse[j]<<" " << inverse[k]<< " "<<inverse[l]<< " "<<inverse[m] << endl;
    //    cout << "total ordering " << total [w]<< ":" << total[j]<<" " << total[k]<< " "<<total[l]<< " "<<total[m] << endl;
    
    w = (((ONE<<j) & trumpmask) && Card[j] != Card[w] && inverse[j] < inverse[w]) ? j : w;
    w = (((ONE<<k) & trumpmask) && Card[k] != Card[w] && inverse[k] < inverse[w]) ? k : w;
    w = (((ONE<<l) & trumpmask) && Card[l] != Card[w] && inverse[l] < inverse[w]) ? l : w;
    w = (((ONE<<m) & trumpmask) && Card[m] != Card[w] && inverse[m] < inverse[w]) ? m : w;
    return w;
 
  }
  int w = j;
  w = (Suit[w] == Suit[k] && Card[w] != Card[k] && inverse[k] < inverse[w]) ? k : w;
  w = (Suit[w] == Suit[l] && Card[w] != Card[l] && inverse[l] < inverse[w]) ? l : w;
  w = (Suit[w] == Suit[m] && Card[w] != Card[m] && inverse[m] < inverse[w]) ? m : w;
  return w;
}

int GamePlay::win(int j, int k, int l, card trumpmask) {
#ifdef TAROT
  if (j == fool) return win(k,l,trumpmask);
  if (k == fool) return win(j,l,trumpmask);
  if (l == fool) return win(j,k,trumpmask);
#endif
  //  cout << card_name[j] << card_name[k] << card_name[l] << endl;
  if ((((ONE<<j)|(ONE<<k)|(ONE<<l)) & trumpmask)) {
    int w = ((ONE<<j) & trumpmask) ? l : -1;
    w = ((ONE<<k) & trumpmask) ? k : w;
    w = ((ONE<<l) & trumpmask) ? j : w;

    w = (((ONE<<j) & trumpmask) && Card[w] != Card[j] && inverse[j] < inverse[w]) ? j : w;
    w = (((ONE<<k) & trumpmask) && Card[w] != Card[k] && inverse[k] < inverse[w]) ? k : w;
    w = (((ONE<<l) & trumpmask) && Card[w] != Card[l] && inverse[l] < inverse[w]) ? l : w;
    return w;
  }
  //  cout << "no trump " << suitorder[j] << ","  << suitorder[k] << ","  << suitorder[l]  << "," << endl;
  int w = j;
  w = (Suit[w] == Suit[k] && Card[w] != Card[k] && inverse[k] < inverse[w]) ? k : w;
  w = (Suit[w] == Suit[l] && Card[w] != Card[l] && inverse[l] < inverse[w]) ? l : w;
  return w;
}

int GamePlay::win(int j, int k, card trumpmask) {
#ifdef TAROT
  if (j == fool) return k;
  if (k == fool) return j;
#endif
  //  cout << card_name[j] << card_name[k] << endl;
  if ((((ONE<<j)|(ONE<<k)) & trumpmask)) {
    int w = ((ONE<<j) & trumpmask) ? k : -1;
    w = ((ONE<<k) & trumpmask) ? j : w;
    w = (((ONE<<j) & trumpmask) && Card[w] != Card[j] && inverse[j] < inverse[w]) ? j : w;
    w = (((ONE<<k) & trumpmask) && Card[w] != Card[k] && inverse[k] < inverse[w]) ? k : w;
    return w;
  }
  //  cout << "no trump " << suitorder[j] << ","  << suitorder[k] << ","  << "," << endl;
  int w = j;
  w = (Suit[w] == Suit[k] && Card[w] != Card[k] && inverse[k] < inverse[w]) ? k : w;
  return w;
}

int GamePlay::equivalent_card(card hand, int index, card playednottable) { 
  //  return 1;
  card ex = hand | (playednottable);
  //  if (!(hand & (ONE<<index))) { print_vector(hand,trumpmask); exit(1); } // good for debugging
  if (index > 0 && (hand & (ONE<<index-1)) && equiv[index][index-1]) return 0;
  if (index > 1 && (ex & (ONE<<index-1)) && (hand & (ONE<<index-2)) && equiv[index][index-2]) return 0;
  //#ifndef BELOTE
  if (index > 2 &&
      ex == (ex & ((ONE<<index-1) | (ONE<<index-2))) &&
      (hand & (ONE<<index-3))  &&
      equiv[index][index-3])
    return 0;
  if (index > 3 &&
      ex == (ex & ((ONE<<index-1) | (ONE<<index-2) | (ONE<<index-3))) &&
      (hand & (ONE<<index-4)) &&
      equiv[index][index-4])
    return 0;
  if (index > 4 &&
      ex == (ex & ((ONE<<index-1) | (ONE<<index-2) | (ONE<<index-3) | (ONE<<index-4))) &&
      (hand & (ONE<<index-5)) &&
       equiv[index][index-5])
    return 0;
  if (index > 5 &&
      ex == (ex & ((ONE<<index-1) | (ONE<<index-2) | (ONE<<index-3) | (ONE<<index-4) | (ONE<<index-5))) &&
      (hand & (ONE<<index-6)) &&
      equiv[index][index-6])
      return 0;
  if (index > 6 &&
      ex == (ex & ((ONE<<index-1) | (ONE<<index-2) | (ONE<<index-3) | (ONE<<index-4) | (ONE<<index-5) | (ONE<<index-6))) &&
      (hand & (ONE<<index-7)) &&
      equiv[index][index-7])
    return 0;
  if (index > 7 &&
      ex == (ex & ((ONE<<index-1) | (ONE<<index-2) | (ONE<<index-3) | (ONE<<index-4) | (ONE<<index-5) | (ONE<<index-6) | (ONE<<index-7))) &&
      (hand & (ONE<<index-8)) &&
      equiv[index][index-8])
    return 0;
  //#endif
  return 1;
}

card GamePlay::playable_cards(card hand, card played) {
  //  cout << "playcards/0 "; ::print_vector(hand,trumpmask); cout << " "; ::print_vector(played,trumpmask); cout << endl;
  card playable = hand;
#ifdef HEART
  if (count(played) == 0)
    if (hand & C2) playable =(hand & C2);
#endif
#ifdef SPADE
  if ((hand & trumpmask) == hand)
    playable = hand;
  else if (played & trumpmask)
    playable = hand;
  else if (hand & trumpmask)
    playable = hand & ~trumpmask;
#endif
  card reduced = playable;
  //  cout << " before reducing/0 "; ::print_extvector(playable,trumpmask); cout << " " << endl;
  while (reduced) {
    int index = largest(reduced);
    card bitindex = (ONE<<index);
    if (!equivalent_card(hand,index,played)) {
      reduced &= ~bitindex;
      playable &= ~bitindex;
      continue;
    }
    reduced &= ~bitindex;
  }
  //  cout << " after reducing/0 "; ::print_extvector(playable,trumpmask); cout << " " << endl;

  return playable;
}

card GamePlay::playable_cards(card hand, int issuer, card played, int* i) {  
  card tab = 0;
  for (int j=0;j<PLAYER;j++)
    if (i[j] != -1) tab |= (ONE<<i[j]);
  return playable_cards(hand,issuer,played,tab,i);
}
  
card GamePlay::playable_cards(card hand, int issuer, card played, card table, int* i) {
  //  cout << "playcards/1 "; cout << issuer; cout << ":"; ::print_vector(hand,trumpmask); cout << " played "; ::print_vector(played,trumpmask); cout << " table "; ::print_vector(table,trumpmask); cout << endl;

  int index = i[issuer];
  card playable = hand;
#if defined(BELOTE) || defined(TAROT)
  int winner = 0;
  int shown = count(table);
  int player = (issuer + shown) % PLAYER;
  //    cout << " shown " << shown << " player P" << player<< endl; 
  if (shown == 1) winner = issuer;
  else if (shown == 2) {
    if (index == win(index,i[(issuer+1) % PLAYER],trumpmask))
      winner = issuer;
    else winner = (issuer+1) % PLAYER;
  }
  else if (shown == 3) {
    int w = win(index,i[(issuer+1) % PLAYER],i[(issuer+2)%PLAYER],trumpmask);
    if (index == w) winner = issuer;
    else if (index == i[(issuer+1) % PLAYER])
      winner = (issuer+1) % PLAYER;
    else winner = (issuer+2) % PLAYER;
  }
  //  cout << "winner" << card_name[i[winner]] << endl; 
#endif
  card playednottable = played & ~table;
  //  cout << "checking "<< card_name[index] << "(" << index << ")" << endl;
  //  print_vector(trumpmask,trumpmask);
  if (trumpmask & (ONE<<index)) {
    if (trumpmask & hand) {// obey trump
      playable = hand & trumpmask;
#ifdef BELOTE
      if (higher_value(i[winner],trumpmask) & hand) {
	if (shown > 1 && (team[winner] == team[player]))
	  playable = hand & trumpmask;
	else 
	  playable = higher_value(i[winner],trumpmask) & hand;
      }
#endif  
    }
    else {
      playable = hand;
    }
  }
  else {
    int suit = Suit[index];    
    //    cout << "suit/1 " << suit << ": " ; print_vector(Mask[suit] & ~trumpmask,trumpmask); cout << endl;
    if (hand & Mask[suit] & ~trumpmask) { // have to obey
      playable = hand & Mask[suit] & ~trumpmask;
    }
    else { // can cut
      playable = hand;
#ifdef TAROT
      playable = hand & trumpmask;
      if (!playable) playable = hand;
      else {
	if (higher_value(i[winner],trumpmask) & hand) {
	  playable = higher_value(i[winner],trumpmask) & hand;
	}
      }
#endif       
#ifdef BELOTE
      if (trumpmask & hand) { // must cut
	if (shown > 1 && (team[winner] == team[player]))
	  playable = hand;
	else 
	  playable = hand & trumpmask;
      }
#endif
    }
  }
  //    cout << " before reducing/1b "; ::print_extvector(playable,trumpmask); cout << " " << endl; 
  card reduced = playable;
  while (reduced) {
    int index = largest(reduced); 
    card bitindex = (ONE<<index);
    if (!equivalent_card(hand,index,playednottable)) {
      reduced &= ~bitindex;
      playable &= ~bitindex;
      continue;
    }
    reduced &= ~bitindex;
  }
  //    cout << " after reducing/1b "; ::print_extvector(playable,trumpmask); cout << " " << endl; 
#ifdef TAROT
  playable |= (reduced & FOOL);
#endif
  return playable;
}

int GamePlay::small_trump(card hand) {
  hand &= trumpmask;
  if (count(hand) >= 2) {
    int s = (WORDSIZE - 1) - CLZ(hand);
    if (VALUE(s) > 10)
      return small_trump(hand & ~(ONE<<s));
  }
#ifdef BELOTE  
  if (hand & ~(JACKS|NINES))
    return (WORDSIZE - 1) - CLZ(hand&~(JACKS|NINES));
  if (hand & NINES) return largest(hand & NINES);
  if (hand & JACKS) return largest(hand & JACKS);
#endif
  return (WORDSIZE - 1) - CLZ(hand);
}

int GamePlay::smallest_trump(card hand) {
  hand &= trumpmask;
#ifdef BELOTE  
  if (hand & ~(JACKS|NINES))
    return (WORDSIZE - 1) - CLZ(hand&~(JACKS|NINES));
  if (hand & NINES) return largest(hand & NINES);
  if (hand & JACKS) return largest(hand & JACKS);
#endif
  return (WORDSIZE - 1) - CLZ(hand);
}

int GamePlay::largest_trump(card hand) {
  hand &= trumpmask;
#ifdef BELOTE  
  if (hand & JACKS) return largest(hand & JACKS);
  if (hand & NINES) return largest(hand & NINES);
#endif
  int index = CTZ(hand);
  return index;
}

int GamePlay::smallest(card hand) {
  int index = (WORDSIZE - 1) - CLZ(hand);
  return index;
}

int GamePlay::smallest_value(card hand) {
#ifdef TAROT
  card h = hand & ACES;
  if (h) return (WORDSIZE - 1) - CLZ(h);
  h = hand & TWOS;
  if (h) return (WORDSIZE - 1) - CLZ(h);
  h = hand & THREES;
  if (h) return (WORDSIZE - 1) - CLZ(h);
  h = hand & FOURS;
  if (h) return (WORDSIZE - 1) - CLZ(h);
  h = hand & FIVES;
  if (h) return (WORDSIZE - 1) - CLZ(h);
  h = hand & SIXS;
  if (h) return (WORDSIZE - 1) - CLZ(h);
  h = hand & SEVENS;
  if (h) return (WORDSIZE - 1) - CLZ(h);
  h = hand & EIGHTS;
  if (h) return (WORDSIZE - 1) - CLZ(h);
#endif
  int index = (WORDSIZE - 1) - CLZ(hand);
  return index;
}

int GamePlay::smallest(card hand, int suit) {
  int index = (WORDSIZE - 1) - CLZ(hand & Mask[suit]);
  return index;
}

int GamePlay::largest(card hand) {
  int index = CTZ(hand);
  return index;
}

int GamePlay::largest(card hand, int suit) {
  int index = CTZ(hand & Mask[suit]);
  return index;
}


card GamePlay::fromabovetrump(card hand, card played) { // vostand
  card vo = 0;
  //  card hand = hand & trumpmask;
  int tops[TRUMPCARDTYPES];
  for (int j = 0; j < TRUMPCARDTYPES; j++) {
    tops[j] = count((played|hand) & abovetrump[j] & trumpmask);
  }
  int c = 0;
  for (int i=0;i<TRUMPCARDTYPES;i++) {
    bool bigand = true;
    for (int j=0;j<i;j++) {
#ifdef DOPPEL
      bigand &= (tops[j] == 2);
#else
      bigand &= (tops[j] == 1);
#endif
    }
    c += (bigand == true) && (hand & abovetrump[i] & trumpmask);
  }
  int countopponents = count(trumpmask & ~hand & ~played);  
  if (c >= countopponents)
    vo |= hand & trumpmask;
  else {
    for (int j = 0; j < TRUMPCARDTYPES; j++) {
      if ((c > 0) && (hand & abovetrump[j] & trumpmask)) {
	vo |= hand & abovetrump[j] & trumpmask; c--; }
      //	else {
      //	  if ((c > 0) && (hand & trumpmask)) {
      //	    vo |= hand & trumpmask; c--; }
    }
  }
  return vo;
}
  
card GamePlay::fromabovesuit(card hand, card played) { // vostand
  card vo = 0;
  for (int suit=0;suit<4;suit++) {
    //    cout << "mask"; ::print_vector(Mask[suit],trumpmask); cout << endl;
    
    if ((Mask[suit] & trumpmask) == 0) {
      int tops[CARDTYPES];
      for (int j = 0; j < CARDTYPES; j++) {
	tops[j] = count((played|hand) & abovesuit[j] & Mask[suit]);
      }
      int c = 0;
      for (int i=0;i<CARDTYPES;i++) {	
	bool bigand = true;
	for (int j=0;j<i;j++) {
#ifdef DOPPEL
	  bigand &= (tops[j] == 2);
#else
	  bigand &= (tops[j] == 1);
#endif
	}
	c += (bigand == true) && (hand & abovesuit[i] & Mask[suit]);
      }	
      int countopponents = count(Mask[suit] & ~hand & ~played);
      if (c >= countopponents)
	vo |= hand & Mask[suit];
      else {
	for (int j = 0; j < CARDTYPES; j++) {
	  if ((c > 0) && (hand & abovesuit[j] & Mask[suit])) {
	    vo |= hand & abovesuit[j] & Mask[suit]; c--; }
	}
      }
    }
  }
  return vo;
}


void GamePlay::print_vector(card hand, card trumpmask) {
   card oldhand = hand;
#ifdef BELOTE
  while (hand) {
    int index = CTZ(hand);
    if ((ONE<<index) & trumpmask & JACKS) 
      std::cout << card_name[index] << " ";
    hand &= ~(ONE << index);
  }
  hand = oldhand & trumpmask & ~JACKS;
  while (hand) {
    int index = CTZ(hand);
    if ((ONE<<index) & trumpmask & NINES) 
      std::cout << card_name[index] << " ";
    hand &= ~(ONE << index);
  }
  hand = oldhand & trumpmask & ~JACKS & ~NINES;
#endif
  while (hand) {
    //cout << "H:"; print_128(hand); cout << endl;
    int index = CTZ(hand);
    if ((ONE<<index) & trumpmask)
      std::cout << card_name[index] << " ";
    hand &= ~(ONE << index);
  }
  for (int suit=0;suit<SUITCOUNT;suit++) {
    //    cout << "M:"; print_128(Mask[suit]); cout << endl;
    hand = oldhand & Mask[suit];
    for (int i=0;i<DECKSIZE;i++) {
      if (hand & (ONE<<total[i]))
	if ((ONE<<total[i]) & ~trumpmask)
	  std::cout << card_name[total[i]] << " ";
    }
    /*
    while (hand) { 
      int index = CTZ(hand);
      if (((ONE<<index) & trumpmask) == 0)
	std::cout << card_name[index] << " ";
      hand &= ~(ONE << index);
    }
    */
  }
}

void GamePlay::print_extvector(card hand, card trumpmask) {
  card oldhand = hand;
#ifdef BELOTE
  while (hand) {
    int index = CTZ(hand);
    if (((ONE<<index) & trumpmask & JACKS) != 0) 
      std::cout << card_name[index] << "[" << index << "]" << " ";
    hand &= ~(ONE << index);
  }
  hand = oldhand & trumpmask & ~JACKS;
  while (hand) {
    int index = CTZ(hand);
    if (((ONE<<index) & trumpmask & NINES) != 0) 
      std::cout << card_name[index] << "[" << index << "]" << " ";
    hand &= ~(ONE << index);
  }
  hand = oldhand & trumpmask & ~NINES & ~JACKS;
#endif
  while (hand) {
    int index = CTZ(hand);
    if (((ONE<<index) & trumpmask) != 0)
      std::cout << card_name[index] << "[" << index << "]" << " ";
    hand &= ~(ONE << index);
  }
  for (int suit=0;suit<SUITCOUNT;suit++) {
    hand = oldhand & Mask[suit];  
    while (hand) { 
      int index = CTZ(hand);
      if (((ONE<<index) & trumpmask) == 0)
	std::cout << card_name[index] << "[" << index << "]" << " ";
      hand &= ~(ONE << index);
    }
  }
}

int GamePlay::strength(card hand) {
  int score = 0;
#ifdef TAROT
  if (hand & (ONE<<t_21)) score += 10;
  if ((hand & (ONE<<t_01)) && count(hand & TRUMPS) >= 5) score += 6;
  if ((hand & (ONE<<t_01)) && count(hand & TRUMPS) >= 6) score += 1;
  if ((hand & (ONE<<t_01)) && count(hand & TRUMPS) >= 7) score += 1;
  if ((hand & (ONE<<t_01)) && count(hand & TRUMPS) >= 8) score += 1;
  if ((hand & (ONE<<t_01)) && count(hand & TRUMPS) >= 9) score += 1;
  if (hand & (ONE<<fool)) score += 7;

  score += count(hand & TRUMPS & ~HONOR) + count(hand & TRUMPS & ~HONOR)+ count(hand & TRUMPS & ~HONOR);

  score += count(hand & ((ONE<<t_16)|(ONE<<t_17)|(ONE<<t_18)|(ONE<<t_19)|(ONE<<t_20)));
  score += count(hand & ((ONE<<t_16)|(ONE<<t_17)|(ONE<<t_18)|(ONE<<t_19)|(ONE<<t_20)));

  score += count(hand & (ONE<<t_16)|(ONE<<t_17)|(ONE<<t_18)) == 3;
  score += count(hand & (ONE<<t_17)|(ONE<<t_18)|(ONE<<t_19)) == 3;
  score += count(hand & (ONE<<t_18)|(ONE<<t_19)|(ONE<<t_20)) == 3; 

  for (int i=0;i<SUITCOUNT;i++) {
    score += 10 * (count (hand & (KINGS | QUEENS) & Mask[i]) == 2);
    score += 6 * count (hand & KINGS & ~QUEENS & Mask[i]);
    if (count (hand & Mask[i]) >= 2) 
      score += 3 * count (hand & QUEENS & ~KINGS & Mask[i]);
    else
      score += 2 * count (hand & QUEENS & ~KINGS & Mask[i]);
    if (count (hand & Mask[i]) >= 2)
      score += 2 * count (hand & KNIGHTS & ~KINGS & Mask[i]);
    else
      score += 1 * count (hand & KNIGHTS & ~QUEENS & Mask[i]);
    if (count (hand & Mask[i]) >= 2) 
      score += 1 * count (hand & JACKS & Mask[i]);

    if (count (hand & Mask[i]) >= 5) score += 5;
    if (count (hand & Mask[i]) >= 6) score += 1;
    if (count (hand & Mask[i]) >= 7) score += 1;
    if (count (hand & Mask[i]) >= 8) score += 1;
    if (count (hand & Mask[i]) >= 9) score += 1;
    if (count (hand & Mask[i]) >= 10) score += 1;
    if (count (hand & Mask[i]) >= 11) score += 1;
    if (count (hand & Mask[i]) >= 12) score += 1;

    if ((game == 7 || game == 8) && count (hand & Mask[i]) == 0) score += 5;
    if ((game == 7 || game == 8) && count (hand & Mask[i]) == 1) score += 3;
    if ((game == 7 || game == 8) && count (hand & Mask[i]) == 2) score += 1;
  }
#endif
  return score;
}

void GamePlay::print_hands(card p[PLAYER], int start, card tm) {
  /*
  card played = ALLBITS;
  for (int i=0;i<PLAYER;i++)
    played &= ~p[i];
  */
  //  cout << "played"; print_vector(played,tm); cout << endl;

  for (int i=0;i<PLAYER;i++) {
    cout << team[i] << ((start == i)? ">" : " ") << "P" << i << " ";
    print_vector(p[i],tm); cout << " (" << sum(p[i],tm) << "," << strength(p[i]) << ")";
    //cout << " standing ";
    //    print_vector(fromabovetrump(p[i],played),tm); cout << "|";
    //    print_vector(fromabovesuit(p[i],played),tm);
    cout << endl;
  }

  if (skat) {
    cout << "  SK ";
    print_vector(skat,tm); cout << " (" << sum(skat,tm) << ") ";
    cout << endl;
  }
  cout << "GamePlay1 ";
  if (tm == ZERO) cout << "NT";
  if (tm == ~ZERO) cout << "AT";
  if (tm == JACKS) cout << "JACKS";
  if (tm == QUEENS) cout << "QUEENS";
#ifdef TAROT
  if (game == 5) cout << "PETIT (1x)";
  if (game == 6) cout << "GARDE (2x)";
  if (game == 7) cout << "GARDE SANS (3x)";
  if (game == 8) cout << "GARDE CONTRE (4x)";
#endif
  if (tm == (HT|JACKS|QUEENS|DIAMONDS)) cout << "NG";
  if (tm == DIAMONDS) cout << suitname[3];
  if (tm == HEARTS) cout << suitname[2];
  if (tm == SPADES) cout << suitname[1];
  if (tm == CLUBS) cout << suitname[0];
  cout <<" (limit "<< limit << " of "<< sumvalues << ")"<< endl;
}

