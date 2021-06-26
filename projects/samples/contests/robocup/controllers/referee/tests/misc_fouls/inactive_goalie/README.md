# Inactive goalie

There are different parts in this scenario:

## What is tested

- Field players are allowed to stay near the ball inside their goal area.
  (Part B)
- A goalie is allowed to stay near the ball as long as it wants if the goalie is
  located outside its own goal area:
  - Even if the goalie is in its own penalty area and the ball in the goal area.
    (Part B)
  - Even if the goalie is inside the opponent goal area. (Part A)
- A goalie that does not move is penalized -> penalty (Part A,C,D)
- A goalie that reduces the distance toward the ball is not penalized (Part C)
- A goalie that touched the ball recently is not penalized (Part D)
- A goalie that moves away enough from the ball (more than 50cm) is not penalized (Part E)
- A goalie that moves slightly away from the ball (less than 50cm) is penalized (Part F)
- For any infringement to this rule, game continues and the only impact is
  removal penalty.

Note: for all those tests, goalie are always considered as the player with id 1.
Therefore, those tests do NOT test situations where teams change the id of the
goalkeeper. Doing so in an automated way would imply too much complexity on the
testing side.

## Setup and description

This scenario involves three robots:

- RED 1: goalkeeper
- RED 2: field player
- BLUE 1: goalkeeper


### A. Static situation 1

- Setup:
  - Ball is in the penalty area of BLUE team, close to the goal area
  - RED 1 is closer than 50 cm of the ball, inside goal area
  - BLUE 1 is closer than 50 cm of the ball, inside goal area
  - RED 2 is far away
- Expectations:
  - During 20 seconds, none of the robot is penalized
  - After 20 seconds, BLUE 1 is penalized, but not RED 1

### B. Static situation 2

- Setup:
  - Ball is in the goal area of RED team
  - RED 1 is closer than 50 cm of the ball, outside goal area
  - RED 2 is in goal area, closer than 50cm of the ball, located in a way that
    ensures it is not performing ball holding with RED 1
  - BLUE 1 is far away
- Expectations:
  - During 20 seconds, none of the robot is penalized
  - After 20 seconds, none of the robot is penalized

### C. Dynamic situation 1

- Setup:
  - Ball is in the goal area of RED team
  - RED 1 is at 40 cm of the ball, inside goal area
  - BLUE 1 and RED 2 are far away
- Expectations:
  - 10 seconds elapse, none of the robot is penalized
  - RED 1 reduces the distance to the ball to 30cm by moving
  - During the next 20 seconds, RED 1 is not penalized

### D. Dynamic situation 2

- Setup:
  - Ball is in the goal area of RED team
  - RED 1 is at 20 cm of the ball, inside goal area
  - BLUE 1 and RED 2 are far away
- Expectations:
  - 10 seconds elapse, none of the robot is penalized
  - RED 1 moves to the ball and kicks 40cm away
  - During the next 20 seconds, RED 1 is not penalized

### E. Dynamic situation 3

- Setup:
  - Ball is in the goal area of BLUE team
  - BLUE 1 is at 30 cm of the ball, inside goal area
  - RED 1 and RED 2 are far away
- Expectations:
  - 10 seconds elapse, none of the robot is penalized
  - BLUE 1 increases its distance to the ball to 70cm by moving away
  - During the next 30 seconds, BLUE 1 is not penalized (final position is valid)

### F. Dynamic situation 4

- Setup:
  - Ball is in the goal area of BLUE team
  - BLUE 1 is at 20 cm of the ball, inside goal area
  - RED 1 and RED 2 are far away
- Expectations:
  - 10 seconds elapse, none of the robot is penalized
  - BLUE 1 increases its distance to the ball to 40cm by moving away
  - During the next 10 seconds, BLUE 1 is not penalized
  - After those 10 seconds, BLUE 1 receives a penalty
