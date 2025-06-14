#!/bin/bash

xhost +local:docker

docker run --rm -it \
    --env DISPLAY=$DISPLAY \
    --volume /tmp/.X11-unix:/tmp/.X11-unix \
    --volume "$(pwd)":/home/devuser/app \
    dicom-client \
    bash -c "rm -r build && mkdir -p build && cd build && cmake .. && make && ctest -V"
