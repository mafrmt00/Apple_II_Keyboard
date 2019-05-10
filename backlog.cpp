#include "Arduino.h"

#include "backlog.h"

/*! \brief Constructor, init everything to zero.

*/
Backlog::Backlog()
{
  pCurrentInputString = NULL;
  pputs = NULL;
  BacklogOutputPointer = 0;

  for (int i = 0; i < BACKLOG_STEPS; i++)
    StringBacklog[i] = NULL;
}

/*! \brief Set the puts callback.

   Set the puts callback function, in our case the output to the II keyb interface.
*/
void Backlog::SetPuts(int (*pputs_funcp)(const char *string))
{
  pputs = pputs_funcp;
}

/*! \brief Erase the current input on the screen.

   The current typed input is erased with backspaces.
*/
void Backlog::EraseCurrent(void)
{
  uint16_t bufferlen = strlen(pCurrentInputString);

  for (int i = 0; i < bufferlen; i++)
  {
    if (pputs)
    {
      pputs(0x08); //Backspace
      delay(CHAR_OUT_DELAY);
    }
  }
}

/*! \brief Output the current string vie puts.

   The current active string ist typed to the screen via puts.
*/
void Backlog::OutputCurrent(void)
{
  uint16_t bufferlen = strlen(pCurrentInputString);

  for (int i = 0; i < bufferlen; i++)
  {
    if (pputs)
    {
      pputs(pCurrentInputString[i]);
      delay(CHAR_OUT_DELAY);
    }
  }
}

/*! \brief Go one string back in the backlog.

   Erases the current input from the screen and goes one back ion the log.
*/
void Backlog::OneBack(void)
{
  uint8_t uiNewIndex = BacklogOutputPointer;

  ChangeToIndex(uiNewIndex);

  uiNewIndex++;

  if (uiNewIndex > BACKLOG_STEPS - 1)
    uiNewIndex = BACKLOG_STEPS - 1;

  BacklogOutputPointer = uiNewIndex;
}

/*! \brief Go one string forward in the backlog.

   Erases the current input from the screen and goes one foreward ion the log.
*/
void Backlog::OneForeward(void)
{
  uint8_t uiNewIndex = BacklogOutputPointer;

  if (uiNewIndex)
    uiNewIndex--;

  ChangeToIndex(uiNewIndex);

  BacklogOutputPointer = uiNewIndex;
}

/*! \brief Change the screen to a specific index from the backlog.

   Changes the active display to a specific string from the backlog.
*/
void Backlog::ChangeToIndex(uint8_t uiNewIndex)
{
  EraseCurrent();
  delete pCurrentInputString;
  if (StringBacklog[uiNewIndex])
  {
    pCurrentInputString = new char[BUFFER_STEPS + (BUFFER_STEPS * (strlen(StringBacklog[uiNewIndex]) / BUFFER_STEPS)) ];
    strcpy(pCurrentInputString, StringBacklog[uiNewIndex]);
    OutputCurrent();
  }
}

/*! \brief Stores the current string in the backlog.

   The current string is stored in the backlog and the log is advanced one step.
*/
void Backlog::BacklogCurrString(void)
{
  BacklogOutputPointer = 0;

  if (StringBacklog[BACKLOG_STEPS - 1])
    delete StringBacklog[BACKLOG_STEPS - 1];

  for (int i = BACKLOG_STEPS - 1; i > 0; i--)
  {
    StringBacklog[i] = StringBacklog[i - 1];
  }

  StringBacklog[0] = pCurrentInputString;
  pCurrentInputString = NULL;
#if 0
  Serial.println("Current Backlog:");
  for (int i = 0; i < BACKLOG_STEPS; i++)
  {
    if (StringBacklog[i])
      Serial.println(StringBacklog[i]);
  }
#endif
}

/*! \brief If the current typed string exceeds BUFFER_STEPS the buffer is increased.
 * 
   If the current string gets bigger than BUFFER_STEPS another BUFFER_STEPS is added.
*/
void Backlog::IncreaseBuffer(void)
{
  if (NULL == pCurrentInputString)
  {
    pCurrentInputString = new char[BUFFER_STEPS];
    memset(pCurrentInputString, 0, BUFFER_STEPS);
  }
  else
  {
    uint16_t bufferlen = strlen(pCurrentInputString);
    if (((bufferlen % BUFFER_STEPS) == (BUFFER_STEPS - 2)) && (bufferlen > 0))
    {
      char *pnewCurrentInputString = new char[bufferlen + BUFFER_STEPS];
      memcpy(pnewCurrentInputString, pCurrentInputString, bufferlen + 1);
      delete [] pCurrentInputString;
      pCurrentInputString = pnewCurrentInputString;
    }
  }
}

/*! \brief Add one typed char to the current input string.
 * 
   On char is added, or if 0x0d is received, the string is stored and the backlog advanced.
*/
void Backlog::addchar(char c)
{

  IncreaseBuffer();

  if (0x0d == c) // Finished
  {
    BacklogCurrString();
  }
  else if (0x08 == c) // Backspace)
  {
    uint16_t bufferlen = strlen(pCurrentInputString);
    if (bufferlen)
      pCurrentInputString[bufferlen - 1] = 0x00;
  }
  else
  {
    uint16_t bufferlen = strlen(pCurrentInputString);

    pCurrentInputString[bufferlen] = c;
    pCurrentInputString[bufferlen + 1] = 0x00;
  }
}
