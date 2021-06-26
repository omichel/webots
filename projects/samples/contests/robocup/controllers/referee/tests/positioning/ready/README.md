# Positioning - ready

## What is tested

We have some robots initially placed outside of the field in valid positions and are going to be in illegal positions after finishing ready state.

## Setup

- 4 red robots and 3 blue robots are used

## Description

1. In `READY` state, `Red 2` goes inside center circle while having kick-off, `Blue 2` goes inside center circle while not having kick-off, `Red 1` goes to the opponent's field.
2. `Blue 2` and `Red 1` are penalized. `Red 3`, `Red 4`, and `Blue 3` are also penalized due to not moving from `INITIAL` until end of `READY`.
3. During `SET`, `Blue 1` falls into opponent's field and is immediately penalized.