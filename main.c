#include "io430.h"
#include "screen.h"
#include "buttons.h"


//  Time defines
#define DEFAULT_MENU 0
#define DEFAULT_SETTINGS 0
#define DEFAULT_YEAR 2000
#define DEFAULT_MONTH 0
#define DEFAULT_DAY 0
#define DEFAULT_HOUR 0
#define DEFAULT_MINUTE 0

//  Static time
static int menu = DEFAULT_MENU;
static int settings = DEFAULT_SETTINGS;
static int year = DEFAULT_YEAR;
static int month = DEFAULT_MONTH;
static int day = DEFAULT_DAY;
static int hour = DEFAULT_HOUR;
static int minute = DEFAULT_MINUTE;

void set_time(int button);
void set_settings(void);
void confirm_settings(void);

void test_the_buttons(int button);


// Function which displays time on the screen
void display_time(void);

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

// Screen functions

void test_the_buttons(int button){
  switch (button)
  {
  case BUTTON0:
    if (menu==0){ // Menu Glowne -> Menu StoperA
      menu = 1;
      clearDisplay();
      sendStr("stoper");
    }
    else if (menu==1){ // Menu Stopera -> Start Stopera
      clearDisplay();
      sendStr("start stopera");
    }
    else if (menu==2){ // Menu Timera-> Ustawinie Timera
      //wcisnieco tego przyciksu stopuje i zeruje timer
      clearDisplay();
      sendStr("ustawienie timera");
    }
    else if (menu==3){ // Ustawienia-> Ustawinie godziny/minuty/etc
      clearDisplay();
      sendStr("ustawienie czasau");
      set_ustawienia();
      
      
    }
    break;
  
  case BUTTON1:
    if (menu==0){ // Menu Glowne -> Menu Timera       
      menu = 2;
      clearDisplay();
      sendStr("timer");
    }
    else if (menu==1){ // Menu Stopera -> Stop Stopera
      clearDisplay();
      sendStr("stop stopera");
    }
    else if (menu==2){ // Menu Timera-> Start Timera
      clearDisplay();
      sendStr("start timera");
    }
    break;

  case BUTTON2:
    if (menu==0){ // Menu Glowne -> Ustwinie godziny     
      menu = 3;
      clearDisplay();
      sendStr("ustawienia");
    }
    else if (menu==1){ // Menu Stopera -> Start Stopera
      clearDisplay();
      sendStr("reset stopera");
    }
    else if (menu==2){ // Menu Timera-> Stop Timera
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

void set_settings(){
  if (settings < 5){
    settings++;
  }
  else{
    settings = 0;
  }
 confirm_settings();
}
      
void confirm_settings(){
  switch (settings){
     case 0:
      menu = 0;
      clearDisplay();
      display_time();
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
  switch (settings){

     case 0:
      break;

    case 1:
      if (button == ENCODER_LEFT && year > 0){
        year--;
        clearDisplay();
        display_time();
      }
      else if (button == ENCODER_RIGHT && year < 3000){
        year++;
        clearDisplay();
        display_time();
      }
      else{
        break;
      }
      break;

    case 2:
      if (button == ENCODER_LEFT && month > 0){
        month--;
        clearDisplay();
        display_time();
      }
      else if (button == ENCODER_RIGHT && month < 12){
        month++;
        clearDisplay();
        display_time();
      }
      else{
        break;
      }
      break;

    case 3:
      if (button == ENCODER_LEFT && day > 0){
        day--;
        clearDisplay();
        display_time();
      }
      else if (button == ENCODER_RIGHT && day < 31){
        day++;
        clearDisplay();
        display_time();
      }
      else{
        break;
      }
      break;

    case 4:
      if (button == ENCODER_LEFT && hour > 0){
        hour--;
        clearDisplay();
        display_time();
      }
      else if (button == ENCODER_RIGHT && hour < 23){
        hour++;
        clearDisplay();
        display_time();
      }
      else{
        break;
      }
      break;

    case 5:
      if (button == ENCODER_LEFT && minute > 0){
        minute--;
        clearDisplay();
        display_time();
      }
      else if (button == ENCODER_RIGHT && minute < 59){
        minute++;
        clearDisplay();
        display_time();
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