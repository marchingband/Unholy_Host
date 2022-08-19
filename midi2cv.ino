#include <Arduino.h>
#include "usb_host.h"
#include "usb_device.h"
#include "midi.h"
#include "gates.h"
#include "dac.h"
#include "calibration.h"
#include "handlers.h"

#define USB_MODE_SELECT_PIN 5
// #define TEST_MODE

int is_device = 0;

void test_lights(void)
{
  static uint8_t cnt = 0;
  static uint8_t cnt2 = 0;
  for(int i = 0; i<16; i++)
  {
    int even = cnt++ % 2 == 0;
    gate_set(cnt2, even);
    gates_update();
    cnt2 = even ? cnt2 : cnt2 + 1;
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

  // start the shift register
  gates_init();

  // blink
  test_lights();
  
  // build the dac just-notes LUT
  init_just_dac_vals();

  // build the midi event handlers for gates
  init_handlers();

  // start the dac
  dac_init();
  // test_dac();

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
//4095
uint test_vals[] = {
  0,
  500,
  1000,
  1500,
  2000,
  2500,
  3000,
  3500,
};
uint test_index = 0;

void loop()
{
#ifdef TEST_MODE
  while(1){
    dac_set_chan_all(test_vals[test_index], test_vals[test_index]);
    delay(1000);
    if(test_index < 7){
      test_index++;
    } else {
      test_index = 0;
    }
  }
#endif
  if(is_device)
  {
    usb_device_loop();
  }
  else
  {
    usb_host_loop();
  }
  clear_triggers();
}
