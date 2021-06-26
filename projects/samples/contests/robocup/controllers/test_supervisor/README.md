# Test Supervisor

This document presents the test supervisor that allows to run automatically
a scenario, based on a `JSON` file provide through the environment variable
`WEBOTS_ROBOCUP_TEST_SCENARIO`. Those tests can be launched through the
[launch_test.sh](../referee/tests/launch_test.sh) script.

Each `Event` inside a `Scenario` is composed of the following elements:

- A `TimeSpecification` that defines when an event should be activated and when
  it should be considered as finished. It is present in two varieties:
  - `TimePoint` for events that should be triggered only once
  - `TimeInterval` for events that should be triggered at every timestep during
    a given interval.
- A variable number of tests of type `Test`, a class can contain verification
  over different elements of the game such as:
  - The position of an object.
  - The penalty status of a robot.
  - The current game state received by the GameController
- A variable number of actions of type `Action`, a class allowing to modify the
  current state of a game, e.g. moving an object or applying a force to it.

All of this classes can be build directly from a dictonary or a list resulting
of parsing the `JSON` file of the scenario.

To fulfill their roles, the previously mentioned classes rely on:

- A `supervisor` which is started by [referee.py](../referee/referee.py).
- A simple UDP socket to listen to the Game Controller messages, `GCListener`.
- A `StatusInformation` object which stores the latest information and a minimal
  history to provide history based information such as: how many seconds have
  elapsed since we entered the `READY` state.

## Guidelines when writing a scenario

Try to aim for deterministic behaviors:
- Always set up kickoff and side in the `game.json` file.
- Take some time margin between applying actions and testing their effect.

For time specifications:
- Try to stick with one type of clock per scenario (easier to understand)

Reduce the time required to compute your tests:
- If possible set `real_time_factor=0` in `game.json`
