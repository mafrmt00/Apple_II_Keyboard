#define IIKEYB_D0 4
#define IIKEYB_D1 5
#define IIKEYB_D2 6
#define IIKEYB_D3 7
#define IIKEYB_D4 8
#define IIKEYB_D5 9
#define IIKEYB_D6 10

#define IIKEYB_RESET 11
#define IIKEYB_STRB 12

void setupIO()
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

void II_Reset(void)
{
  pinMode(IIKEYB_RESET, OUTPUT);
  pinMode(IIKEYB_RESET, LOW);
  delay(10);
  pinMode(IIKEYB_RESET, INPUT_PULLUP);
  digitalWrite(IIKEYB_RESET, HIGH);

}

int II_putchar(int bOutput)
{
  if (10 == bOutput)
    bOutput = '\r';

  if (0x0D == bOutput)
    bOutput = '\r';


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

int II_puts(const char *string)
{
  int i = 0;
  while (string[i]) //standard c idiom for looping through a null-terminated string
  {
    if ( II_putchar(string[i]) == EOF) //if we got the EOF value from writing the char
    {
      return EOF;
    }
    i++;
  }

  if (II_putchar('\r') == EOF) //this will occur right after we quit due to the null terminated character.
  {
    return EOF;
  }
  return 1; //to meet spec.
}

char *convert(unsigned int num, int base)
{
  static char Representation[] = "0123456789ABCDEF";
  static char buffer[50];
  char *ptr;

  ptr = &buffer[49];
  *ptr = '\0';

  do
  {
    *--ptr = Representation[num % base];
    num /= base;
  } while (num != 0);

  return (ptr);
}

void II_printf(char* format, ...)
{
  char *traverse;
  unsigned int i;
  char *s;

  //Module 1: Initializing Myprintf's arguments
  va_list arg;
  va_start(arg, format);

  for (traverse = format; *traverse != '\0'; traverse++)
  {
    while ( *traverse != '%' )
    {
      II_putchar(*traverse);
      traverse++;
    }

    traverse++;

    //Module 2: Fetching and executing arguments
    switch (*traverse)
    {
      case 'c' : i = va_arg(arg, int);    //Fetch char argument
        II_putchar(i);
        break;

      case 'd' : i = va_arg(arg, int);        //Fetch Decimal/Integer argument
        if (i < 0)
        {
          i = -i;
          II_putchar('-');
        }
        II_puts(convert(i, 10));
        break;

      case 'o': i = va_arg(arg, unsigned int); //Fetch Octal representation
        II_puts(convert(i, 8));
        break;

      case 's': s = va_arg(arg, char *);      //Fetch string
        II_puts(s);
        break;

      case 'x': i = va_arg(arg, unsigned int); //Fetch Hexadecimal representation
        II_puts(convert(i, 16));
        break;
    }
  }

  //Module 3: Closing argument list to necessary clean-up
  va_end(arg);
}



void setup()
{
  Serial.begin(115200);
  setupIO();
  Serial.println("Apple II Keyboard started");

  II_puts("   ");

  while (Serial.available() == 0) {}


  //II_monitor();
}

char* monitorcode[40];

void II_monitor(void)
{
  II_puts("call -151");
  delay(500);

  monitorcode[0] = "0: 00 0A 00 40 00 00 00 00";
  monitorcode[1] = "8: 00 00 00 00 00 00 00 00";
  monitorcode[2] = "800: A9 00 85 07 A9 00 A8 AA";
  monitorcode[3] = "808: 85 06 A5 00 85 04 A5 01";
  monitorcode[4] = "810: 85 05 C0 04 D0 04 A5 04";
  monitorcode[5] = "818: 85 06 C0 05 D0 04 A5 05";
  monitorcode[6] = "820: 85 06 A5 06 81 04 A1 04";
  monitorcode[7] = "828: C5 06 D0 2E E6 04 D0 02";
  monitorcode[8] = "830: E6 05 A5 02 C5 04 D0 04";
  monitorcode[9] = "838: A5 03 C5 05 D0 D4 A5 00";
  monitorcode[10] = "840: 85 04 A5 01 85 05 C0 04";
  monitorcode[11] = "848: D0 04 A5 04 85 06 C0 05";
  monitorcode[12] = "850: D0 04 A5 05 85 06 A1 04";
  monitorcode[13] = "858: C5 06 D0 69 E6 04 D0 02";
  monitorcode[14] = "860: E6 05 A5 02 C5 04 D0 04";
  monitorcode[15] = "868: A5 03 C5 05 D0 D8 C0 00";
  monitorcode[16] = "870: D0 08 A9 FF 85 06 C8 4C";
  monitorcode[17] = "878: 0A 08 C0 01 D0 04 A9 01";
  monitorcode[18] = "880: D0 F2 C0 02 D0 08 06 06";
  monitorcode[19] = "888: 90 ED A9 7F D0 E6 C0 03";
  monitorcode[20] = "890: D0 08 38 66 06 B0 E0 C8";
  monitorcode[21] = "898: D0 DD C0 04 F0 F9 A9 50";
  monitorcode[22] = "8A0: 20 EC 08 A9 41 20 EC 08";
  monitorcode[23] = "8A8: A9 53 20 EC 08 A9 53 20";
  monitorcode[24] = "8B0: EC 08 20 1E 09 E6 07 A5";
  monitorcode[25] = "8B8: 07 20 F2 08 20 13 09 EA";
  monitorcode[26] = "8C0: EA EA 4C 04 08 48 98 20";
  monitorcode[27] = "8C8: F2 08 20 1E 09 A5 05 20";
  monitorcode[28] = "8D0: F2 08 A5 04 20 F2 08 20";
  monitorcode[29] = "8D8: 1E 09 A5 06 20 F2 08 20";
  monitorcode[30] = "8E0: 1E 09 68 20 F2 08 20 13";
  monitorcode[31] = "8E8: 09 00 EA EA 09 80 20 F0";
  monitorcode[32] = "8F0: FD 60 48 4A 4A 4A 4A 29";
  monitorcode[33] = "8F8: 0F 09 30 C9 3A 90 02 69";
  monitorcode[34] = "900: 06 20 EC 08 68 29 0F 09";
  monitorcode[35] = "908: 30 C9 3A 90 02 69 06 20";
  monitorcode[36] = "910: EC 08 60 A9 0D 20 EC 08";
  monitorcode[37] = "918: A9 0A 20 EC 08 60 A9 20";
  monitorcode[38] = "920: 20 EC 08 60";
  //monitorcode[39] = "0: 00 E0 00 F0"; //Setup E000 to EFFF
  monitorcode[39] = "0: 00 40 00 40"; //Setup 4000 to 7FFF

  for (int i = 0; i < 40; i++)
  {
    II_puts(monitorcode[i]);
    delay(50);
  }
}

void loop()
{
  int incomingByte = 0;
  int II_Byte = 0;

  while (Serial.available() == 0) {}
  incomingByte = Serial.read();

  II_Byte = incomingByte;

  if (10 == incomingByte)
    II_Byte = '\r';

  if (0x0D == incomingByte)
    II_Byte = '\r';

  if (28 == incomingByte)
    II_Byte = 0x08;

  if (29 == incomingByte)
    II_Byte = 0x15;

#if 0
  Serial.print("Received Value: 0x");
  Serial.print(incomingByte, HEX);
  Serial.print(" Wich shows as '");
  Serial.write(incomingByte);
  Serial.print("'. Sent as: 0x");
  Serial.print(II_Byte, HEX);
  Serial.println("' to Apple II");
#endif
  Serial.write(II_Byte);

  II_putchar(II_Byte);

}
