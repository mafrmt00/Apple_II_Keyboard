/*! \file II_Encoder.cpp 
 * \brief Output ASCII in parallel form to Apple II keyboard connector.
 *
 *  Takes a ASCII character or string and sets the IO Pins accordingly for APPLE II keayboard Input.
 */

#include "Arduino.h"
#include "II_Encoder.h"

void II_Encoder::begin(void)
{
  pinMode(IIKEYB_D0, OUTPUT);
  pinMode(IIKEYB_D1, OUTPUT);
  pinMode(IIKEYB_D2, OUTPUT);
  pinMode(IIKEYB_D3, OUTPUT);
  pinMode(IIKEYB_D4, OUTPUT);
  pinMode(IIKEYB_D5, OUTPUT);
  pinMode(IIKEYB_D6, OUTPUT);

  pinMode(IIKEYB_RESET, INPUT_PULLUP);
  pinMode(IIKEYB_STRB, OUTPUT);

  digitalWrite(IIKEYB_D0, LOW);
  digitalWrite(IIKEYB_D1, LOW);
  digitalWrite(IIKEYB_D2, LOW);
  digitalWrite(IIKEYB_D3, LOW);
  digitalWrite(IIKEYB_D4, LOW);
  digitalWrite(IIKEYB_D5, LOW);
  digitalWrite(IIKEYB_D6, LOW);

  digitalWrite(IIKEYB_RESET, HIGH);
  digitalWrite(IIKEYB_STRB, HIGH);

}

void II_Encoder::Reset(void)
{
  pinMode(IIKEYB_RESET, OUTPUT);
  digitalWrite(IIKEYB_RESET, LOW);
  delay(10);
  pinMode(IIKEYB_RESET, INPUT_PULLUP);
  digitalWrite(IIKEYB_RESET, HIGH);

}

int II_Encoder::IIputchar(int bOutput)
{
  if (bOutput & 0b00000001)
    digitalWrite(IIKEYB_D0, HIGH);
  else
    digitalWrite(IIKEYB_D0, LOW);

  if (bOutput & 0b00000010)
    digitalWrite(IIKEYB_D1, HIGH);
  else
    digitalWrite(IIKEYB_D1, LOW);

  if (bOutput & 0b00000100)
    digitalWrite(IIKEYB_D2, HIGH);
  else
    digitalWrite(IIKEYB_D2, LOW);

  if (bOutput & 0b00001000)
    digitalWrite(IIKEYB_D3, HIGH);
  else
    digitalWrite(IIKEYB_D3, LOW);

  if (bOutput & 0b00010000)
    digitalWrite(IIKEYB_D4, HIGH);
  else
    digitalWrite(IIKEYB_D4, LOW);

  if (bOutput & 0b00100000)
    digitalWrite(IIKEYB_D5, HIGH);
  else
    digitalWrite(IIKEYB_D5, LOW);

  if (bOutput & 0b01000000)
    digitalWrite(IIKEYB_D6, HIGH);
  else
    digitalWrite(IIKEYB_D6, LOW);

  pinMode(IIKEYB_STRB, HIGH);
  delay(1);
  pinMode(IIKEYB_STRB, LOW);

  delay(10);
  return (bOutput);
}

int II_Encoder::IIputs(const char *string)
{
  int i = 0;
  while (string[i]) //standard c idiom for looping through a null-terminated string
  {
    if (IIputchar(string[i]) == EOF) //if we got the EOF value from writing the char
    {
      return EOF;
    }
    i++;
  }

  if (IIputchar('\r') == EOF) //this will occur right after we quit due to the null terminated character.
  {
    return EOF;
  }
  return 1; //to meet spec.
}

char *II_Encoder::convert(unsigned int num, const int base, char *ptr)
{
  static char Representation[] = "0123456789ABCDEF";
  *ptr = '\0';

  do
  {
    *--ptr = Representation[num % base];
    num /= base;
  } while (num != 0);

  return (ptr);
}

void II_Encoder::IIprintf(char* format, ...)
{
  char *traverse;
  unsigned int i;
  char *s;
  char pBuffer[25];

  //Module 1: Initializing Myprintf's arguments
  va_list arg;
  va_start(arg, format);

  for (traverse = format; *traverse != '\0'; traverse++)
  {
    while ( *traverse != '%' )
    {
      IIputchar(*traverse);
      traverse++;
    }

    traverse++;

    //Module 2: Fetching and executing arguments
    switch (*traverse)
    {
      case 'c' : i = va_arg(arg, int);    //Fetch char argument
        IIputchar(i);
        break;

      case 'd' : i = va_arg(arg, int);        //Fetch Decimal/Integer argument
        if (i < 0)
        {
          i = -i;
          IIputchar('-');
        }
        IIputs(convert(i, 10, pBuffer));
        break;

      case 'o': i = va_arg(arg, unsigned int); //Fetch Octal representation
        IIputs(convert(i, 8, pBuffer));
        break;

      case 's': s = va_arg(arg, char *);      //Fetch string
        IIputs(s);
        break;

      case 'x': i = va_arg(arg, unsigned int); //Fetch Hexadecimal representation
        
        IIputs(convert(i, 16, pBuffer));
        break;
    }
  }

  //Module 3: Closing argument list to necessary clean-up
  va_end(arg);
}

