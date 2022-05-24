**Description**
General playground for a tricktaking card game with bids

**Howto**
Define the rules wanted in the Makefile with
`CXXFLAGS += -DRULES_XXX`, XXX in {BASIC, CONTREE}
Define the global constant for the game in 'card.h' (highly dependent on the rules)

**Architecture**
main            -> initiates game+players and simulates a game
play            -> general functions to organize the playing of a game
player          -> define the functionalities of the Player class, with his knowledge and his strategy
bidstrat/       -> folder of different bidding strategies for a player
trickstrat/     -> folder of different tricktaking strategies for a player
rules           -> functions that directly depends on the rules of the game chosen
                  rules.cpp: functions that could be defined independently of the rules
                  rules/xxx.cpp: functions dependent
game            -> class representing the state of a game when played, with simple primitives
card            -> basic utils functions with the manipulation of card type

**Notes**
The number of cards used has to be under 64
Careful if suits are not bit adjacent
Binary output of the game: win/loose
Each trick every player plays
AOS requires only two teams
game over when N_ROUNDS are played