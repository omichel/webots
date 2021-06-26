# Forceful contact - charging close to ball
This situation include 4 variations: A,B,C,D. When they differ, it is explicitly
specified. In all situations, R1 moves away from the ball and R2 moves to the ball

A. R1 and R2 move slowly -> no penalty
B. R1 moves slowly, R2 moves quickly -> R2 is penalized (Note: this is competing for the ball)
C. R1 moves quickly, R2 moves slowly, dist > 1m -> R1 is penalized
D. R1 moves quickly, R2 even more, dist < 1m -> Freekick

## Setup:

- GameState is READY and ball is in play
- Robots R1 and R2 are nearby, they are of opposite team, not penalized and
  outside any penalty area. They move at the speed indicated by the table below.
- The ball is placed relatively close to the robots, ensuring that at
  the collision time, the distance to the robots is approximately `D` as
  provided below.
  
| Variation | R1 speed [m/s] | R2 speed [m/s] | D [m] |
|-----------|----------------|----------------|-------|
| A         |           0.05 |            0.1 |   1.5 |
| B         |            0.1 |            0.4 |   1.5 |
| C         |           0.25 |           0.15 |   1.5 |
| D         |           0.25 |            0.5 |   0.8 |

## Expected outcome

- A. No freekick, no penalties
- B. R2 is penalized, R1 is not penalized
- C. R1 is penalized, R2 is not penalized
- D. Freekick occurs, none of the robot is penalized
