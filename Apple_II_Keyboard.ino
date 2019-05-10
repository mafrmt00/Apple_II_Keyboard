#include "II_Encoder.h"
#include <hidboot.h>
#include <usbhub.h>
// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>
#include <avr/pgmspace.h>
#include "layoutmemory.h"
#include "backlog.h"

const char string_0[] PROGMEM = " .----------------.  .----------------.  .----------------.  .----------------.  .----------------.   .----------------.  .----------------. ";
const char string_1[] PROGMEM = "| .--------------. || .--------------. || .--------------. || .--------------. || .--------------. | | .--------------. || .--------------. |";
const char string_2[] PROGMEM = "| |      __      | || |   ______     | || |   ______     | || |   _____      | || |  _________   | | | |     _____    | || |     _____    | |";
const char string_3[] PROGMEM = "| |     /  \\     | || |  |_   __ \\   | || |  |_   __ \\   | || |  |_   _|     | || | |_   ___  |  | | | |    |_   _|   | || |    |_   _|   | |";
const char string_4[] PROGMEM = "| |    / /\\ \\    | || |    | |__) |  | || |    | |__) |  | || |    | |       | || |   | |_  \\_|  | | | |      | |     | || |      | |     | |";
const char string_5[] PROGMEM = "| |   / ____ \\   | || |    |  ___/   | || |    |  ___/   | || |    | |   _   | || |   |  _|  _   | | | |      | |     | || |      | |     | |";
const char string_6[] PROGMEM = "| | _/ /    \\ \\_ | || |   _| |_      | || |   _| |_      | || |   _| |__/ |  | || |  _| |___/ |  | | | |     _| |_    | || |     _| |_    | |";
const char string_7[] PROGMEM = "| ||____|  |____|| || |  |_____|     | || |  |_____|     | || |  |________|  | || | |_________|  | | | |    |_____|   | || |    |_____|   | |";
const char string_8[] PROGMEM = "| |              | || |              | || |              | || |              | || |              | | | |              | || |              | |";
const char string_9[] PROGMEM = "| '--------------' || '--------------' || '--------------' || '--------------' || '--------------' | | '--------------' || '--------------' |";
const char string_A[] PROGMEM = "'----------------'  '----------------'  '----------------'  '----------------'  '----------------'   '----------------'  '----------------'  ";

const char *const string_table[] PROGMEM = {string_0, string_1, string_2, string_3, string_4, string_5, string_6, string_7, string_8, string_9, string_A};
char buffer[145];  // make sure this is large enough for the largest string it must hold

II_Encoder KeyEncoder;
KeyLayout CurrLayout;
Backlog StringBackLog;

class KbdRptParser : public KeyboardReportParser
{   void PrintKey(uint8_t mod, uint8_t key);

  protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);

    void OnKeyDown  (uint8_t mod, uint8_t key);
    void OnKeyUp  (uint8_t mod, uint8_t key);
    void OnKeyPressed(uint8_t key);

  public:
    bool bDebugPrint;
};

void KbdRptParser::PrintKey(uint8_t m, uint8_t key)
{
  MODIFIERKEYS mod;
  *((uint8_t*)&mod) = m;
  if (bDebugPrint)
  {
    Serial.print((mod.bmLeftCtrl   == 1) ? "C" : " ");
    Serial.print((mod.bmLeftShift  == 1) ? "S" : " ");
    Serial.print((mod.bmLeftAlt    == 1) ? "A" : " ");
    Serial.print((mod.bmLeftGUI    == 1) ? "G" : " ");

    Serial.print(" >");
    PrintHex<uint8_t>(key, 0x80);
    Serial.print("< ");

    Serial.print((mod.bmRightCtrl   == 1) ? "C" : " ");
    Serial.print((mod.bmRightShift  == 1) ? "S" : " ");
    Serial.print((mod.bmRightAlt    == 1) ? "A" : " ");
    Serial.println((mod.bmRightGUI    == 1) ? "G" : " ");
  }
};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
  //Serial.print("DN ");
  //PrintKey(mod, key);
  uint8_t c = OemToAscii(mod, key);
  if (bDebugPrint)
  {
    Serial.print("MOD: ");
    Serial.print(mod, HEX);
    Serial.print(" KEY: ");
    Serial.println(key, HEX);
  }

  if (0x52 == key) //Arrow UP
  {
    StringBackLog.OneBack();
  }
  else if (0x51 == key) //Arrow DOWN
  {
    StringBackLog.OneForeward();
  }
  else if (c)
  {
    OnKeyPressed(c);
  }
}

