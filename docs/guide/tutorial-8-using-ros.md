## Tutorial 8: Using ROS (60 Minutes)

This tutorial explains how to use the nodes from the `webots_ros` package provided with Webots.

These examples were tested with ROS `noetic` and `melodic` on Linux.
There is no warranty they will work if you use a different platform or an ancient distribution of ROS.

### Installing ROS and "webots\_ros" Package

In order to use these nodes, you will first need to install the ROS framework.
To install the latest version of ROS on Ubuntu use the following commands:

%tab-component "ros"

%tab "noetic"


```sh
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg
sudo apt-get update
sudo apt-get install ros-noetic-desktop-full ros-noetic-moveit # takes time, get a coffee :)
sudo apt-get install python3-rosdep
sudo rosdep init
rosdep update
sudo apt-get install ros-noetic-webots-ros
```

%tab-end

%tab "melodic"

```sh
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg
sudo apt-get update
sudo apt-get install ros-melodic-desktop-full ros-melodic-moveit # takes time, get a coffee :)
sudo apt-get install python-rosdep
sudo rosdep init
rosdep update
sudo apt-get install ros-melodic-webots-ros
```

%tab-end


%end

For more information or to install it on another platform please read [http://wiki.ros.org/ROS/Installation](http://wiki.ros.org/ROS/Installation).
Unless you need older version for some other application, you should choose a recent distribution (Noetic Ninjemys or Melodic Morenia).

The last line is to install the [webots\_ros](http://wiki.ros.org/webots\_ros) package.

If you never used the ROS framework before, it is strongly recommended to follow some tutorials (_Beginner Level_) from: [http://wiki.ros.org/ROS/Tutorials](http://wiki.ros.org/ROS/Tutorials).
These tutorials will also help you set up your ROS environment and initialize your catkin workspace.
The minimum requirement is to follow these instructions (taken from the [ROS tutorial 1.1.1](http://wiki.ros.org/ROS/Tutorials/InstallingandConfiguringROSEnvironment)).
1. Open a terminal
2. Check if the ROS environment is properly set-up: `printenv | grep ROS`
3. If not, source the setup script: `source /opt/ros/noetic/setup.bash`. Check it again to be sure.
4. Create a `catkin_ws` workspace for ROS:
  1. Create some folders: `mkdir -p ~/catkin_ws/src`
  2. Go back in the main folder: `cd ~/catkin_ws/`
  3. Generate files by compilation: `catkin_make`
  4. Source again the new files: `source devel/setup.bash`
5. To make sure your workspace is properly set-up by the setup script, make sure `ROS_PACKAGE_PATH` environment variable includes the directory you are in. The `echo $ROS_PACKAGE_PATH` command should return `/home/youruser/catkin_ws/src:/opt/ros/noetic/share`.
6. The last step is to set the [`WEBOTS_HOME`](https://cyberbotics.com/doc/guide/compiling-controllers-in-a-terminal) environment variable: `export WEBOTS_HOME=/usr/local/webots`. If you installed Webots in a different place, adapt the variable. This line can be added to your `.bashrc` file.

** Remember**: Each time you open a terminal, you have to source the environment with this command: `source /opt/ros/noetic/setup.bash` or you can add this line in your `.bashrc` file.

### Running the Nodes

You can start any simulation using ROS with the following commands (here the `e_puck_line` one for example):

```sh
source /opt/ros/noetic/setup.bash
roslaunch webots_ros e_puck_line.launch
```
This launch file will launch Webots (the `WEBOTS_HOME` environment variable should be set) and start the corresponding node.

<details>
<summary>Webots snap and ROS Noetic notice</summary>

If Webots is installed as a snap package you need to append the `${WEBOTS_HOME}/projects/default/controllers/ros/lib/ros` path to the `LD_LIBRARY_PATH` environment variable:

```sh
export WEBOTS_HOME=/snap/webots/current/usr/share/webots
source /opt/ros/noetic/local_setup.bash
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${WEBOTS_HOME}/projects/default/controllers/ros/lib/ros
```
  
This is specific to the Webots snap package and ROS Noetic.
  
</details>

The seed of Webots' random number generator is initialized at the beginning of the simulation and not when the ROS nodes connect.
Webots has to be running for the ROS nodes to connect.
However, we cannot guarantee how long it will run before the ROS nodes connect.
Therefore, the sensor measurements and motor commands will slightly differ from one run to another, due to the noise being slightly different at the time of the connection of the ROS nodes.
This may have consequences on the behavior of the robots, thus making such simulations non fully reproducible.
You can use the `--synchronize` argument in order to make sure that Webots will not run before the ROS node connects.
This is useful to make ROS-based simulation reproducible.

If you want to use different computers for the ROS master, the Webots simulation and/or the nodes, you must be able to connect to each of them with SSH in both ways.
The hostname and IP addresses of these computers should be listed in the known hosts list of each computer and the `ROS_MASTER_URI` variable should be adjusted accordingly.

### Creating New Nodes

These examples only show a few possibilities for interfacing ROS and Webots, but you can build your own nodes to connect with Webots.
The source code of these nodes can be found on the repository of the [webots\_ros](https://github.com/cyberbotics/webots\_ros) package.

The `robot_information_parser` node is the most basic one and is a good base to start building your own node.
The `complete_test` node doesn't show any particular application but contains an almost exhaustive list of the Webots API functions.

All the functions from the Webots API have their corresponding services or topics.
You can find in the [Reference Manual](../reference/nodes-and-api-functions.md) the definitions of all the services and topics associated to each device.

### "webots\_ros" Package

If you are running the latest version of Webots, the easiest way to setup the `webots_ros` package is to install the `ros-$ROS_DISTRO-webots-ros` package directly from the package manager as described above.
But if you are running an older Webots version, some functionalities might not be fully supported or might be broken by the latest `ros-$ROS_DISTRO-webots-ros` package.
In this case, it is possible to install an older `webots_ros` package version from sources following the instructions on the [ROS wiki page](http://wiki.ros.org/webots_ros#From_Sources) and selecting the tag/release matching the Webots version in the [`webots_ros` GitHub repository](https://github.com/cyberbotics/webots_ros/releases).
