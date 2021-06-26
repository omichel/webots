# Forceful contact - far from ball

This situation include 4 variations: A,B,C,D. When they differ, it is explicitly
specified.

A. Low-speed far from ball -> no penalty
B. High-speed far from ball, similar speed -> no penalty
C. Both robot move at high speed, R1 significantly faster than R2 -> penalty on R1
D. R1 moves slowly, R2 moves at high speed-> penalty on R2

## Setup:

- GameState is READY and ball is in play
- Robots R1 and R2 are nearby, they are of opposite team, not penalized and
  outside any penalty area.
- The ball is placed far from the robots (at least 2 meters of any position they
  take during the scenario)
  
## Procedure:

- Both robots enter in collision:
  - A. While R1 is moving at 0.15 m/s and R2 is moving at 0.05 m/s
  - B. While R1 is moving at 0.4 m/s and R2 is moving at 0.25 m/s
  - C. While R1 is moving at 0.5 m/s and R2 is moving at 0.25 m/s
  - D. While R1 is moving at 0.05 m/s and R2 is moving at 0.3 m/s

## Expected outcome

- A+B. No freekick, no penalties
- C. R1 is penalized but not R2
- D. R2 is penalized but not R1
