# Results - Win

Declaring the winner by the AutoRef is going to be tested

## What is tested

- Game can properly end with a winner when using round-robin game
- All transitions leading to a 1-0 result are properly working

## Setup

- Team RED has Kick-off and is on left side
- One robot from each team are used

## Description

1. Team RED score a goal.
2. Game time passes untill the end.
3. AutoRef declares team red winner of the match.

## Additional manual checks

The log file from the referee should contain a mention that the game was won by
team RED.
