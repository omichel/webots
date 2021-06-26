# Forceful contact - competing for the ball

## Introduction

This situation include 4 variations: A,B,C. When they differ, it is explicitly
specified.

A. Both players move toward the ball, distance to ball is similar (diff < 0.1m) -> no foul
B. Both players move toward the ball, R1 is signicantly closer -> R2 commits a foul
C. R1 is static, closer to the ball, R2 move towards the ball from behind and
   collide -> R2 commits a foul

## Setup:

- GameState is READY and ball is in play
- Robots R1 and R2 are nearby, they are of opposite team, not penalized and
  outside any penalty area. They move approximately at the speed indicated by
  the table below.
- The ball is placed relatively close to the robots, ensuring that at
  the collision time, the distance to the robots is approximately `Dist` as
  provided below.
  
| Variation | R1 speed [m/s] | R2 speed [m/s] | Dist [m] |
|-----------|----------------|----------------|----------|
| A         |            0.3 |            0.3 |      1.5 |
| B         |            0.4 |            0.4 |      1.5 |
| C         |            0.0 |            0.5 |      1.5 |

## Expected outcome

- A. No freekick, no penalties
- B. R2 is penalized, R1 is not penalized
- C. R2 is penalized, R1 is not penalized

## Notes:

While 'A' is passing, it is very sensitive to initial position because after the
first tick of collision between the robots, the trajectories of the robot can
change
