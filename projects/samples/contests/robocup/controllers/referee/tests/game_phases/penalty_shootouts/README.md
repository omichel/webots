# Game phases : penalty shootouts

## What is tested

- Kicker and goalkeeper are placed according to the configuration file (T1-3)
- States transition occurs automatically (T1-4)
- If goalkeeper moves away from his line:
  - During state 'SET', it receives a penalty. (T1)
  - Before ball was kicked 5cm forward by the kicker, it receives a penalty. (T2)
  - After the ball was kicked, it's not penalized. (T3)
- End of the trial is properly detected:
  - If a goal is scored (T1)
  - If the ball leaves the field (T2)
  - If the ball stops while **entirely** in the goal area (T3)
  - If 60 seconds have elapsed and the ball is not rolling anymore (T4)
- The kicker can touch the ball twice (or more) as long as its outside of goal
  area (T3)
## Setup

- Game is expected to start with penalty shootouts
- Team RED starts taking the penalties
- Penalties occurs on the left side of the field

## Description

1. Game starts with state INITIAL
2. The state automatically changes to SET
3. Team take their trials one after the other

### Trial 1: Penalty for RED

- Goalkeeper BLUE moves during SET phase
- BLUE 1 receives a penalty
- RED 1 kicks the ball into a goal
- Goal is scored for RED and game continues with Trial 2

### Trial 2: Penalty for BLUE

- After 15 seconds of PLAYING, Goalkeeper RED moves back from the goal line
- RED 1 receives a penalty
- BLUE 1 kicks the ball that leaves the field on the side
- The trial ends without scores for blue

### Trial 3: Penalty for RED

- RED 1 kicks the ball a first time, ball moves toward the goal and stops near
  the goal area
- BLUE 1 moves towards the ball, it does not receive a penalty
- RED 1 kicks the ball again
- The ball stops rolling
- The trial ends without a score for blue

### Trial 4: Penalty for Blue

- Both robots don't move at all
- After 60 seconds, the trial is considered as failed and ends
