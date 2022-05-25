# TrickGame

<img src="img/deck.png" alt="deck" width="500"/>

## About The Project

General playground for a tricktaking card game with bids.

## Getting Started

### Prerequisites

* (mpi ```sudo apt install mpicc```)

### Usage

1. Define the rules wanted in the Makefile changing the line
`CXXFLAGS += -DRULES_XXX`, XXX in {BASIC, CONTREE}
2. Define the global constant for the game in 'card.h' (highly dependent on the rules see at the beginning of 'XXX.cpp')
3. Setup the strategies wanted for each player and the wanted behavior of a game (how to deal cards, number of games, how to show results) in 'main.cpp'
4. run ```sh
make```

## Code

### Architecture
**main**: initiates game+players and simulates a game

**play**:  general functions to organize the playing of a game

**player**: define the functionalities of the Player class, with his knowledge and his strategy

**bidstrat/**: folder of different bidding strategies for a *player*

**trickstrat/**: folder of different tricktaking strategies for a player

**rules**: functions that directly depends on the rules of the game chosen (rules.cpp: functions that could be defined independently of the rules/rules/xxx.cpp: functions dependent)

**game**: class representing the state of a game when played, with simple primitives

**card**: basic utils functions with the manipulation of card type

### Notes

- The number of cards used has to be under 64
- Careful if suits are not bit adjacent
- Binary output of the game: win/loose
- Each trick every player plays
- AOS requires only two teams
- game over when N_ROUNDS are played