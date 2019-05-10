/*! \file backlog.h
 * \brief Provides BASH like command backlog.
 *
 *  With the Arrow Up / Down keys the last commands can be recalled and used.
 */
#pragma once

#define BUFFER_STEPS 40
#define BACKLOG_STEPS 10
#define CHAR_OUT_DELAY 10
 
class Backlog
{
  private:
    uint8_t BacklogOutputPointer;

    char* StringBacklog[BACKLOG_STEPS];
    char* pCurrentInputString;
    void IncreaseBuffer(void);
    void BacklogCurrString(void);

    int (*pputs)(const char *string);

    void EraseCurrent(void);
    void OutputCurrent(void);

    void ChangeToIndex(uint8_t uiNewIndex);

  public:
    Backlog();
    void addchar(char c);
    void SetPuts(int (*pputs_funcp)(const char *string));
    void OneBack(void);
    void OneForeward(void);
};
