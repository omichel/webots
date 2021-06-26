# Dropped Ball

## What is tested

- The dropped ball is called approximatively 2 minutes ball was touched
- GameController state is set appropriately
- Ball is replaced according to the rules (field center, on SET)
- Robots at an invalid position when SET is called in a Dropped Ball are
  penalized
- Robots can enter the center circle immediately after play

## Setup

- Team BLUE has kickoff

## Description

1. BLUE 1 kicks the ball once state is PLAYING (t=2)
2. RED 2 touches the ball afterwards (t=12)
3. For 2 minutes, state is PLAYING (until t=131)
4. After 2 minutes (from t=138), dropped ball is called:
5. During the READY phase, robots are moved to valid positions except:
   - RED 2 who moves partially inside the center circle
   - BLUE 2 who is fully inside center circle
   - BLUE 3 who moves inside BLUE side of the field
6. State goes to SET, Robots BLUE 2, BLUE 3 and RED 2 are penalized, not the others.
7. State goes to PLAYING and BLUE 1 and RED 3 enter the center circle immediately.
8. BLUE 1 and RED 3 are not penalized
