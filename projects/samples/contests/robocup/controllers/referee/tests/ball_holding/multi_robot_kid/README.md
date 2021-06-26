This situation include 3 variations: a,b,c when they differ, it is explicitely
specified.

## Setup:

- GameState is READY, ball is in play and the game is a KidSize game
- R1 is at less than 30 centimeters from the ball
- R2 is exactly on the other side of the ball
  - a+b. 30 centimeters away from the ball
  - c. 1 meters away from the ball
- a+c. R1 and R2 are from the same team
- b. R1 and R2 are from opposite team

## Procedure:

- 1 seconds elapses with all robots static

## Expected outcome

- a. A free kick in favor of the opposite team is given, no robot is penalized.
- b+c. Free kick is not called, no robot is penalized.
