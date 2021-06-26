# Game phases : penalty shootouts penalized
## What is tested

- If a goalkeeper is penalized at the end of a trial, he can still be a kicker
  on the next one
- If a kicker is penalized at the end of a trial, it can still act as a goalkeeper for the next trial
- If robot 1 receives a red card, robot 2 acts as a kicker

## Setup

- Game is expected to start with penalty shootouts
- Team BLUE starts taking the trials
- Penalties occurs on the left side of the field

## Description

1. Game starts with state INITIAL
2. The state automatically changes to SET
3. Team take their trials one after the other

### Trial 1: Penalty for BLUE

- 25 seconds after the start of the trial BLUE 1 falls, it receives a penalty 20 seconds after
- 40 seconds after the start of the trial, Goalkeeper RED moves and receives a penalty
- At the end of the trial, penalties are removed and game continues

### Trial 2: Penalty for RED

- After 15 seconds of PLAYING, Goalkeeper BLUE moves back from the goal line
- BLUE 1 receives a penalty
- BLUE 1 tries to enter repeatedly and receives a red card
- The trial ends without scores for red

### Trial 3: Penalty for BLUE

- BLUE 2 starts as kicker (1 has a red card)
