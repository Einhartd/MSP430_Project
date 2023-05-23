//  Buttons defines
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

//  INITIALIZE BUTTONS FUNCS
void initialize_buttons(void);
void initialize_encoder(void);
//  END OF INITIALIZE BUTTONS FUNCS

//  FUNC WHICH DETECTS PRESSED BUTTON
int check_pressed_button(void);