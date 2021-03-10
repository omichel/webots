# docker build . --tag headlesstest
# docker run -v ${PWD}:/home --workdir=/home -it headlesstest

FROM ubuntu:latest

ENV DEBIAN_FRONTEND=noninteractive
RUN apt -y update && \
    apt install -y libopengl-dev libgssapi-krb5-2 libxcursor1 libxi-dev \
        libxrandr2 libdbus-1-3 libasound2 lsb-release g++ make libavcodec-extra \
        libglu1-mesa libxkbcommon-x11-dev execstack libusb-dev libxcb-keysyms1 \
        libxcb-image0 libxcb-icccm4 libxcb-randr0 libxcb-render-util0 libxcb-xinerama0 \
        libxcomposite-dev libxtst6 libnss3 libglfw3-dev libgles2-mesa-dev libosmesa6-dev \
        qt5-default
