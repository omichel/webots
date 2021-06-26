# Robocup Virtual Humanoid League 2021

This folder contains the simulation setup for the Robocup Virtual Humanoid League 2021.

## Requirements

In order to run this simulation, you will need to have a [fairly powerful](https://cyberbotics.com/doc/guide/system-requirements) Linux, Windows or macOS computer.
You will also need to get familiar with Webots by reading the [Webots User Guide](https://cyberbotics.com/doc/guide/) and following the [Tutorials](https://cyberbotics.com/doc/guide/tutorials).

## Installation

1. Build Webots from the source from the release branch on the official RoboCup Humanoid TC fork: https://github.com/RoboCup-Humanoid-TC/webots

   ```
   git clone --recurse-submodules --branch release https://github.com/RoboCup-Humanoid-TC/webots
   cd webots
   ./scripts/install/linux_compilation_dependencies.sh
   make -j
   echo WEBOTS_HOME=/path/to/webots >> ~/.bashrc
   ```

   If you already cloned the repo, update Webots like this:

   ```
   git pull --recurse-submodules
   ./scripts/install/linux_compilation_dependencies.sh
   git clean -xfd
   make clean

   # make sure that you have no modified files before continuing
   git status

   make -j
   ```

   Also see the Webots wiki for more information: https://github.com/cyberbotics/webots/wiki

2. Build the latest version of the official RoboCup Humanoid TC fork of the [GameController](https://github.com/RoboCup-Humanoid-TC/GameController).
   ```
   apt-get install ant
   git clone https://github.com/RoboCup-Humanoid-TC/GameController
   cd GameController
   ant
   ```
3. Install Python dependencies:
   ```
   cd webots/projects/samples/contests/robocup/controllers/referee
   pip3 install -r requirements.txt
   ```
4. Build the controllers:
   ```
   apt-get install protobuf-compiler libprotobuf-dev libjpeg9-dev
   cd webots/projects/samples/contests/robocup
   make # This requires the environment variable WEBOTS_HOME to be set
   ```

## Run the Demo

1. Open the [robocup.wbt](worlds/robocup.wbt) world file in Webots and run it until you see the GameController window showing up.
   ```
   export GAME_CONTROLLER_HOME=/path/to/GameController JAVA_HOME=/usr
   ./webots ./projects/samples/contests/robocup/worlds/robocup.wbt
   ```
   You have to pass the environment variables `GAME_CONTROLLER_HOME` which points to the `GameController` folder and `JAVA_HOME` which points to your Java installation (which might be under `/usr`).
2. You can manually move the robots and the ball using the mouse (<kbd>Shift</kbd>-right-click-and-drag).
3. Launch the sample robot controller [client.cpp](controllers/player/client.cpp) by typing `./client` in the [controllers/player](controllers/player) folder.
4. The sample client program will simply move the neck of one of the Darwin-OP robot.

## Modify the Game and Teams Configuration

1. Quit Webots.
2. Edit the [game.json](controllers/referee/game.json) file to change the game configuration. The ports in this file are where the API server will open ports and the API servers will only accept traffic from the whitelisted IP, i.e. you might want to change the IPs to 127.0.0.1 for a local setup.
3. Edit the [team_1.json](controllers/referee/team_1.json) and [team_2.json](controllers/referee/team_2.json) files to change the teams configuration.
4. Restart the simulation.

## Program your Own Robot Controllers

1. Update the [game.json](controllers/referee/game.json) configuration file and create your own team configuration files, taking inspiration from [team_1.json](controllers/referee/team_1.json) and [team_2.json](controllers/referee/team_2.json).
2. Create your own robot controllers, taking inspiration from the sample [client.cpp](controllers/player/client.cpp).

## Create your Own Robot Model

Create your robot model in the [protos](protos) folder taking inspiration from [RobocupRobot.proto](protos/RobocupRobot.proto) and adjust your team configuration file accordingly.

### Exposed Parameters

Your proto should have the following exposed parameters:

```js
PROTO MyOwnRobocupRobot [
  field SFVec3f                 translation    0 0 0   # Is `Transform.translation`.
  field SFRotation              rotation       0 0 1 0 # Is `Transform.rotation`.
  field SFString                name           ""      # Is `Solid.name`.
  field MFString                controllerArgs []      # Is `Robot.controllerArgs`.
]
{
  Robot {
    translation IS translation
    rotation IS rotation
    name IS name
    controllerArgs IS controllerArgs
    ⋮
  }
}
```

### Creating a Jersey with the Correct Color and Player Number

The team color and player number of each robot is set by the referee in the `name` field of the robot when spawning it.
It can takes values like "red player 1" or "blue player 3", etc.
You should therefore parse this `name` field to determine the team color and player number of your robot:

```lua
  if fields.name.value ~= '' then
    -- name is supposed to be something like "red player 2" or "blue player 1"
    local words = {}
    for word in fields.name.value:gmatch("%w+") do table.insert(words, word) end
    local color = words[1]
    local number = words[3]
```

Then, the `color` and `number` variables should be used by your PROTO file to display the requested color and player number on the jersey of the robot.
This can be achieved by forging a texture name from these variables or using them directly to assign material colors, create shapes, etc.
More information about Webots PROTO is available from the [Webots Reference Manual](https://cyberbotics.com/doc/reference/proto).

### Add Body Part Tags

The referee needs to identify the different parts of your robot to apply the rules of the game.
These parts include the foot, arms, legs, head, etc.
In order to identify each part, you should set the `model` field of each [Solid](https://cyberbotics.com/doc/reference/solid) (or derived) node composing your robot with one of the following strings: `"foot"`, `"arm"`, `"shoulder"`, `"hip"`.

## Continuous Integration Tests

By default, the referee supervisor will read the [game.json](controllers/referee/game.json) file from its local folder.
However, if a special environment variable is set, it will use it to load a different `game.json` file:

```
export WEBOTS_ROBOCUP_GAME="/path/to/my/special/game.json"
```

In order to facilitate Continuous Integration (CI) tests, a number of options are available in the [game.json](controllers/referee/game.json) file:

```json
{
  "side_left": "red",
  "kickoff": "blue",
  "supervisor": "MyTestSupervisor"
}
```

- "left_side" is used to determine which team will play on the left side of the field (x < 0). It can take the following values: "red", "blue", "random". The default value is "random".
- "kickoff" is used to determine which team has the initial kickoff. It can take the same values as the "left_side" field.
- "supervisor" is used to define an optional extra supervisor controller process that will run in parallel with the referee.
This extra supervisor controller will be spawned by the referee at the beginning of the game.
The following new node with be added to the scene after the referee robot: `Robot { supervisor TRUE controller "MyTestSupervisor" }`.

## Model verifier

A semi-automated tool allowing to check if a robot respects the rules is available in
the `controllers/model_verifier` directory.
The available scripts are documented in a dedicated [README](controllers/model_verifier/README.md).
