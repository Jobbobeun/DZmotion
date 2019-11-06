/* Project: DZ Motion 2020
   Author:  Job Heijlighen
   Date:    03-11-19
   Version: 1.0
*/

#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>


#define frequency_invertor_1_on_off 22
#define frequency_invertor_1_direction 24
#define frequency_invertor_1_speed 2
#define frequency_invertor_2_on_off 26
#define frequency_invertor_2_direction 28
#define frequency_invertor_2_speed 3
#define frequency_invertor_3_on_off 30
#define frequency_invertor_3_direction 32
#define frequency_invertor_3_speed 4

#define hydraulic_valve_1_open 34
#define hydraulic_valve_1_close 36
#define hydraulic_valve_1_time 2000
#define hydraulic_valve_2_open 38
#define hydraulic_valve_2_close 40
#define hydraulic_valve_2_time 2000
#define hydraulic_valve_3_open 42
#define hydraulic_valve_3_close 44
#define hydraulic_valve_3_time 2000

#define button_up 31
#define button_down 33
#define button_select 35
#define button_stop 37
#define up 0
#define down 1
#define select 2
#define Stop 3
#define cylinder_amount 3

// debug defines
//#define DEBUG_HYDRAULIC
//#define DEBUG_DETERMINE_ARRAYS
//#define DEBUG_MANUAL_CONTROLE

// Global variables
int frequency_invertor_on_off[4];
int frequency_invertor_direction[4];
int frequency_invertor_state[4];
int frequency_invertor_state_direction[4];

int hydraulic_open[4];
int hydraulic_close[4];
int hydraulic_state[4];
int hydraulic_time[4];

int button_state[4];
int button_last_state[4];
int button[4];

// variable check LCD needs to be updated
int LCD_STATE_OLD;
int LCD_SUB_RUN_OLD;
int LCD_SUB_MANUAL_HYDRAULIC_OLD;
bool LCD_SUB_MANUAL_DIRECTION;
int LCD_WELCOME_DZ_COUNTER;
bool LCD_WELCOME_DZ_FLASH;
bool LCD_WELCOME_DZ_FLASH_RESET = true;

// LCD variables
int sub_manual_cylinder_nr = 1;

bool first_update_lcd = true;
bool AUTOMATIC_CYCLE_START = false;
int AUTOMATIC_CYCLE_COUNTER;


// Enum state machine
enum LCD_STATE_ENUM {
  LCD_STATE_WELCOME,
  LCD_STATE_START,
  LCD_STATE_MANUAL_HYDRAULIC,
  LCD_STATE_MANUAL_FREQUENCY_INVERTER,
  LCD_STATE_SETTINGS
};

enum LCD_SUB_START_ENUM {
  SUB_RUN_IDLE,
  SUB_RUN_START,
  SUB_RUN_STOP
};

enum LCD_SUB_MANUAL_HYDRAULIC_ENUM{
  SUB_MANUAL_HYDRAULIC_IDLE,
  SUB_MANUAL_HYDRAULIC_SET,
  SUB_MANUAL_HYDRAULIC_RUN
};

enum MASTER_STATE_ENUM {
  MASTER_STATE_IDLE,
  MASTER_STATE_START,
  MASTER_STATE_CYCLE_1,
  MASTER_STATE_CYCLE_2,
  MASTER_STATE_CYCLE_3,
  MASTER_STATE_STOP
};


enum MANUAL_CONTROL_VALVE_STATE_ENUM{
  MANUAL_CONTROLE_VALVE_IDLE,
  MANUAL_CONTROLE_VALVE_OUT,
  MANUAL_CONTROLE_VALVE_IN,
  MANUAL_CONTROLE_VALVE_STOP
};

LCD_SUB_MANUAL_HYDRAULIC_ENUM LCD_SUB_MANUAL_HYDRAULIC;
LCD_STATE_ENUM LCD_STATE;
LCD_SUB_START_ENUM LCD_SUB_RUN;
MASTER_STATE_ENUM MASTER_STATE;
MANUAL_CONTROL_VALVE_STATE_ENUM MANUAL_CONTROLE_VALVE_STATE;

