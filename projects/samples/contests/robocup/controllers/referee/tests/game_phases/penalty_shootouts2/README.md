# Game phases : penalty shootouts 2

## What is tested

- If 'kick_off' is set to blue, then BLUE team starts performing the penalty

*This test is a mirror of the beginning of penalty shootouts*

## Setup

- Game is expected to start with penalty shootouts
- Team BLUE starts taking the trials
- Penalties occurs on the left side of the field

## Description

1. Game starts with state INITIAL
2. The state automatically changes to SET
3. Team take their trials one after the other

### Trial 1: Penalty for BLUE

- Goalkeeper RED moves during SET phase
- RED 1 receives a penalty
- BLUE 1 kicks the ball into a goal
- Goal is scored for BLUE and game continues with Trial 2

### Trial 2: Penalty for RED

- After 15 seconds of PLAYING, Goalkeeper BLUE moves back from the goal line
- BLUE 1 receives a penalty
- RED 1 kicks the ball that leaves the field on the side
- The trial ends without scores for red
