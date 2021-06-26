# Communication - end of game

This test scenario checks that after the end of a game, the client get properly
disconnected with an appropriate message.

## Manual check

The content of `client_1` should have the following elements:

- It should contain: "Connected to 127.0.0.1"
- It should end with: "Runtime error: Simulator interrupted the connection"
