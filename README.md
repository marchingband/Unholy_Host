# midi2cv
Firmware for a midi2cv device

## libraries needed
MIDIUSB (available in Arduino library manager)  
MCP48xx (available in Arduino library manager)  
ShiftRegister74HC595 (available in Arduino library manager)  
USB_Host_Library_SAMD ( https://github.com/marchingband/USB_Host_Library_SAMD )  

## board
Seeeduino XIAO ( https://files.seeedstudio.com/arduino/package_seeeduino_boards_index.json )

## step-by-step
* Install Arduino IDE, and open it  
* click Arduino->preferences  
* beside "Additional Board Manager URLS" click the dialogue, and paste the following URL into it:  
https://files.seeedstudio.com/arduino/package_seeeduino_boards_index.json  
* click tools->board->board manager, search for Seeed SAMD Boards, and install it  
* click tools->board->Seeed SAMD->Seeeduino XIAO  
* click tools->manage libraries  
* search for, and install the following 3 libraries  
* * MIDIUSB  
* * MCP48xx  
* * ShiftRegister74HC595  
* open a web browser and navigate to https://github.com/marchingband/USB_Host_Library_SAMD  
* click code->Download Zip, and then unzip this file into /Arduino/libraries  
* now navigate to https://github.com/marchingband/midi2cv  
* click code->Download Zip, and then unzip this file into /Arduino/ (this is your sketch library)
* in the Arduino IDE open the file Arduino/midi2cv/midi2cv.ino
