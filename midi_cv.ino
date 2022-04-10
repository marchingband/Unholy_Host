#include <Arduino.h>
#include "usb_host.h"
#include "usb_device.h"
#include "midi.h"
#include "gates.h"
#include "dac.h"
#include "rgb_led.h"

#define USB_MODE_SELECT_PIN 5

int is_device = 0;

void test_lights(void)
{
  static uint8_t cnt = 0;
  static uint8_t cnt2 = 0;
  for(int i = 0; i<16; i++)
  {
    int even = cnt++ % 2 == 0;
    gate_set(cnt2, even);
    cnt2 = even ? cnt2 : cnt2 + 1;
    rgb_set_color(cnt < 3 ? cnt * (128 / 16) : 0, cnt < 6 ? cnt * (128 / 16) : 0, cnt < 9 ? cnt * (128 / 16) : 0); // test bright white
    delay(100);
  }

}

void setup()
{
  Serial1.begin(9600);
  Serial1.println("lets convert some USB MIDI to CV and GATE signals hey bud?");

  // led
  pinMode( 13, OUTPUT);
  digitalWrite(13, LOW);

  //start the reb led
  rgb_init();
  rgb_set_color(100,100,100); // test bright white

  // start the shift register
  gates_init();
  test_lights();
  
  // start the dac
  dac_init();
  dac_set_chan_all(100, 4096);  // test the dac : 4.04v 4096

  // start the parser
  midi_parser_init();

  // check the host/device pin
  pinMode(5, INPUT_PULLUP);
  is_device = digitalRead(USB_MODE_SELECT_PIN);
  if(is_device)
  {
    Serial1.println("USB mode select pin set to DEVICE");
  }
  else
  {
    Serial1.println("USB mode select pin set to HOST");
    // start the usb host
    usb_host_init();
  }
}

void loop()
{
  if(is_device)
  {
    usb_device_loop();
  }
  else
  {
    usb_host_loop();
  }
}
