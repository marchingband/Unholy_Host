#!/bin/bash 

if [ "$1" == "code" ]; then
    echo "compiling..."
    arduino-cli compile -e --fqbn Seeeduino:samd:seeed_XIAO_m0 midi2cv && \
    echo "done compilation, uploading..." && \
    arduino-cli upload -p /dev/cu.usbmodemFA131 --fqbn Seeeduino:samd:seeed_XIAO_m0 midi2cv && \
    echo "done"
elif [ "$1" == "flash" ]; then 
    echo "uploading..." && \
    arduino-cli upload -p /dev/cu.usbmodemFA131 --fqbn Seeeduino:samd:seeed_XIAO_m0 midi2cv && \
    echo "done"
elif [ "$1" == "compile" ]; then 
    echo "compiling..."
    arduino-cli compile -e --fqbn Seeeduino:samd:seeed_XIAO_m0 midi2cv && \
    echo "done"
elif [ "$1" == "clean" ]; then 
    echo "compiling..."
    arduino-cli compile -e --clean --fqbn Seeeduino:samd:seeed_XIAO_m0 midi2cv && \
    echo "done"
else
    echo "please use one of flags : clean, code, compile, or flash"
fi