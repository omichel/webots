On this test scenario, we simply test that the real time elapsed between the
initialization of the simulator and the `READY` phase is two minutes.

The following should happen:

1. All robots are spawned
2. During 2 minutes (real-time not simulated time), game state is `INITIAL` all
   robots stay static.
3. AutoRef sends a message to the GameController, status is changed to `READY`

The following information should be contained in logs (among others):

```
[SSSS.xxx|0000.000] Info: Spawned RED_PLAYER_1 RobocupRobot on port 10001 at halfTimeStartingPose: translation 4.0 -2.06 0.24, rotation 0 0 1 1.57
[SSSS.xxx|0000.000] Info: Spawned BLUE_PLAYER_1 RobocupRobot on port 10021 at halfTimeStartingPose: translation -4.7 -2.06 0.24, rotation 0 0 1 0
[SSSS.xxx|SSSS.xxx] Info: Sending STATE:READY to GameController
```
