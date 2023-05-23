#include "buttons.h"

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

void initialize_encoder(void){
  P1IE |= ENCODER_INTERRUPT;          //sciagniecie maski przerwan od enkodera
  P1IES |= ENCODER_INTERRUPT;         //wybieramy reakcje na zbocze opadajace
}

void initialize_buttons(void){
  P1DIR = BUTTON_DIR;         // Ustawienie trybu wejsca przyciskow
  P1IE = BUTTON_INTERRUPT;    // przerwania od przyciskow (maska)
  P1IES = BUTTON_INTERRUPT;   // przerwania od przyciskow
}



