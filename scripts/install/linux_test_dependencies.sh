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

sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
apt-key adv --keyserver 'hkp://keyserver.ubuntu.com:80' --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654
apt update -qq
apt-get install -y ros-$ROS_DISTRO-ros-base ros-$ROS_DISTRO-sensor-msgs ros-$ROS_DISTRO-tf ros-$ROS_DISTRO-ros-controllers ros-$ROS_DISTRO-controller-manager ros-$ROS_DISTRO-ros-control ros-$ROS_DISTRO-class-loader ros-$ROS_DISTRO-roslib liburdfdom-tools

if [[ $1 != "--norecurse" ]]; then
	script_full_path=$(dirname "$0")
	$script_full_path/linux_runtime_dependencies.sh
fi
