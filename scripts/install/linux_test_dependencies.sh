#!/bin/bash

if [[ $EUID -ne 0 ]]; then
       echo "This script must be run as root"
       exit 1
fi

UBUNTU_VERSION=$(lsb_release -rs)
if [[ $UBUNTU_VERSION == "16.04" ]]; then
       export ROS_DISTRO=kinetic
elif [[ $UBUNTU_VERSION == "18.04" ]]; then
       export ROS_DISTRO=melodic
elif [[ $UBUNTU_VERSION == "20.04" ]]; then
       export ROS_DISTRO=noetic
else
       echo "Unsupported Linux version: dependencies may not be completely installed. Only the two latest Ubuntu LTS are supported."
fi

if [[ $@ != *"--exclude-ros"* ]]; then
  echo "Adding ROS dependencies"
  #sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
  curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | apt-key add -
  apt update -qq

  apt install -y ros-$ROS_DISTRO-ros-base ros-$ROS_DISTRO-sensor-msgs ros-$ROS_DISTRO-tf ros-$ROS_DISTRO-ros-controllers ros-$ROS_DISTRO-controller-manager ros-$ROS_DISTRO-ros-control ros-$ROS_DISTRO-class-loader ros-$ROS_DISTRO-roslib ros-$ROS_DISTRO-moveit-ros liburdfdom-tools

  if [[ $UBUNTU_VERSION == "16.04"  || $UBUNTU_VERSION == "18.04" ]]; then
         apt install -y python-rosdep
  elif [[ $UBUNTU_VERSION == "20.04" ]]; then
         apt install -y python3-rosdep
  fi
fi

if [[ $@ != *"--norecurse"* ]]; then
  echo "Installing runtime dependencies"
  script_full_path=$(dirname "$0")
  $script_full_path/linux_runtime_dependencies.sh
fi
