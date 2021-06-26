# Web Component Studio

The Web Component Studio is a script used to create the 3D web components displayed in the online documentation.

A web component is an interactive 3D view of a Webots model, typically a robot.
If the model is a robot, the web component also displays the list of devices of that robot, in addition to the 3D view of the robot.
Motors can be moved using sliders.
The device origin location are displayed when the mouse is over an item of the device list.

An example of a web component showing the Aibo ERS7 robot is displayed on the top of the following page: https://cyberbotics.com/doc/guide/aibo-ers7


## Requirements

- Python 3.5 or above.
- lxml: `pip install lxml`.

## Usage

```sh
cd $WEBOTS_HOME
cd scripts/web_component_studio
python web_component_studio.py
```
