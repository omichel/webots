# Ball-out - throw-in

## What is tested

- If the ball is still partially inside the field after being kicked, throw-in
  is not awarded.
- If the ball moves entirely out of the field from a touchline, a throw-in is
  awarded against the team that touched ball last.
- Ball is replaced where it left the field on the touch line.

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
