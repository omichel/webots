# Kick-off - scenario 2

## What is tested

- Ball in play at kick-off
  - Moved less than 5cm, elapsed less than 10 seconds -> not in play
  - Moved more than 5cm, elapsed less than 10 seconds -> in play
- Valid position for robots when PLAYING before ball is in play
  - Team who has not kick off can't enter center circle
- Scoring a goal
  - If the same player touches twice the ball inside the center circle: goal kick

## Setup

- Team BLUE has Kick-off
- All robots are placed according to the rules and BLUE 2 is inside the center circle

## Description

1. BLUE 2 touches the ball just after PLAYING is called, ball moves only 2cm (t=2)
2. RED 2 enters the center circle immediately after the ball moved, it
   receives a penalty. (t=4)
3. BLUE 2 kicks the ball again it moves 10 cm forward (t=6)
4. RED 3 enters the center circle, it is not penalized (t=8)
5. BLUE 2 kicks the ball again into RED goal, goal kick is awarded to BLUE
