#include "Arduino.h"
#include <EEPROM.h>
#include "layoutmemory.h"

#define MAX_KEYB_LAYOUT_ID 1

/*! \brief Only init variables

*/
KeyLayout::KeyLayout()
{
  uiCurrentKeyboardLayout = 0xFF;
}

/*! \brief Init function, not to be called from constructor.

   Restores settings from eeprom.
*/
void KeyLayout::Init(void)
{
  Restore();
}

/*! \brief Print current selected layout namew.

*/
void KeyLayout::Print(void)
{
  Serial.print("Keyboard Layout is: ");

  switch (uiCurrentKeyboardLayout)
  {
    case 0:
      Serial.println("US");
      break;
    case 1:
      Serial.println("DE");
      break;
    default:
      Serial.println(uiCurrentKeyboardLayout, HEX);
      break;
  }
}

/*! \brief Stores current active layout to eeprom.

*/
void KeyLayout::Store(void)
{
  EEPROM.write(0, uiCurrentKeyboardLayout);
}

/*! \brief Restores active layout from eeprom.

*/
void KeyLayout::Restore(void)
{
  Set(EEPROM.read(0));
}

/*! \brief Select new layout.

   Switch to the next layout and back to 0 if the MAX_KEYB_LAYOUT_ID is reached. 
*/
void KeyLayout::Toggle(void)
{
  Set(uiCurrentKeyboardLayout + 1);
}


/*! \brief Set the active layout direct..

   Sets the active layout and thecks the range.
*/
void KeyLayout::Set(uint8_t uiLayoutID)
{
  if (uiLayoutID > MAX_KEYB_LAYOUT_ID)
    uiLayoutID = 0;

  if (uiCurrentKeyboardLayout != uiLayoutID)
  {
    uiCurrentKeyboardLayout = uiLayoutID;
    Store();
    Print();
  }
}


/*! \brief Returns the current active layout.

*/
uint8_t KeyLayout::Get(void)
{
  return uiCurrentKeyboardLayout;
}

