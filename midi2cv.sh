#!/bin/bash 

if [ "$1" == "code" ]; then
    echo "compiling..."
    pio run -t upload && pio device monitor
    echo "done"
elif [ "$1" == "flash" ]; then 
    echo "uploading..." && \
    pio run -t nobuild -t upload && pio device monitor
    echo "done"
elif [ "$1" == "compile" ]; then 
    echo "compiling..."
    pio run``
    echo "done"
elif [ "$1" == "clean" ]; then 
    echo "compiling..."
    pio run -t clean
    echo "done"
else
    echo "please use one of flags : clean, code, compile, or flash"
fi