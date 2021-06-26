# Knock-out game: penalty shootouts win

This test has to be performed manually for now.

## What is tested

- If after the 2 extended periods the team have scored the same number of goals,
  the game continues with penalty shootouts.
- Once it's not possible for a team to win the game anymore during the penalty
  shootouts phase, the referee announces that its opponent won the game and ends
  the simulation.

## Description

When the description mentions that "Game time passes", it is possible to use
the GameController buttons to speed-up the process.

1. Team BLUE scores a goal during first half.
2. Team RED scores a goal during second half.
3. Game time passes until the end of the second half.
4. First extended period is automatically started by the referee.
5. Game time passes until the end of the second extended period.
6. The Autoreferee automatically switches to penalty shootouts.
7. RED teams score a goal during the first three trials, while BLUE team miss
   the first three trials.
8. The Autoreferee announces that team RED won the game.
9. The simulation closes automatically afterwards.
