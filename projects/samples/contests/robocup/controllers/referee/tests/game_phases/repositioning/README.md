# Game phases: Repositioning after scoring a goal

This test is very similar to the test associated for positioning after the READY
phase with the main following exception: the procedure starts after a goal and
not from INITIAL.

## What is tested

- Kick-off team is properly attributed after the goal.
- Robot who are fully or partially on the opponent field at the beginning of SET
  or during the SET phase re-enter from the side.
- Robots at a legal position during the SET phase are not penalized, even if
  they did not move during the READY phase.
- No robots is penalized once state is PLAYING again.

## Setup

- 3 red robots and 3 blue robots are used
- Kick-off is attributed to RED team
- The scenario starts in PLAYING

## Description

1. Team RED scores a goal
2. Referee set state to READY and robot move to the following locations:
   - `Red 1` goes to the opponent's field.
   - `Red 2` stays inside center circle while not having kick-off,
   - `Red 3` moves to a valid position
   - `Blue 1` stays at its valid location
   - `Blue 2` goes inside center circle while having kick-off,
   - `Blue 3` moves out of the field
3. Time elapses until state SET is reached, `Red 1`, `Red 2` and `Blue 3` are
   penalized.
4. During `SET`, `Blue 1` falls into opponent's field and is immediately penalized.
5. Time elapses and state PLAYING is reached again, all penalties are removed
