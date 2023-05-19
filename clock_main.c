#include "io430.h"


#define BUTTON0 0
#define BUTTON1 1
#define BUTTON2 2
#define BUTTON3 3
#define ENCODER_LEFT 4  //To check
#define ENCODER_RIGHT 5 //To check

#define RESET_INTERRUPT_FLAG 0x00
#define BUTTON_DIR 0x00
#define BUTTON_INTERRUPT 0x08
#define ENCODER_INTERRUPT 0x02

static int menu =0;
static int ustawienia =0;
static int rok =2000;
static int miesiac =0;
static int dzien =0;
static int godzina =0;
static int minuta =0;

void set_time(int button);
void set_ustawienia(void);
void confirm_ustawienia(void);
void initialize_buttons(void);
void initialize_encoder(void);
int check_pressed_button(void);
void test_the_buttons(int button);
void sendChar(char c);


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



void sendChar(char c)
{
  //char -> lista bitów
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

void clearDisplay()
{
  set_bits(0,0,0,0,0);
  set_bits(0,0,0,0,1);
  
  set_bits(0,0,0,0,0);
  set_bits(0,0,1,1,0);
}

int main(void)
{
  //    Testowe wyjscie do sprawdzania peryferiow
  P2OUT = 0x00;
  P2DIR = 0xFF; //ustawniamy P2 jako port wyjsciowy

  P1IFG = RESET_INTERRUPT_FLAG;         //zerujemy flagi przerwan

  initialize_buttons();
  initialize_encoder();
  initDisplay();
  clearDisplay();
  sendStr("menu glowne");
  


  WDTCTL = (WDTPW | WDTHOLD);                    // Stop watchdog timer

  __bis_SR_register( GIE );                    // Enable global interrupts

  while( 1 )                                   // Endless loop - main program
  {;}
}


//  Wektor przerwania dla przyciskow
#pragma vector=PORT1_VECTOR
__interrupt void port1_int(void)
{
  //  Sprawdzenie, ktory przycisk jest wcisniety
  int button = check_pressed_button();

  //  Wykonanie instrukcji z danego przycisku
  test_the_buttons(button);

  P1IFG = RESET_INTERRUPT_FLAG; //zerujemy flagi przerwan od enkodera i przyciskow
}


void initialize_buttons(void){
  P1DIR = BUTTON_DIR;         // Ustawienie trybu wejsca przyciskow
  P1IE = BUTTON_INTERRUPT;    // przerwania od przyciskow (maska)
  P1IES = BUTTON_INTERRUPT;   // przerwania od przyciskow
}

void initialize_encoder(void){
  P1IE |= ENCODER_INTERRUPT;          //sciagniecie maski przerwan od enkodera
  P1IES |= ENCODER_INTERRUPT;         //wybieramy reakcje na zbocze opadajace
}

int check_pressed_button(void){
  if (P4IN_bit.P3==0)
  {
    return BUTTON0;
  }
  else if (P4IN_bit.P2==0)
  {
    return BUTTON1;
  }
  else if (P4IN_bit.P1==0)
  {
    return BUTTON2;
  }
  else if (P4IN_bit.P0==0)
  {
    return BUTTON3;
  }
  else
  {
    if(P1IN_bit.P2==0)
    {
      return ENCODER_LEFT;
    }
    else
    {
      return ENCODER_RIGHT;
    }
  }
}

void test_the_buttons(int button){
  switch (button)
  {
  case BUTTON0:
    if (menu ==0){ // Menu Glowne -> Menu StoperA
      menu =1;
      clearDisplay();
      sendStr("stoper");
    }
    else if (menu ==1){ // Menu Stopera -> Start Stopera
      clearDisplay();
      sendStr("start stopera");
    }
    else if (menu ==2){ // Menu Timera-> Ustawinie Timera
      //wcisnieco tego przyciksu stopuje i zeruje timer
      clearDisplay();
      sendStr("ustawienie timera");
    }
    else if (menu ==3){ // Ustawienia-> Ustawinie godziny/minuty/etc
      clearDisplay();
      sendStr("ustawienie czasau");
      set_ustawienia();
      
      
    }
    break;
  
  case BUTTON1:
    if (menu ==0){ // Menu Glowne -> Menu Timera       
      menu =2;
      clearDisplay();
      sendStr("timer");
    }
    else if (menu ==1){ // Menu Stopera -> Stop Stopera
      clearDisplay();
      sendStr("stop stopera");
    }
    else if (menu ==2){ // Menu Timera-> Start Timera
      clearDisplay();
      sendStr("start timera");
    }

    break;

  case BUTTON2:
    if (menu ==0){ // Menu Glowne -> Ustwinie godziny     
      menu =3;
      clearDisplay();
      sendStr("ustawienia");
    }
    else if (menu ==1){ // Menu Stopera -> Start Stopera
      clearDisplay();
      sendStr("reset stopera");
    }
    else if (menu ==2){ // Menu Timera-> Stop Timera
      clearDisplay();
      sendStr("stop timera");
    }

    break;

  case BUTTON3:
      menu =0;
      clearDisplay();
      sendStr("menu glowne");
    break;
  
  case ENCODER_LEFT:
    set_time(button);
    break;
  
  case ENCODER_RIGHT:
    set_time(button);
    break;

  default:
    break;
  }
}

void set_ustawienia(){
  if (ustawienia < 5){
    ustawienia++;
  }
  else{
    ustawienia =0;
  }
 confirm_ustawienia();
}
      
void confirm_ustawienia(){
  switch (ustawienia){
     case 0:
      menu =0;
      clearDisplay();
      sendBigNumber(rok);
      sendChar(':');
      sendNumber(miesiac);
      sendChar(':');
      sendNumber(dzien);
      sendChar(':');
      sendNumber(godzina);
      sendChar(':');
      sendNumber(minuta);
      break;
    case 1:
      clearDisplay();
      sendStr("rok");
      break;
    case 2:
      clearDisplay();
      sendStr("miesiac");
      break;
    case 3:
      clearDisplay();
      sendStr("dzien");
      break;
    case 4:
      clearDisplay();
      sendStr("godzina");
      break;
    case 5:
      clearDisplay();
      sendStr("minuta");
      break;
    default:
      break;
  }
  
}

void set_time(int button){
  switch (ustawienia){
     case 0:
      break;
    case 1:
      if (button == ENCODER_LEFT && rok > 0){
        rok--;
        clearDisplay();
        sendBigNumber(rok);
        sendChar(':');
        sendNumber(miesiac);
        sendChar(':');
        sendNumber(dzien);
        sendChar(':');
        sendNumber(godzina);
        sendChar(':');
        sendNumber(minuta);
      }
      else if (button == ENCODER_RIGHT && rok < 3000){
        rok++;
        clearDisplay();
        sendBigNumber(rok);
        sendChar(':');
        sendNumber(miesiac);
        sendChar(':');
        sendNumber(dzien);
        sendChar(':');
        sendNumber(godzina);
        sendChar(':');
        sendNumber(minuta);
      }
      else{
        break;
      }
      break;
    case 2:
      if (button == ENCODER_LEFT && miesiac > 0){
        miesiac--;
        clearDisplay();
        sendBigNumber(rok);
        sendChar(':');
        sendNumber(miesiac);
        sendChar(':');
        sendNumber(dzien);
        sendChar(':');
        sendNumber(godzina);
        sendChar(':');
        sendNumber(minuta);
      }
      else if (button == ENCODER_RIGHT && miesiac < 12){
        miesiac++;
        clearDisplay();
        sendBigNumber(rok);
        sendChar(':');
        sendNumber(miesiac);
        sendChar(':');
        sendNumber(dzien);
        sendChar(':');
        sendNumber(godzina);
        sendChar(':');
        sendNumber(minuta);
      }
      else{
        break;
      }
      break;
    case 3:
      if (button == ENCODER_LEFT && dzien > 0){
        dzien--;
        clearDisplay();
        sendBigNumber(rok);
        sendChar(':');
        sendNumber(miesiac);
        sendChar(':');
        sendNumber(dzien);
        sendChar(':');
        sendNumber(godzina);
        sendChar(':');
        sendNumber(minuta);
      }
      else if (button == ENCODER_RIGHT && dzien < 31){
        dzien++;
        clearDisplay();
        sendBigNumber(rok);
        sendChar(':');
        sendNumber(miesiac);
        sendChar(':');
        sendNumber(dzien);
        sendChar(':');
        sendNumber(godzina);
        sendChar(':');
        sendNumber(minuta);
      }
      else{
        break;
      }
      break;
    case 4:
      if (button == ENCODER_LEFT && godzina > 0){
        godzina--;
        clearDisplay();
        sendBigNumber(rok);
        sendChar(':');
        sendNumber(miesiac);
        sendChar(':');
        sendNumber(dzien);
        sendChar(':');
        sendNumber(godzina);
        sendChar(':');
        sendNumber(minuta);
      }
      else if (button == ENCODER_RIGHT && godzina < 23){
        godzina++;
        clearDisplay();
        sendBigNumber(rok);
        sendChar(':');
        sendNumber(miesiac);
        sendChar(':');
        sendNumber(dzien);
        sendChar(':');
        sendNumber(godzina);
        sendChar(':');
        sendNumber(minuta);
      }
      else{
        break;
      }
      break;
    case 5:
      if (button == ENCODER_LEFT && minuta > 0){
        minuta--;
        clearDisplay();
        sendBigNumber(rok);
        sendChar(':');
        sendNumber(miesiac);
        sendChar(':');
        sendNumber(dzien);
        sendChar(':');
        sendNumber(godzina);
        sendChar(':');
        sendNumber(minuta);
      }
      else if (button == ENCODER_RIGHT && minuta < 59){
        minuta++;
        clearDisplay();
        sendBigNumber(rok);
        sendChar(':');
        sendNumber(miesiac);
        sendChar(':');
        sendNumber(dzien);
        sendChar(':');
        sendNumber(godzina);
        sendChar(':');
        sendNumber(minuta);
      }
      else{
        break;
      }
      break;
    default:
      break;
  }
  }

void display_time(){
    sendBigNumber(rok);
    sendChar(':');
    sendNumber(miesiac);
    sendChar(':');
    sendNumber(dzien);
    sendChar(':');
    sendNumber(godzina);
    sendChar(':');
    sendNumber(minuta);
}