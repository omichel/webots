# Model Verifier

This folder contains the `model_verifier.py` controller which spawns automatically a robot and automatically searches
for infringements to the rules or suspicious elements.

Keep in mind that this controller only checks **part of the rules**, therefore it's possible that a robot is invalid
although it did not trigger errors nor warning during the analysis.

## Dependencies

Additionally to `Webots`, you will need the python modules listed in `requirements.txt`.

Generating a `pdf` report requires the `pandoc` tool:

`sudo apt install pandoc`

## Robot inspection

To inspect a robot, you should make sure the following conditions are met:

- The `PROTO` file of the robot is located in the `robocup/protos` folder or one of its subfolders
- A file named `postures.json` which contains the `upright` and the `extension` poses is located in the same
  folder as the `PROTO` file

You can then run a model check using the following command:

`./model_checker.sh <robot_name.proto>`

This will produce the following result:

1. Start a `Webots` simulation with an empty world
2. Spawn the requested robot
3. Open 2 windows allowing you to visualize the bounding objects, the posture of the robot and the solids annotations:
    - The first window will contain the `upright` posture, the second the `extension` posture
    - The following color code is used:
        - **Brown:** solids labeled as `[arm]`
        - **Green:** solids labeled as `[hand]`
        - **Black:** solids labeled as `[foot]`
        - **Grey:** unlabelled solids
    - Once you close a window, the verification process will continue
    - It is possible to skip this step using the option `--no-display`
4. The simulation will close automatically and the results will be available in the folder `results/robot_name`:
    - `model_verifier.log` contains the messages from the `model_verifier` controller
    - `simulator.log` contains the standard output of the siimulator
    - `simulator.err` contains simulator errors and warning raised when loading the model
    - `robot_properties.json` A file that can be used to import robot dimensions on the
      [submission website](https://submission.robocuphumanoid.com/inspection/leagues)
    - `report.pdf` contains an overview of all the results including:
        - Warnings and Errors during model_verification.
        - Table with all the joints and their values
        - Table with the main properties of the robot
        - Logs of the model verification
    - `report.md` the source file for `report.pdf`

## Archive inspection

To inspect the validity of an archive before uploading it on
https://submission.robocuphumanoid.com you can run the following command:

`./archive_checker.sh <archive.zip>`

It will check some properties of the archive, extract its content in the `robocup/protos/tmp` folder and then perform
an automated inspection with the flag `--no-display`.

## Run mock game

This script is used to detect low performances for the robot and currently requires to perform some manual step.

- The game file `game.json` in the `referee` controller has to be configured as follows:
  - "minimum_real_time_factor" must be set to 1
  - "max_duration" must be set to 120

This script takes as argument the path of a folder `team_name` which has to be organized in the following way:

```
team_name
|- team.json
|- robot_name
|  |- posture.json
|  |- robot_name.proto
|  |- ...
```

Game can then be launched with `,/run_mock_game.sh team_name` or `./run_mock_game.sh --no-rendering team_name`:

This will start a game with a full team red and a full team blue, both of them will have clients
connecting after a little while in order to ensure robots are not in asleep mode.

The output of the clients will be stored in different files in folder `team_name`
(e.g. `RED_1_with_rendering.txt`). At the end of the test, robot is closed and the
last output from the clients are shown on screen. 

The value obtained can be used to estimate the speed factor associated to the team.
