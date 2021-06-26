This situation include 3 variations: a,b,c, when they differ, it is explicitely
specified.

All sequences are applied sequentially.

## Setup:

- GameState is READY and ball is in play
- Two unpenalized robots of the same team, R1 and R2 are close
  - a: In their penalty area
  - b+c: Outside from their penalty area
- The ball is placed:
  - a+b: Less than 2 meters from the robot
  - c: More than 2 meters away from the robots

## Procedure:

- a. They enter in collision repeatedly over 2 seconds
- b. R1 moves at speed > 0.2 m/s towards R2 who is static
- c. R1 and R2 move toward each other at speed < 0.2 m/s

## Expected outcome

- R1 and R2 are not penalized, no free kick is called

## Note

This is an application of the rules, however I'm personally concerned that
allowing a might end up slowing significantly the simulation if two robots of
the same team run into another for a long duration.
