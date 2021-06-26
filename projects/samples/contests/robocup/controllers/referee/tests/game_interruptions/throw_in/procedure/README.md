# Game Interruption - throw-in procedure

## What is tested

- If the ball is still partially inside the field after being kicked, throw-in
  is not awarded.
- If the ball moves entirely out of the field from a touchline, a throw-in is
  awarded against the team that touched ball last.
- Ball is replaced where it left the field on the touch line.
- Time spent in phase 0 is 10 seconds
- If the team that is awarded the throw-in touches the ball during phase 1,
  throw-in is aborted, the ball is in play .
  - In this case, robots from the opponent team that are still in play are not
    getting penalized.

## Setup

- Team RED has Kick-off and is on left side
- One robot from each team are used

## Description

1. RED 1 touches the ball after PLAYING, the ball rolls over the touchline but
   part of it remains inside the field.
2. No throw-in is called
3. Ball entirely crosses the line.
4. Throw-in for team BLUE is called and the ball is respawned where it left the
   field.
5. During phase 1, robot BLUE 1 kicks the ball while RED 1 is closer than 75
   centimeters from the ball.
6. Ball is in play, BLUE 1 receives a warning and RED 1 is not penalized.
