#include "card.h"

card deck = (card)(((long long unsigned)1 << N_CARDS) - 1);

void print_vector(const card &c) {
  card deck_cp = deck;
  while (deck_cp) {
    card i = ONE << __builtin_ctzll(deck_cp);
    deck_cp &= ~i;
    if (c & i)
      cout << 1;
    else
      cout << 0;
    cout << endl;
  }
}

list<card> set_cards(card x) {
  list<card> res;
  for (card c = 1; (c <= deck && c > 0); c = c << 1)
    if (c & x) res.push_back(c);
  return res;
}

card higher(const card &c) { return ~((c << 1) - 1); }

card lower(const card &c) { return c - 1; }

list<card> deal(card deck, const list<int> distribution, int seed) {
  list<card> res;
  card hand;
  srand(seed);

  // for all number of cards to be dealt initialize an empty hand and
  for (int n_to_deal : distribution) {
    hand = 0;
    for (card i = ONE << (rand() % N_CARDS);
         __builtin_popcountll(hand) < n_to_deal; i = ONE << (rand() % N_CARDS))
      if (i & deck) {  // the card chosen is in the deck
        hand |= i;     // update hand
        deck &= ~i;    // update deck
      }
    res.push_back(hand);
  }
  return res;
}