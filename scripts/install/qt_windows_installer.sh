#!/bin/bash

# install the latest Qt 5 which comes with msys64

QT_PACKAGE=mingw-w64-x86_64-qt5
QT_WEBKIT_PACKAGE=mingw-w64-x86_64-qtwebkit

PACMAN_AVAILABLE_QT_VERSION=(`pacman -Syi $QT_PACKAGE | grep Version`)
IFS='-' read -a QT_AVAILABLE_VERSION_SPLIT <<< "${PACMAN_AVAILABLE_QT_VERSION[2]}"
AVAILABLE_QT_VERSION=${QT_AVAILABLE_VERSION_SPLIT[0]}

PACMAN_INSTALLED_QT_VERSION=(`pacman -Qi $QT_PACKAGE | grep Version`)
IFS='-' read -a QT_INSTALLED_VERSION_SPLIT <<< "${PACMAN_AVAILABLE_QT_VERSION[2]}"
INSTALLED_QT_VERSION=${QT_INSTALLED_VERSION_SPLIT[0]}

REQUESTED_QT_VERSION=$AVAILABLE_QT_VERSION  # we usually need to use the latest version of Qt5

echo Qt5 version requested: $REQUESTED_QT_VERSION
echo Qt5 version installed: $INSTALLED_QT_VERSION
echo Qt5 version available: $AVAILABLE_QT_VERSION

if [[ $REQUESTED_QT_VERSION != $INSTALLED_QT_VERSION ]] && [[ $REQUESTED_QT_VERSION != $AVAILABLE_QT_VERSION ]]
then
  echo Requested version not available
  exit
fi

if [[ $PACMAN_AVAILABLE_QT_VERSION != $PACMAN_INSTALLED_QT_VERSION ]]
then
  echo Installing Qt5 version ${PACMAN_AVAILABLE_QT_VERSION[2]}
  pacman -Sy --noconfirm $QT_PACKAGE
  pacman -Sy --noconfirm $QT_WEBKIT_PACKAGE
fi

script_name=$0
script_full_path=$(dirname "$0")
cd $script_full_path/../..

rm -rf include/qt

for MODULE in QtCore QtConcurrent QtGui QtNetwork QtOpenGL QtPrintSupport QtQml QtWebKit QtWebKitWidgets QtWebSockets QtWidgets QtXml
do
  echo installing $MODULE...
  mkdir -p include/qt/$MODULE/$MODULE
  cp /mingw64/include/$MODULE/* include/qt/$MODULE/$MODULE/ 2>&1 | grep -v 'omitting directory'
done

echo installing QtPlatformHeaders...
mkdir -p include/qt/QtPlatformHeaders/QtPlatformHeaders
cp /mingw64/include/QtPlatformHeaders/* include/qt/QtPlatformHeaders/QtPlatformHeaders/

echo done.
