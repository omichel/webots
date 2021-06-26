# Leaving field

## What is tested

- When a robot stays on the turf, even if it's out of the field area, it does
  not receive a penalty.
- When a robot moves out of the turf, it is not penalized immediately
- If a robot stays out of the turf for more than 20 seconds, it is penalized and
  placed again at the appropriate location.

## Setup

- Only one robot is used and the scenario starts from the PLAYING phase

## Description

1. RED 1 moves outside of the field
2. 20 second elapses it does not get penalized
3. RED 1 moves out of the turf
4. 20 seconds elapse, afterwards the robot get penalized and positioned at a the
   reentryStartingPose
