#include "aos.h"

card aos(Game game, int id, card hand, card *have_not) {
  // if game is over, return a card corresponding to the output of the game
  if (end_trickgame(&game)) {
    if (won(id, game)) return deck;
    return 0;
  }

  // allocate space for virtual players and game
  Player virtual_player[N_PLAYERS];
  for (int i = 0; i < N_PLAYERS; i++) {
    virtual_player[i].id = i;
    virtual_player[i].trickstrat = &aos;
  }
  Game virtual_game;

  // branch on every playable card c
  for (card c : set_cards(playable(hand, game))) {
    // initiate virtual players and game
    virtual_game = game;
    for (int i = 0; i < N_PLAYERS; i++) {
      virtual_player[i].hand = ~(have_not[i]);
      copy_n(have_not, N_PLAYERS, virtual_player[i].have_not);
    }

    // play c in virtual game
    virtual_player[id].hand &= ~c;
    update_card(&virtual_game, virtual_player, c);

    // ask the next virtual player to move, and return according to his play
    card next_c = virtual_player[virtual_game.turn].playCard(virtual_game);
    if ((game.team[virtual_game.turn] == game.team[id]) != (next_c == 0))
      return c;
  }
  return 0;
}