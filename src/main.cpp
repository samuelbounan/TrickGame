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

  // for (int p = 0; p < N_PLAYERS; p++)
  //   for (int i = 0; i < N_PLAYERS; i++) player[p].have_not[i] =
  //   ~player[i].hand;
  bidding(&game, player, PRINTING);
  // cout << "aos 0: " << aos(&game, player[0].have_not, 0) << endl;
  // cout << "aos 10: " << aos(&game, player[0].have_not, 10) << endl;
  // cout << "aos 20: " << aos(&game, player[0].have_not, 20) << endl;
  // cout << "aos 30: " << aos(&game, player[0].have_not, 30) << endl;
  // cout << "aos 40: " << aos(&game, player[0].have_not, 40) << endl;
  // cout << "aos 50: " << aos(&game, player[0].have_not, 50) << endl;
  // cout << "aos 60: " << aos(&game, player[0].have_not, 60) << endl;
  // cout << "aos 70: " << aos(&game, player[0].have_not, 70) << endl;
  // cout << "aos 80: " << aos(&game, player[0].have_not, 80) << endl;
  // cout << "aos 90: " << aos(&game, player[0].have_not, 90) << endl;
  // cout << "aos 100: " << aos(&game, player[0].have_not, 100) << endl;
  // cout << "aos 110: " << aos(&game, player[0].have_not, 110) << endl;
  // cout << "aos 120: " << aos(&game, player[0].have_not, 120) << endl;
  // cout << "aos 130: " << aos(&game, player[0].have_not, 130) << endl;
  // cout << "aos 140: " << aos(&game, player[0].have_not, 140) << endl;
  // cout << "aos 150: " << aos(&game, player[0].have_not, 150) << endl;
  // cout << "aos 160: " << aos(&game, player[0].have_not, 160) << endl;
  // cout << "aos 170: " << aos(&game, player[0].have_not, 170) << endl;
  trickgame(&game, player, PRINTING);
  return 0;
}