# Knock-out game: penalty shootouts win

This test has to be performed manually for now.

## What is tested

- If after each team played 5 trials during the penalty shootouts, both teams
  scored the same number of goals, the sudden death penalty phase starts.
- During the sudden death phase:
  - If both teams score, another round is started
  - If both teams fail to score, another round is started
  - If only one of the team scores, the referee announces that it won the game
    and ends the simulation.

## Description

When the description mentions that "Game time passes", it is possible to use
the GameController buttons to speed-up the process.

1. Team BLUE scores a goal during first half.
2. Team RED scores a goal during second half.
3. Game time passes until the end of the second half.
4. First extended period is automatically started by the referee.
5. Game time passes until the end of the second extended period.
6. The Autoreferee automatically switches to penalty shootouts.
7. RED team fails their 3 first trials but scores the 2 following,
   BLUE team scores the 2 first trials but fails the three following.
8. The game continues with sudden death
9. ROUND 1: Both teams fail to score.
10. ROUND 2: Both teams score.
11. ROUND 3: Team BLUE scores and team RED fails to score.
12. The Autoreferee announces that team RED won the game.
13. The simulation closes automatically afterwards.
