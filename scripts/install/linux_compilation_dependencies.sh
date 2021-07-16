#!/bin/bash

if [[ $EUID -ne 0 ]]; then
       echo "This script must be run as root"
       exit 1
fi

apt update
apt install --yes git lsb-release cmake swig libglu1-mesa-dev libglib2.0-dev libfreeimage-dev libfreetype6-dev libxml2-dev libzzip-0-13 libboost-dev libgd3 libssh-gcrypt-dev libzip-dev libreadline-dev pbzip2 libpci-dev wget libssl-dev zip unzip

UBUNTU_VERSION=$(lsb_release -rs)
if [[ $UBUNTU_VERSION == "20.04" ]]; then
       apt install --yes libzip5
else
       echo "Unsupported Linux version: dependencies may not be completely installed. Only the two latest Ubuntu LTS are supported."
fi

script_full_path=$(dirname "$0")
$script_full_path/linux_runtime_dependencies.sh
