# midi2cv
Firmware for a midi2cv device

## Build the firmware

install PLATFORMIO core or IDE : https://platformio.org/install/cli  
`$ git clone https://github.com/marchingband/midi2cv.git`  
`$ cd midi2cv`  
`$ pio run`  

## Edit the configuration

open `scr/config.h` in a text editor  
change the `#define`'s to one of the options listed to the right of any given option  
to compile your code and upload to the board, connect via usb-c, double-tap the reset button, and run:  
`$ pio run -t upload`
