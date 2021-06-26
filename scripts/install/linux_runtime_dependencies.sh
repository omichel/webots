#!/bin/bash

if [[ $EUID -ne 0 ]]; then
       echo "This script must be run as root"
       exit 1
fi

apt update
apt install --yes lsb-release g++ make libavcodec-extra libglu1-mesa libxkbcommon-x11-dev execstack libusb-dev libxcb-keysyms1 libxcb-image0 libxcb-icccm4 libxcb-randr0 libxcb-render-util0 libxcb-xinerama0 libxcomposite-dev libxtst6 libnss3 protobuf-compiler
if [[ -z "$DISPLAY" ]]; then
       apt install --yes xvfb
fi

UBUNTU_VERSION=$(lsb_release -rs)
if [[ $UBUNTU_VERSION == "16.04" ]]; then
       apt install --yes libav-tools
elif [[ $UBUNTU_VERSION == "18.04" ]]; then
       apt install --yes ffmpeg
elif [[ $UBUNTU_VERSION == "20.04" ]]; then
       apt install --yes ffmpeg
else
       echo "Unsupported Linux version: dependencies may not be completely installed. Only the two latest Ubuntu LTS are supported."
fi
