//  SCREEN FUNCS

  //  Init funcs

    //  Function which makes delay mainly for screen
void delay(int length);

    //  set_bits for screen
void set_bits(int bit0, int bit5, int bit4, int bit3, int bit2);

    //  Initialize display screen
void initDisplay(void);

    //  Clear display
void clearDisplay(void);


  //  Sending funcs

    // Function which sends single characters to the screen
void sendChar(char c);

    // Function which sends numbers to the screen 00-99
void sendNumber(int a);

    // Function which sends big numbers to the screen 0000-9999
void sendBigNumber(int a);

    // Function which sends strings to the screen
void sendStr(char str[]);



//  END OF SCREEN FUNCS