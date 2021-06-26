# Second half

## What is tested

- Automatic transition to 2nd half
- Automatic transition between state during 2nd half kickoff
- Spawning robots with change of sides
- Illegal positioning take into account that sides have changed
- Kickoff is attributed to the other team

## Setup

- Team RED has kickoff

## Description

1. 10 minutes of game time in the first half elapses. The AutoRef calls the first half time to be over.
2. The robots respawn on the starting positions at the side of the field.
3. 15 seconds (real-time) elapse.
4. The game state switches to READY.
5. RED 1 and BLUE 1 are placed according to the rules. RED 2 is placed in the
   center circle, BLUE 2 is placed in RED side of the field.
6. After 45 seconds game state switches to SET.
7. RED 2 and BLUE 2 are penalized
7. After 5 seconds, the game state switches to PLAY.
