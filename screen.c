void delay(int length){
  int a = length;
  while(a != 0){
    a--;
  }
}

void set_bits(int bit0, int bit5, int bit4, int bit3, int bit2){
  P2OUT_bit.P1 = 1;
  P2OUT_bit.P0 = bit0;
  P2OUT_bit.P5 = bit5;
  P2OUT_bit.P4 = bit4;
  P2OUT_bit.P3 = bit3;
  P2OUT_bit.P2 = bit2;
  P2OUT_bit.P1 = 0;
  delay(100);
}

void initDisplay()
{
  set_bits(0,0,0,1,1);
  //delay(1000);
  set_bits(0,0,0,1,1);
  //delay(100);
  set_bits(0,0,0,1,1);
  
  set_bits(0,0,0,1,0);
  
  set_bits(0,0,0,1,0);
  set_bits(0,1,1,0,0);
  
  set_bits(0,0,0,0,0);
  set_bits(0,1,0,0,0);
  
  set_bits(0,0,0,0,0);
  set_bits(0,0,0,0,1);
  
  set_bits(0,0,0,0,0);
  set_bits(0,0,1,1,0);
  
  set_bits(0,0,0,0,0);
  set_bits(0,1,1,1,1);
}

void clearDisplay()
{
  set_bits(0,0,0,0,0);
  set_bits(0,0,0,0,1);
  
  set_bits(0,0,0,0,0);
  set_bits(0,0,1,1,0);
}

void sendNumber(int a)
{

  int tens = a/10;
  int ones = a-10*tens;
  char o = (char)(ones+48);
  char t = (char)(tens+48);
  sendChar(t);
  sendChar(o);

}

void sendBigNumber(int a)
{
  int thousands =a/1000;
  int hundreds = (a-1000*thousands)/100 ;
  int tens = (a-1000*thousands-100*hundreds)/10;
  int ones = a -1000*thousands-100*hundreds- 10*tens;

  char o = (char)(ones+48);
  char t = (char)(tens+48);
  char h = (char)(hundreds+48);
  char th = (char)(thousands+48);
  
  
  sendChar(th);
  sendChar(h);
  sendChar(t);
  sendChar(o);

}

void sendChar(char c)
{
  //char -> lista bitow
    int output[8];
    for (int i = 7; i > -1; --i) {
      int a = (c >> i) & 1;
      output[7-i] = a;
    }
    set_bits(1,output[0],output[1],output[2],output[3]);
    set_bits(1,output[4],output[5],output[6],output[7]);
}

void sendStr(char str[])
{
  int i = 0;
  while(str[i]!='\0')
  {
    sendChar(str[i]);
    i++;
  }
}



//  End of screen functions