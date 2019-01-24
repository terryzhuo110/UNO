// The player of Final Card-Down. v1.1.0
//
// !!! DO NOT CHANGE THIS FILE !!!

#ifndef PLAYER_H
#define PLAYER_H

#include "Game.h"

// This function is to be implemented by the AI.
// It will be called when the player can take an action on their turn.
// The function is called repeatedly until they make the action
// END_TURN.
// If the player's turn is skipped, this funciton is not called for that
// player.
playerMove decideMove(Game game);

#endif
