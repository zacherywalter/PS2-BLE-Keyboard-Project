/*
  Code put together by Zachery Walter 29.01.2023
  Inspired by code by Jon Titus:
  PS-2 PS/2 Keyboard decoder gets ASCII values
  Jon Titus, Herriman, UT 2018
  03-12-2018 at 2007H MDST
*/

#include <ps2.h>
#include <BleKeyboard.h>
#include "espAdcLut.h"
//Lowercase array with Function keys added, 16 elements per row
uint8_t LC_Array[] =
{ 0x00, 0xCA, 0x00, 0xC6, 0xC4, 0xC2, 0xC3, 0xCD, 0x00, 0xCB, 0xC9, 0xC7, 0xC5, 0xB3, 0x60, 0x00, //0   - 15
  0x00, 0x00, 0x00, 0x00, 0x00, 0x71, 0x31, 0x00, 0x00, 0x00, 0x7A, 0x73, 0x61, 0x77, 0x32, 0x00, //16  - 31
  0x00, 0x63, 0x78, 0x64, 0x65, 0x34, 0x33, 0x00, 0x00, 0x20, 0x76, 0x66, 0x74, 0x72, 0x35, 0x00, //32  - 47
  0x00, 0x6E, 0x62, 0x68, 0x67, 0x79, 0x36, 0x00, 0x00, 0x00, 0x6D, 0x6A, 0x75, 0x37, 0x38, 0x00, //48  - 63
  0x00, 0x2C, 0x6B, 0x69, 0x6F, 0x30, 0x39, 0x00, 0x00, 0x2E, 0x2F, 0x6C, 0x3B, 0x70, 0x2D, 0x00, //64  - 79
  0x00, 0x00, 0x27, 0x00, 0x5B, 0x3D, 0x00, 0x00, 0xC1, 0x00, 0x0A, 0x5D, 0x00, 0x5C, 0x00, 0x00, //80  - 95
  0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //96  - 111
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB1, 0x00, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //112 - 127

  0x00, 0x00, 0x00, 0xC8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //128 - 143
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //144 - 159
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //160 - 175
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //176 - 191
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //192 - 207
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //208 - 223
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //224 - 239
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  //240 - 255
};

//Extended ASCII codes by keyboard-code indexes benninging with E0
uint8_t EX_Array[] =
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0   - 15
  0x00, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x83, //16  - 31
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, //32  - 47
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //48  - 63
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //64  - 79
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //80  - 95
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD5, 0x00, 0xD8, 0xD2, 0x00, 0x00, 0x00, //96  - 111
  0xD1, 0xD4, 0xD9, 0x00, 0xD7, 0xDA, 0x00, 0x00, 0x00, 0x00, 0xD6, 0x00, 0x00, 0xD3, 0x00, 0x00, //112 - 127

  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //128 - 143
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //144 - 159
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //160 - 175
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //176 - 191
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //192 - 207
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //208 - 223
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //224 - 239
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  //240 - 255
};

#define BAT_LEVEL_PIN 34

//initialise blekeyboard object with ("device name", "manufacturer", initial battery level)
BleKeyboard bleKeyboard("ZacheryKeyboard", "Isaac Industries", 80);

//initialise ps2 object
//Pin 17 is the ps2 data pin, pin 22 is the clock pin
PS2 kbd(22, 17);//kbd(clk,data)

unsigned char Keyboard_Data;        //Extracted key code for a pressed key

uint8_t KeyRelease_Flag;            //Flag indicates the release of a key
uint8_t Extended_Flag;              //Extended key code indication 0xE0
uint8_t LED_State = 0b00000000;     //bit 2 = capslock, bit 1 = numberlock, bit 0 = scrolllock

uint8_t Old_Data;                   //Previous keyboard data
uint8_t New_Data;                   //Newest keyboard data
unsigned char Display_Data;         //ASCII value, or equivalent code for pressed key

//function to initialise the ps2 keyboard
void kbd_init() {
  char ack;
  kbd.write(0xff);   // send reset code
  ack = kbd.read();  // byte, kbd does self test
  ack = kbd.read();  // another ack when self test is done
}

//Function to handle flags when a key code is recieved by ps2 keyboard
void Modifier_Detect(uint8_t MODIFIER) {
  if (KeyRelease_Flag == 0x00) {      //if key has been pressed
    bleKeyboard.press(MODIFIER);      //No previous Shift detected before now, so set Shift_Key flag now
    Serial.print("pressed: ");
    Serial.println(MODIFIER, HEX);
    
  }
  else {                           //if key was released
    KeyRelease_Flag = 0x00;        //Clear key-release flag, too
    Extended_Flag = 0x00;          //Clear Extended_flag also? TODO: is this necessary?
    bleKeyboard.release(MODIFIER);
    Serial.print("released: ");
    Serial.println(MODIFIER, HEX);
  }
}

