# Knock-out game: extended time win

This test has to be performed manually for now.

## What is tested

- If at the end of the regular time of a knockout game both no goal has been
  scored, game goes on with extended time.
- If a team scores during the first extended period, the game still proceeds
  with a second extended period.
- If one team scored more goals than the other after the 2 extended periods, the
  referee announces that it won the game and ends the simulation.

## Description

When the description mentions that "Game time passes", it is possible to use
the GameController buttons to speed-up the process.

1. Game time passes until the end of the second half.
2. First extended period is automatically started by the referee.
3. Team RED scores a goal during first extended period.
4. Game time passes until the end of the first extended period.
5. The second extended period is automatically started.
6. Team BLUE scores two goals in a row.
7. Game time passes until the end of the second extended period.
8. At the end of second extended period, AutoRef declares team red winner of the match.
9. The simulation closes automatically afterwards.
