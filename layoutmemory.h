/*! \file layoutmemory.h
 * \brief Handles selection of multiple keyboard layout.
 *
 *  Toggles the keyboard layout WIN+SPace as a trigger e.g. and stores the current layout in eeprom.
 */
 #pragma once
 
class KeyLayout
{
  public:
    KeyLayout();
    void Init(void);
    void Restore(void);
    void Store(void);

    void Print(void);

    void Toggle(void);
    void Set(uint8_t uiLayoutID);
    uint8_t Get(void);

  private:
    uint8_t uiCurrentKeyboardLayout;//ID of the current active keyboard Layout

};

