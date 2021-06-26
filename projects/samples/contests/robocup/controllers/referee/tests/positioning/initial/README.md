On this test scenario, we have multiple valid and invalid positions for the
robots of both team:

- Team 1 (RED): 2 robots with invalid position
  - 1: entirely on the field
  - 2: partially on the border line
- Team 2 (BLUE): 2 robots have an invalid position
  - 2: partially on the back line
  - 4: partially on the wrong side

The following should happen:

1. All robots are spawned on their appropriate side, according to the team
   configuration files (except `Blue 4` who is on the wrong side).
2. While game state is `INITIAL` all robots stay static.
3. AutoRef sends a message to the GameController, status is changed to `READY`
4. Penalties are being called for illegaly positioned robots who are moved to
   appropriate locations.
5. Simulation is paused, `Red 1`, `Red 3` and `Blue 1` are manually moved inside
   the field, simulation is resumed.
6. Removal penalty and a yellow card is applied to `Red 1` (illegal entry).
7. Time elapses and game state changes to `SET`.
8. Robots `Red 4` and `Blue 3` are removed from the field by the auto-referee.
9. Time elapses and game state changes to `PLAY`.
10. Simulation is paused, `Red 1`, `Blue 2` and `Blue 3` are manually moved
    inside the field, simulation is resumed.
11. No robot is penalized.

The following information should be contained in logs (among others):

```
[SSSS.xxx|0000.000] Info: Spawned RED_PLAYER_1 RobocupRobot on port 10001 at halfTimeStartingPose: translation 4.0 -2.06 0.24, rotation 0 0 1 1.57
[SSSS.xxx|0000.000] Info: Spawned RED_PLAYER_2 RobocupRobot on port 10002 at halfTimeStartingPose: translation 4.0 3.02 0.24, rotation 0 0 1 4.71
[SSSS.xxx|0000.000] Info: Spawned RED_PLAYER_3 RobocupRobot on port 10003 at halfTimeStartingPose: translation 0.75 -3.06 0.24, rotation 0 0 1 1.57
[SSSS.xxx|0000.000] Info: Spawned RED_PLAYER_4 RobocupRobot on port 10004 at halfTimeStartingPose: translation 0.75 3.06 0.24, rotation 0 0 1 4.71
[SSSS.xxx|0000.000] Info: Spawned BLUE_PLAYER_1 RobocupRobot on port 10021 at halfTimeStartingPose: translation -4.7 -2.06 0.24, rotation 0 0 1 0
[SSSS.xxx|0000.000] Info: Spawned BLUE_PLAYER_2 RobocupRobot on port 10022 at halfTimeStartingPose: translation -4.55 2.06 0.24, rotation 0 0 1 0
[SSSS.xxx|0000.000] Info: Spawned BLUE_PLAYER_3 RobocupRobot on port 10023 at halfTimeStartingPose: translation -0.75 -3.06 0.24, rotation 0 0 1 1.57
[SSSS.xxx|0000.000] Info: Spawned BLUE_PLAYER_4 RobocupRobot on port 10024 at halfTimeStartingPose: translation 1.75 3.06 0.24, rotation 0 0 1 -1.57
[SSSS.xxx|SSSS.xxx] Info: Sending STATE:READY to GameController
[SSSS.xxx|SSSS.xxx] INCAPABLE penalty for red player 1: halfTimeStartingPose inside field. sent to reentryStartingPose: translation x.xx x.xx x.xx orientation x.xx x.xx x.xx
[SSSS.xxx|SSSS.xxx] INCAPABLE penalty for red player 2: halfTimeStartingPose inside field. sent to reentryStartingPose: translation x.xx x.xx x.xx orientation x.xx x.xx x.xx
[SSSS.xxx|SSSS.xxx] INCAPABLE penalty for blue player 2: halfTimeStartingPose inside field. sent to reentryStartingPose: translation x.xx x.xx x.xx orientation x.xx x.xx x.xx
[SSSS.xxx|SSSS.xxx] INCAPABLE penalty for blue player 4: halfTimeStartingPose outside team side. sent to reentryStartingPose: translation x.xx x.xx x.xx orientation x.xx x.xx x.xx
[SSSS.xxx|SSSS.xxx] INCAPABLE penalty for red player 1: illegal field entry. sent to reentryStartingPose: translation x.xx x.xx x.xx orientation x.xx x.xx x.xx
[SSSS.xxx|SSSS.xxx] Yellow card shown to red player 1
[SSSS.xxx|SSSS.xxx] Setting state to SET
[SSSS.xxx|SSSS.xxx] INCAPABLE penalty for red player 4: illegal set position. sent to reentryStartingPose: translation x.xx x.xx x.xx orientation x.xx x.xx x.xx
[SSSS.xxx|SSSS.xxx] INCAPABLE penalty for blue player 3: illegal set position. sent to reentryStartingPose: translation x.xx x.xx x.xx orientation x.xx x.xx x.xx
[SSSS.xxx|SSSS.xxx] Setting state to PLAY
```
