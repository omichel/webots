# Game Interruption - corner procedure

## What is tested

- When ball fully crosses the goal line, last touched by a robot of the
  defending team, a corner kick is awarded to the attacking team.
- For the corner kick, the ball is placed at the appropriate location after
  phase 0
- Time spent in phase 0 is 10 seconds
- Time spent in phase 1 is 30 seconds
- Time spent in phase 2 is low
- Robots of the team that is awarded a corner are allowed nearby
- Opponents closer than 75cm of the ball are penalized after phase 1
- Opponents entering the area before 10 seconds have elapsed are penalized 1

## Setup

- Team RED has Kick-off and is on left side
- RED 1 and 3 are playing, BLUE 1

## Description

1. RED 1 touches the ball after PLAYING, then the ball moves outside the field
   from its own goal line.
2. A corner is awarded to team BLUE
3. During phase 0, RED 1 moves near the ball
4. During phase 1, BLUE 1 moves near the ball, RED 3 stays 1m away from the ball
5. When corner is executed, RED 1 gets penalized
6. RED 3 moves closer to the ball before 10 seconds elapse, it receives a
   penalty

