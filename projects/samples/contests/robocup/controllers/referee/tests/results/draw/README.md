# Results - Draw

the procedure of determinig a game as a draw by the AutoRef is going to be tested

## What is tested

- Game can properly end with a draw when using round-robin game
- All transitions leading to a 1-1 result are properly working

## Setup

- Team RED has Kick-off and is on left side
- One robot from each team are used

## Description

1. Team RED score a goal.
2. Team BLUE score a goal.
3. Game time passes untill the end.
4. AutoRef declares the game to be a draw.

## Additional manual checks

The log file from the referee should contain a mention that the game ended as a
draw.
