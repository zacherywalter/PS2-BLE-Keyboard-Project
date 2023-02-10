# README PS2-BLE-Keyboard

by Zachery Walter 21.01.2023

## About

This Project includes code to help revive your old ps2 keyboard to a bluetooth keyboard using an [esp32-wroom-32 board](https://de.aliexpress.com/item/32858054775.html?spm=a2g0o.order_list.order_list_main.10.6e215c5fzWOnmD&gatewayAdapt=glo2deu) programmed with Arduino IDE. The PS2 keyboard used is similiar to [this one](https://www.mindfactory.de/product_info.php/KeySonic-ACK-595C--Mini-Tastatur-Weiss-Deutsch-PS2-USB_590520.html) but any ps2 keyboard should do.

### Dependencies

- Ps2 library and explanation for arduino
  [ps2 library used here](https://playground.arduino.cc/ComponentLib/Ps2mouse/)
  It is necessary to change wprogram.h into Arduino.h in ps2.h
  [Instructables ps2 explanation](https://www.instructables.com/Connect-PS2-Keyboard-to-Arduino/)
  [Didn't use this one but good source](https://www.pjrc.com/teensy/td_libs_PS2Keyboard.html)

- **ESP32-BLE-Keayboard** library used in arduino IDE on **esp wroom 32** 
  
  [GitHub - T-vK/ESP32-BLE-Keyboardy](https://github.com/T-vK/ESP32-BLE-Keyboard). #define USE_NIMBLE must be defined in BleKeyboard.h . For this it is required to install...

- NimBLE Library
  
  [GitHub - h2zero/NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino)
  
  Explanation to nimble: [NimBLE-based host APIs - ESP32](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/bluetooth/nimble/index.html)
  
  I read it but did't really get it, but it makes processing faster so that's a plus

- Ps2 to ascii:

### Missing Features/Todos

- [ ] Highlighting words using Shift and arrow keys only works with right shift, don't know why.

- [ ] Numlock and Scrolllock only toggle the leds on the keyboard

- [ ] Menu/Application key not integrated

- [ ] Printscreen and Pause button not integrated

- [ ] Couldn't get the "<", ">" keys to print

### Useful Resources for dev

- The Main Bulk of the code is inspired by code from [this website](https://community.infineon.com/t5/PSoC-5-3-1/PS-2-Keyboard-to-ASCII-Decoder-for-5LP/td-p/55867)

- [Useful explaination of ps2-11byte flow to ascii conversion]([Get ASCII Data from PS-2 Keyboards | Nuts & Volts Magazine](https://www.nutsvolts.com/magazine/article/get-ascii-data-from-ps-2-keyboards))

- Good explainations of the PS2 key codes can be found here:

- - https://wiki.osdev.org/PS/2_Keyboard

- - https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html

- A useful key event view: https://w3c.github.io/uievents/tools/key-event-viewer.html

- See Keycodes.txt to see a list of Collected Keycodes in HEX
