#include "main.h"

using namespace std;

int main() {
  // initialization
  Player player[N_PLAYERS];
  for (int i = 0; i < N_PLAYERS; i++) {
    player[i].id = i;
    player[i].trickstrat = &pimc;
  }
  Game game;

  // playing
  deal_hands(player, 0);
  bidding(&game, player, PRINTING);
  card world[N_PLAYERS];
  for (int i = 0; i < N_PLAYERS; i++) world[i] = player[i].hand;
  for (int p = 0; p < N_PLAYERS; p++)
    for (int i = 0; i < N_PLAYERS; i++) player[p].have_not[i] = ~player[i].hand;
  cout << "aos 0: " << aos(&game, world, 0) << endl;
  cout << "aos 10: " << aos(&game, world, 10) << endl;
  cout << "aos 20: " << aos(&game, world, 20) << endl;
  cout << "aos 30: " << aos(&game, world, 30) << endl;
  cout << "aos 40: " << aos(&game, world, 40) << endl;
  cout << "aos 50: " << aos(&game, world, 50) << endl;
  cout << "aos 60: " << aos(&game, world, 60) << endl;
  cout << "aos 70: " << aos(&game, world, 70) << endl;
  cout << "aos 80: " << aos(&game, world, 80) << endl;
  cout << "aos 90: " << aos(&game, world, 90) << endl;
  cout << "aos 100: " << aos(&game, world, 100) << endl;
  cout << "aos 110: " << aos(&game, world, 110) << endl;
  cout << "aos 120: " << aos(&game, world, 120) << endl;
  cout << "aos 130: " << aos(&game, world, 130) << endl;
  cout << "aos 140: " << aos(&game, world, 140) << endl;
  cout << "aos 150: " << aos(&game, world, 150) << endl;
  cout << "aos 160: " << aos(&game, world, 160) << endl;
  cout << "aos 170: " << aos(&game, world, 170) << endl;
  trickgame(&game, player, PRINTING);
  return 0;
}