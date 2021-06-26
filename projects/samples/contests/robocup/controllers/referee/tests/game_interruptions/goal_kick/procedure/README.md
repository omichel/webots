# Ball-out - Goal kick

## What is tested

- When ball fully crosses the goal line, last touched by a robot of the
  attacking team, a goal kick is awarded to the defending team.
- For the goal kick, the ball is placed at the appropriate location after
  phase 0
- Time spent in phase 0 is 10 seconds
- Time spent in phase 1 is 30 seconds
- Time spent in phase 2 is low
- Robots of the team that is awarded a goal kick are allowed nearby
- If an opponent touches the ball during phase 1:
  - The robot receives a warning
  - The goal kick is retaken (restart at phase 0)
- Opponents are allowed to move closer to the area once the ball is in play

## Setup

- Team RED has Kick-off and is on left side
- RED 1 and BLUE 1 are playing

## Description

1. RED 1 touches the ball after PLAYING, then the ball moves outside the field
   from the opponent's goal line.
2. The ball is replaced on the touchline at the intersection with the centerline
   on the side the ball left the field.
3. A goal kick is awarded to team BLUE.
4. During phase 0, nothing happens.
5. During phase 1, RED 1 touches the ball. It receives a warning but not a
   removal penalty.
6. Goal kick is retaken, starting at phase 0 again.
7. During the second phase 1, BLUE 1 moves near the ball and RED 1 stays far
   enough.
8. Right after 'Execute goal kick' is called, BLUE 1 kicks the ball for more
   than 5 centimeters.
9. Before 10 seconds elapse, RED 1 moves near the ball, it does not receive a
   penalty because ball is in play.
