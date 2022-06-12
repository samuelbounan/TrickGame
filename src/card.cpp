#include "card.h"

card deck = (card)(((long long unsigned)1 << N_CARDS) - 1);

card _card(int i) { return (card)1 << (i % N_CARDS); }

void print_vector(card c) {
  for (card i : set_cards(deck))
    if (c & i)
      cout << 1;
    else
      cout << 0;
  cout << endl;
}

list<card> set_cards(card x) {
  list<card> res;
  for (card c = 1; (c <= deck && c > 0); c = c << 1)
    if (c & x) res.push_back(c);
  return res;
}

card lower(card c) { return c - 1; }

card higher(card c) { return ~((c << 1) - 1); }

list<card> deal(card deck, list<int> distribution) {
  // srand(time(nullptr));
  srand(0);
  list<card> res;
  card hand;

  // for all number of cards to be dealt initialize an empty hand and
  for (int n_to_deal : distribution) {
    hand = 0;
    for (card i = _card(rand() % N_CARDS);
         __builtin_popcountll(hand) < n_to_deal; i = _card(rand() % N_CARDS))
      if (i & deck) {  // the card chosen is in the deck
        hand |= i;     // update hand
        deck &= ~i;    // update deck
      }
    res.push_back(hand);
  }
  return res;
}