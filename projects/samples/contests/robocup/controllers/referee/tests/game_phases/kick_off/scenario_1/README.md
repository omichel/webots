# Kick-off - scenario 1

## What is tested

- Ball in play at kick-off
  - Not touched, elapsed less than 10 seconds -> not in play
  - Not touched, elapsed more than 10 seconds -> in play
- Valid position for robots when PLAYING before ball is in play
  - Team who has not kick off can't enter center circle, the rest is authorized
- Scoring a goal
  - If a second player touches the ball: goal is valid
  
## Setup

- Team Red has Kick-off
- All robots are placed according to the rules and RED 1 is inside the center circle

## Description

1. RED 2 and BLUE 2 enter the center-circle immediately, RED 3 and BLUE 3
   moves to their opponent field.
2. Only BLUE 2 receives a removal penalty, all other moves are legal.
3. 10 seconds elapses.
4. BLUE 3 enters the center circle, it does not receive a penalty.
5. RED 1 kicks the ball, which moves of around 20 centimeters
6. RED 2 kicks the ball again, to the opponent goal, goal is scored.
