/* Base*/

#ifndef __BASE
#define __BASE

#include <unistd.h>  // sleep

#include <ctime>     // for seed
#include <iostream>  // cout
#include <list>      // for lists
#include <random>    // for random generation
using namespace std;

#define D(x) cout << " " << #x << ": " << x << endl;

/* ADAPT TO THE GAME */
#define N_PLAYERS 4
#define N_ROUNDS 8
#define N_CARDS 32
#define N_SUITS 4
#define N_BIDS 43
#define SIZE_HAND 8
#define N_TEAMS 2
///////////////////////

#if N_CARDS <= 32
typedef unsigned int card;  // reprezents several cards (bit vector)
#elif N_CARDS <= 64
typedef unsigned long long int card;
#endif
extern card deck;  // set of all cards

/**
 * @brief transform an index into a card
 *
 * @param i index
 * @return card (1 << i)
 */
card _card(int i);

/**
 * @brief prints card in bit vector representation
 *
 * @param c card to print
 */
void print_vector(card c);

/**
 * @brief transforms a set of cards into a list of every card in it
 *
 * @param x set of cards
 * @return list<card> enumeration of x
 */
list<card> set_cards(card x);

/**
 * @brief cards strictly higher than c
 *
 * @param c
 * @return card
 */
card higher(card i);

/**
 * @brief cards strictly lower than c
 *
 * @param c
 * @return card
 */
card lower(card i);

/**
 * @brief returns a list of card dealt according to the distribution
 *
 * @param deck cards to deal
 * @param distribution {2,3} will return a hand of 2 cards followed by a hand of
 * 3
 * @return list<card> hands dealt
 */
list<card> deal(card deck, list<int> distribution);

#endif