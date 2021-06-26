On this test scenario, we have some robots initially placed outside of the field in valid positions. Once, a valid goal is going to be scored, another time, the ball will not completely cross the line. The AutoRef's actions against these situations is going to be tested:

The following should happen:

1. The robots are spawned and the game state is `INITIAL`.
2. Time elapses and game state changes to `READY`, then `SET`, and then `PLAY`.
3. After changing the state to `PLAY`:
   - Simulation is paused. Then, robot `Red 1` is manually moved to near the opponent's goal, exactly to:\
   `4 0.00 0.24` (if the robot is on the left side)\
   `-4 0.00 0.24` (if the robot is on the right side)
   - Then, the ball is manually moved to near the opponent's goal, in front of `Red 1`, exactly to:\
   `4.25 0.00 0.08` (if team red is on the left side)\
   `-4.25 0.00 0.08` (if team red is on the right side)
   - A force of 5 Newton is applied to the ball along positive or negative X-axis according to robot's side (from the ball towards the robot), simulation is resumed.
   - The ball bounces on `Red 1`.
4. - Simulation is paused.
   - Then, the ball is manually moved to near the opponent's goal, in front of `Red 1`, exactly to:\
   `4.25 0.00 0.08` (if team red is on the left side)\
   `-4.25 0.00 0.08` (if team red is on the right side)
   - A force of 3.5 Newton is applied to the ball along positive or negative X-axis according to robot's side (from the ball towards the goal), simulation is resumed.
   - Ball doesn't entirely cross the goal line and doesn't completely enter the goal.
   - No goal is scored as the ball doesn't entirely passes the goal line. ([#8](https://github.com/RoboCup-Humanoid-TC/webots/issues/8))
5. - Simulation is paused.
   - Then, the ball is manually moved to near the opponent's goal, in front of `Red 1`, exactly to:\
   `4.25 0.00 0.08` (if team red is on the left side)\
   `-4.25 0.00 0.08` (if team red is on the right side)
   - A force of 5 Newton is applied to the ball along positive or negative X-axis according to robot's side (from the ball towards the goal), simulation is resumed.
   - Ball entirely crosses the goal line and enters the goal.
   - Then, a goal is scored for team red. ([#7](https://github.com/RoboCup-Humanoid-TC/webots/issues/7))

The following information should be contained in logs (among others):

```
[SSSS.xxx|SSSS.xxx] Info: Ball touched by red player 1.
[SSSS.xxx|SSSS.xxx] Info: Ball touched by red player 1.
[SSSS.xxx|SSSS.xxx] Info: Ball left the field at -4.58 0 0.08 after being touched by red player 1.
[SSSS.xxx|SSSS.xxx] Info: Kickoff is blue
[SSSS.xxx|SSSS.xxx] Info: Score in blue goal by red player 1
[SSSS.xxx|SSSS.xxx] Info: Ball respawned at 0 0 0.08
[SSSS.xxx|SSSS.xxx] Info: New state received from GameController: STATE_PLAYING
```