LiquidCrystal_PCF8574 lcd(0x27);// SDA --> D20 SCL --> D21 5v --> 5v GND --> GND

/************************************************************************************/
void setup()
/************************************************************************************/
{

  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(0x27); //Your LCD Address
  lcd.begin(20, 4); // initialize the lcd
  lcd_start();
  lcd.print("Loading");
  LCD_LIGHT_ON();

  update_lcd();
  determine_arrays();

}

/************************************************************************************/
void loop()
/************************************************************************************/
{

  check_buttons();
  update_lcd();
  cycle_state();
 manual_control_valve();
 
}

/************************************************************************************/
void determine_arrays()   // Set all pins to array new outputs & inputs needs to be add here.
/************************************************************************************/
{

#if defined(DEBUG_DETERMINE_ARRAYS)
  Serial.println("set hydraulic open");
#endif

  // set hydraulic open
  hydraulic_open[1] = hydraulic_valve_1_open;
  hydraulic_open[2] = hydraulic_valve_2_open;
  hydraulic_open[3] = hydraulic_valve_3_open;

  for (int i = 1 ; i < 4 ; i++) {
    pinMode(hydraulic_open[i], OUTPUT);
    //digitalWrite(hydraulic_open[i],HIGH);
  }

#if defined(DEBUG_DETERMINE_ARRAYS)
  Serial.println("set hydraulic close");
#endif

  // set hydraulic close
  hydraulic_close[1] = hydraulic_valve_1_close;
  hydraulic_close[2] = hydraulic_valve_2_close;
  hydraulic_close[3] = hydraulic_valve_3_close;

  for (int i = 1 ; i < 4 ; i++) {
    pinMode(hydraulic_close[i], OUTPUT);
    // digitalWrite(hydraulic_close[i],HIGH);
  }

#if defined(DEBUG_DETERMINE_ARRAYS)
  Serial.println("set hydraulic time");
#endif

  // set hydraulic time
  hydraulic_time[1] = hydraulic_valve_1_time;
  hydraulic_time[2] = hydraulic_valve_2_time;
  hydraulic_time[2] = hydraulic_valve_2_time;

#if defined(DEBUG_DETERMINE_ARRAYS)
  Serial.println("set frequency invertor on");
#endif

  // set frequency invertor on
  frequency_invertor_on_off[1] = frequency_invertor_1_on_off;
  frequency_invertor_on_off[2] = frequency_invertor_2_on_off;
  frequency_invertor_on_off[3] = frequency_invertor_3_on_off;


  for (int i = 1 ; i < 4 ; i++) {
    pinMode(frequency_invertor_on_off[i], OUTPUT);
    //  digitalWrite(frequency_invertor_on_off[i], HIGH);
  }

#if defined(DEBUG_DETERMINE_ARRAYS)
  Serial.println("set frequency invertor direction");
#endif

  // set frequency invertor direction
  frequency_invertor_direction[1] = frequency_invertor_1_direction;
  frequency_invertor_direction[2] = frequency_invertor_2_direction;
  frequency_invertor_direction[3] = frequency_invertor_3_direction;

  for (int i = 1 ; i < 4 ; i++) {
    pinMode(frequency_invertor_direction[i], OUTPUT);
    //  digitalWrite(frequency_invertor_direction[i], HIGH);
  }

#if defined(DEBUG_DETERMINE_ARRAYS)
  Serial.println("set buttons");
#endif

  // set button
  button[up] = button_up;
  button[down] = button_down;
  button[select] = button_select;
  button[Stop] = button_stop;

  for (int i = 0 ; i < 4 ; i++) {
    pinMode(button[i], INPUT);
  }

}

/************************************************************************************/
void update_eeprom()
/************************************************************************************/
{

}