void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after)
{

  MODIFIERKEYS beforeMod;
  *((uint8_t*)&beforeMod) = before;

  MODIFIERKEYS afterMod;
  *((uint8_t*)&afterMod) = after;

  if (beforeMod.bmLeftCtrl != afterMod.bmLeftCtrl) {
    if (bDebugPrint)
      Serial.println("LeftCtrl changed");
  }
  if (beforeMod.bmLeftShift != afterMod.bmLeftShift) {
    if (bDebugPrint)
      Serial.println("LeftShift changed");
  }
  if (beforeMod.bmLeftAlt != afterMod.bmLeftAlt) {
    if (bDebugPrint)
      Serial.println("LeftAlt changed");
  }
  if (beforeMod.bmLeftGUI != afterMod.bmLeftGUI) {
    if (bDebugPrint)
      Serial.println("LeftGUI changed");
  }

  if (beforeMod.bmRightCtrl != afterMod.bmRightCtrl) {
    if (bDebugPrint)
      Serial.println("RightCtrl changed");
  }
  if (beforeMod.bmRightShift != afterMod.bmRightShift) {
    if (bDebugPrint)
      Serial.println("RightShift changed");
  }
  if (beforeMod.bmRightAlt != afterMod.bmRightAlt) {
    if (bDebugPrint)
      Serial.println("RightAlt changed");
  }
  if (beforeMod.bmRightGUI != afterMod.bmRightGUI) {
    if (bDebugPrint)
      Serial.println("RightGUI changed");
  }

}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{
  //Serial.print("UP ");
  //PrintKey(mod, key);
}

void KbdRptParser::OnKeyPressed(uint8_t key)
{
  if (bDebugPrint)
  {
    Serial.print("ASCII: ");
    Serial.print((char)key);
    Serial.print(" ");
    Serial.println((char)key, HEX);
  }
  else
  {
    StringBackLog.addchar((char)key);

    if (0x0D == key)
      Serial.println();
    else
      Serial.print((char)key);
  }

  if (0x11 == key)
  {
    if (bDebugPrint)
      Serial.print("Reset");
    KeyEncoder.Reset();
  }
  else if (0x12 == key)
  {
    CurrLayout.Toggle();
    SetLayout(CurrLayout.Get());
  }
  else
    KeyEncoder.IIputchar((char)key);
};

USB     Usb;
//USBHub     Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);

KbdRptParser Prs;

/*! \brief Wrapper for callback use.

*/
int IIputsWrapper(const char *string)
{
  KeyEncoder.IIputchar(string);
}

/*! \brief Print a welcome message

   Prints some nice ASCII art through the arduino terminal as a welcome.
*/
void print_welcome_msg(void)
{
  Serial.println("");
  Serial.println("Welcome to:");
  Serial.println("");

  for (int i = 0; i < 11; i++) {
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[i])));  // Necessary casts and dereferencing, just copy.
    Serial.println(buffer);
  }

  Serial.println("");
  Serial.println("USB-Keyboard interface.");
  Serial.println("");
};

/*! \brief The Arduino setup functiopn.

   Called once at startup.
*/
void setup()
{
  Prs.bDebugPrint = false;

  Serial.begin( 115200 );
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  print_welcome_msg();

  CurrLayout.Init();

  if (Usb.Init() == -1)
    Serial.println("OSC did not start.c");

  delay( 200 );

  HidKeyboard.SetReportParser(0, &Prs);

  KeyEncoder.begin();
  //KeyEncoder.IIputs(" ");

  Prs.SetLayout(CurrLayout.Get());

  StringBackLog.SetPuts(&IIputsWrapper);

}

/*! \brief The Arduino mainloop.

   Only the USB Task is polled here.
*/
void loop()
{
  Usb.Task();
}