void Toggle_LED_States(uint8_t LED_Toggle) {  //bit 2 = capslock, bit 1 = numberlock, bit 0 = scrolllock
  kbd.write(0xED);                            //ps2 set led state command
  LED_State ^= LED_Toggle;                    //Xor boolean with state to toggle state
  unsigned char Response = kbd.read();        //get response
  //Serial.println(Response, HEX);
  if (Response ==  0xFA) {                    //response acknowledge
    kbd.write(LED_State);                     //send led states to be displayed
  } else if (Response == 0xFE) {              //response is a resend request
    Serial.println("ps2 kbd requests a command resend");
  } else {
    Serial.println(" led change state command Response not recognised");
  }
}

void setTypematicByte(void){
  delay(20);
  Serial.println("getting scan code set");
  kbd.write(0xF3);
  delay(20);
  uint32_t Response = kbd.read();        //get response
  Serial.println(Response, HEX);
  //bit7 0 bit 6,5 01b 500ms bit 4,0 00000b 30Hz repeat
  kbd.write(0b00100000);
  delay(20);
  Response = kbd.read();        //get response
  Serial.println(Response, HEX);
}


//function to handle a keypress / convert ps2 to ascii
void handle_keypress() {

  switch (Keyboard_Data) {
    case 0xF0:                  //Key-release code 0xF0 detected
      KeyRelease_Flag = 0x01;     //set KeyRelease_Flag
      break;

    case 0xE0:                            //Extended key code 0xE0 detected
      Extended_Flag = 0x01;               //set Extended_Flag
      break;

    case 0x14:                            //left ctrl key code detected
      Modifier_Detect(KEY_LEFT_CTRL);
      break;

    case 0x11:                            //Alt key code detected
      {
        if (Extended_Flag == 0x00) {      //TODO: make a switch case?
          Modifier_Detect(KEY_LEFT_ALT);
        } else {
          Modifier_Detect(KEY_RIGHT_ALT);
          Extended_Flag = 0x00;
        }
        break;
      }

    case 0x12:                            //left shift key detected
      Modifier_Detect(KEY_LEFT_SHIFT);
      break;

    case 0x59:                            //right shift key detected
      Modifier_Detect(KEY_RIGHT_SHIFT);
      break;

    case 0x1F:                            //left window key detected
      {
        if (Extended_Flag = 0x01) {
          Modifier_Detect(KEY_LEFT_GUI);
          Extended_Flag = 0x00;
          break;
        }
      }
    case 0x27:                            //right window key detected
      {
        if (Extended_Flag = 0x01) {
          Modifier_Detect(KEY_RIGHT_GUI);
          Extended_Flag = 0x00;
          break;
        }
      }


    //When no "case" applies, program jumps (breaks) to here
    default:                                      //Detect upper- or lower-case condition, get ASCII code
      Old_Data = New_Data;                        //Save previous data so we can check it
      New_Data = Keyboard_Data;                   //when key released.  Save newest UART input
      if (KeyRelease_Flag == 0x00) {              //If no key-release detected yet,
        //check for extended flag
        if (Extended_Flag == 0x00) {              //no extended key detected
          Display_Data = LC_Array[Keyboard_Data]; //Get lower-case ASCII value from array
        } else {                                  //extended flag set
          Display_Data = EX_Array[Keyboard_Data]; //Get Extended ASCII value from array
        }
        //or use Display_Data as you choose, send data over bluetooth
        if (bleKeyboard.isConnected()) {
          bleKeyboard.write(Display_Data);
          //delay(20);
        }
      }
      else                                        //"End" byte detected, so compare newest key byte
      { //with the previous key's byte
        KeyRelease_Flag = 0x00;
        Extended_Flag = 0x00;
        if (Old_Data == New_Data) {               //If old and new data don't agree,
          // Handle error here
          Serial.println("Error parsing ps2 data stream");
        }
      }
  }
}

//function to send led status to leds on ps2 keyboard
void handle_LEDS(void) {
  switch (Keyboard_Data) {
    case 0x58:                      //Capslock key pressed
      {
        if (KeyRelease_Flag == 0x00) {
          Toggle_LED_States(0x04);  //Toggle the caps led 0b00000100
          break;
        }
      }

    case 0x77:                      //Numlock key pressed
      {
        if (KeyRelease_Flag == 0x00) {
          Toggle_LED_States(0x02);  //Toggle the num led 0b00000010
          break;
        }
      }

    case 0x7E:                      //Scrolllock key pressed
      {
        if (KeyRelease_Flag == 0x00) {
          Toggle_LED_States(0x01);  //Toggle the scroll led 0b00000001
          break;
        }
      }
    default:
      //do nothing
      break;
  }
}

void setup(){
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  analogReadResolution(12);
  bleKeyboard.begin();              //initialise bluetooth keyboard hid
  kbd_init();                       //initialise ps2 keyboard
  setTypematicByte();
  
}

void loop(){
  Keyboard_Data = kbd.read();  //get a keycode from the kbd
  Serial.println(Keyboard_Data, HEX);  //print the keycode in serial
  handle_LEDS();      //function to send led status to leds on ps2 keyboard
  handle_keypress();  //function for converting ps2 commands into ascii characters
  uint16_t rawBatteryLevel = analogRead(BAT_LEVEL_PIN);
  uint16_t calBatteryLevel = ADC_LUT[rawBatteryLevel]/8;
  //Serial.println(calBatteryLevel);
  //bleKeyboard.setBatteryLevel(calBatteryLevel);
}
