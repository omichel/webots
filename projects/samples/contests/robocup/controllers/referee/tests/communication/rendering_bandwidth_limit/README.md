# Communication - rendering bandwidth limit

This test scenario checks that if a team requests too much bandwidth for image
rendering, an error message is sent to the client.

## Setup

There are 3 clients active:

- `client_1`: Robot `RED 1` alone in his team, 1920x1080 images with timestep of 16
- `client_2`: Robot `BLUE 1` alone in his team, 1920x1080 images with timestep of 32
- `client_3`: Robot `BLUE 2` alone in his team, 1920x1080 images with timestep of 64

## Manual check

The content of `client_1` should have the following elements:

- It should contain: "Connected to 127.0.0.1"
- It should contain: "WARNING_MESSAGE: requested rendering bandwidth is above the limit (371MB/s > 350MB/s)

The content of `client_2` should have the following elements:

- It should contain: "Connected to 127.0.0.1"
- It should contain: "WARNING_MESSAGE: requested rendering bandwidth is above the limit (185MB/s > 175MB/s)

The content of `client_3` should have the following elements:

- It should contain: "Connected to 127.0.0.1"
- It should **not** contain: "WARNING_MESSAGE: requested rendering bandwidth ..."

## Missing tests

- Robot with multiple cameras active simultaneously
- Valid camera swap during execution (disabling first and then enabling the
  other camera)
- Invalid camera swap during execution (enabling second camera and then
  disabling the first one)
