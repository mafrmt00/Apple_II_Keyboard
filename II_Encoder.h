#pragma once

//Arduino Pin definitioin for Keyboard Interface
#define IIKEYB_D0 A2
#define IIKEYB_D1 A3
#define IIKEYB_D2 A0
#define IIKEYB_D3 A1
#define IIKEYB_D4 7
#define IIKEYB_D5 6
#define IIKEYB_D6 8

#define IIKEYB_RESET 5
#define IIKEYB_STRB 4


class II_Encoder
{
  public:
    void begin(void);
    void Reset(void);
    int IIputchar(int bOutput);
    int IIputs(const char *string);
    void IIprintf(char* format, ...);
  
    char *convert(unsigned int num, const int base, char *ptr);
};

