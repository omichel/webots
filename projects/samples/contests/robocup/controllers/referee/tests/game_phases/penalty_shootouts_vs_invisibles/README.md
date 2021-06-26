# Game phases : penalty shootouts vs Invisibles

## What is tested

- When penalty shootouts are started between one team that has players and one
  team that has no players, the team that has player is performing the trials as
  usual
- The trials of the team that has no players are skipped

## Setup

- Game is expected to start with penalty shootouts
- Team BLUE starts taking the trials
- Team RED has no players
- Penalties occurs on the left side of the field

## Description

1. Game starts with state INITIAL
2. The state automatically changes to SET
3. Team take their trials one after the other

### Trial 1: Penalty for BLUE

- BLUE 1 kicks the ball into a goal
- Goal is scored for BLUE and game continues with Trial 2

### Trial 2: Penalty for RED

- Game moves directly from SET to FINISHED
- Game continues with Trial 3

### Trial 3: Penalty for BLUE

- BLUE 1 does not touch the ball
- After 60 seconds Game continues with Trial 4

### Trial 4: Penalty for RED

- Game moves directly from SET to finish
- Game continues with Trial 5

### Trial 5: Penalty for BLUE

- Scenario ends here
