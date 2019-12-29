/* Project: DZ Motion 2020
   Author:  Job Heijlighen
   Date:    03-11-19
   Version: 1.0
*/

#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include <EEPROM.h>

// Define pins
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
#define hydraulic_valve_2_open 38
#define hydraulic_valve_2_close 40

#define hydraulic_valve_3_open 42
#define hydraulic_valve_3_close 44

#define relay_1 46
#define relay_2 48
#define relay_3 50
#define relay_4 52

#define small_motor_system 53
#define stewardplatform 51
#define empty_relay 49

#define button_up 31
#define button_down 33
#define button_select 35
#define button_stop 37
#define button_pause 39

#define controle_light_run 47
#define controle_light_stop 45
#define controle_light_pause 43

// Define software numbers
#define up 0
#define down 1
#define select 2
#define Stop 3
#define pause 4
#define cylinder_amount 3
#define frequency_inverter_amount 3
#define large_number_time_delay 6
#define settings_accuracy 500
#define other_components_amount 7

// define eeprom addresses
#define eeprom_automatic_homeing_time 0 // 4 positions needed
#define eeprom_hydraulic_valve_1_time 4 // 4 positions needed
#define eeprom_hydraulic_valve_2_time 8 // 4 positions needed
#define eeprom_hydraulic_valve_3_time 12 // 4 positions needed
#define eeprom_frequency_invertor_startup_delay 16 // 4 positions needed
#define eeprom_automatic_mode_pause_time 20 // 4 positions needed
#define eeprom_automatic_mode_hydraulic_1 24 // 1 position needed
#define eeprom_automatic_mode_hydraulic_2 25 // 1 position needed
#define eeprom_automatic_mode_hydraulic_3 26 // 1 position needed
#define eeprom_automatic_mode_frequency_1 27 // 1 position needed
#define eeprom_automatic_mode_frequency_2 28 // 1 position needed
#define eeprom_automatic_mode_frequency_3 29 // 1 position needed
#define eeprom_automatic_mode_other_relay_1 30 // 1 position needed
#define eeprom_automatic_mode_other_relay_2 31 // 1 position needed
#define eeprom_automatic_mode_other_relay_3 32 // 1 position needed
#define eeprom_automatic_mode_other_relay_4 33 // 1 position needed
#define eeprom_automatic_mode_other_platform 34 // 1 position needed
#define eeprom_automatic_mode_other_12V 35 // 1 position needed
#define eeprom_automatic_mode_other_empty 36 // 1 position needed
#define eeprom_automatic_mode_homing_enable 37 // 1 position needed
#define eeprom_automatic_mode_2020_pause_flashlight_timer_delay 38 // 4 positions needed
#define eeprom_automatic_mode_2020_pause_timer_delay 42 // 4 positions needed
#define eeprom_automatic_mode_2020_startup_delay 46 // 4 positions needed

// default values
#define hydraulic_valve_1_time_default 30000
#define hydraulic_valve_2_time_default 30000
#define hydraulic_valve_3_time_default 30000
#define frequency_invertor_startup_delay_default 15000
#define automatic_homeing_time_default 300000
#define automatic_mode_pause_time_default 1000000
#define automatic_mode_2020_startup_delay_default 40000
#define automatic_mode_2020_pause_timer_delay_default 300000
#define automatic_mode_2020_pause_flashlight_timer_delay_default 300000

// debug defines
//#define DEBUG_HYDRAULIC
//#define DEBUG_DETERMINE_ARRAYS
//#define DEBUG_MANUAL_CONTROLE
//#define DEBUG_AUTOMATIC_MODE
//#define DEBUG_EEPROM
//#define DEBUG_DETERMINE_ARRAYS

// Global variables
int frequency_invertor_on_off[4];
int frequency_invertor_direction[4];
int frequency_invertor_state[4];
int frequency_invertor_state_direction[4];

int hydraulic_open[4];
int hydraulic_close[4];
int hydraulic_state[4];
int hydraulic_time[4];
int hydraulic_actual_time[4];
int fase_relay[5];
int fase_relay_state[5];

int button_state[5];
int button_last_state[5];
int button[5];

int relay_state[5];
bool stewardplatform_state;
bool empty_relay_state;
bool system_12V_state;

// variable check LCD needs to be updated
int LCD_STATE_OLD;
int LCD_SUB_RUN_OLD;
int LCD_SUB_MANUAL_HYDRAULIC_OLD;
int LCD_SUB_MANUAL_OTHER_OLD;
int LCD_SUB_MANUAL_FRQ_OLD;
int LCD_SUB_SETTINGS_OLD;
int LCD_SUB_SETTINGS_ENABLE_OLD;
bool LCD_SUB_MANUAL_DIRECTION;
int LCD_WELCOME_DZ_COUNTER;
bool LCD_WELCOME_DZ_FLASH;
bool LCD_WELCOME_DZ_FLASH_RESET = true;
bool sub_manual_frequency_direction_old;
int automatic_homeing_time_old;
int check_buttons_delay;
int error_nr_old;
int sub_manual_other_component_nr_old;
int AUTOMATIC_MODE_2020_STARTUP_OLD;
int AUTOMATIC_MODE_2020_OLD;
long automatic_mode_2020_startup_delay;
long automatic_mode_2020_pause_timer_delay;
long automatic_mode_2020_pause_flashlight_timer_delay;
int LCD_SUB_SETTINGS_2020_OLD;

// LCD variables
int sub_manual_cylinder_nr = 1;
int sub_manual_frequency_nr = 1;
bool sub_manual_frequency_direction = true;
int sub_manual_other_component_nr = 1;

bool first_update_lcd = true;
bool AUTOMATIC_CYCLE_START = false;
int AUTOMATIC_CYCLE_COUNTER;

// Cycles variables
int cycle_test_approve_counter[4];
bool test_cycle_start = false;
int error_nr;

// Automatic mode variables
bool automatic_mode_state;
long automatic_homeing_time;
int automatic_frequency_inverter_startup_delay;
int frequency_invertor_startup_delay;
bool automatic_frequency_inverter_running = false;
long automatic_mode_pause_time;
long automatic_mode_pause_time_counter;
int automatic_mode_hydraulic_used[4];
int automatic_mode_frequency_used[4];
int automatic_mode_other_used[8];
bool automatic_mode_homeing_enable;
long automatic_mode_2020_startup_delay_timer;
bool automatic_mode_2020_pause;
long automatic_mode_2020_pause_timer;
long automatic_mode_2020_pause_flashlight_timer;

// Settings variables
bool edit_mode = false;

// Test function
int counter_test;

// Enum state machine
enum LCD_STATE_ENUM {
  LCD_STATE_WELCOME,
  LCD_STATE_START,
  LCD_STATE_MANUAL_HYDRAULIC,
  LCD_STATE_MANUAL_FREQUENCY_INVERTER,
  LCD_STATE_MANUAL_OTHER,
  LCD_STATE_SETTINGS,
  LCD_STATE_SETTINGS_2020
};

enum LCD_SUB_START_ENUM {
  SUB_RUN_IDLE,
  SUB_RUN_START,
  SUB_RUN_STOP
};

enum LCD_SUB_MANUAL_HYDRAULIC_ENUM {
  SUB_MANUAL_HYDRAULIC_IDLE,
  SUB_MANUAL_HYDRAULIC_SET,
  SUB_MANUAL_HYDRAULIC_RUN
};

enum LCD_SUB_MANUAL_FRQ_ENUM {
  SUB_MANUAL_FRQ_IDLE,
  SUB_MANUAL_FRQ_SET,
  SUB_MANUAL_FRQ_DIRECTION,
  SUB_MANUAL_FRQ_START,
  SUB_MANUAL_FRQ_STOP
};

enum LCD_SUB_MANUAL_OTHER_ENUM {
  SUB_MANUAL_OTHER_IDLE,
  SUB_MANUAL_OTHER_SET,
  SUB_MANUAL_OTHER_RUN
};
enum LCD_SUB_SETTINGS_ENUM {
  SUB_SETTINGS_IDLE,
  SUB_SETTINGS_AUTOMATIC_STARTUP_DELAY,
  SUB_SETTINGS_HYDRAULIC_VALVE_1,
  SUB_SETTINGS_HYDRAULIC_VALVE_2,
  SUB_SETTINGS_HYDRAULIC_VALVE_3,
  SUB_SETTINGS_FREQUENCY_STARTUP_DELAY,
  SUB_SETTINGS_ENABLE_FEATURE,
  SUB_SETTINGS_HOMEING,
  SUB_SETTINGS_DEFAULT,
  SUB_SETTINGS_SAVE
};
enum LCD_SUB_SETTINGS_2020_ENUM {
  SUB_SETTINGS_2020_IDLE,
  SUB_SETTINGS_2020_START_DELAY,
  SUB_SETTINGS_2020_FLASHLIGHT_DELAY,
  SUB_SETTINGS_2020_PAUSE_DELAY,
  SUB_SETTINGS_2020_SAVE
};

enum LCD_SUB_SETTINGS_ENABLE_ENUM {
  SUB_SETTINGS_ENABLE_IDLE,
  SUB_SETTINGS_ENABLE_HYDRAULIC_1,
  SUB_SETTINGS_ENABLE_HYDRAULIC_2,
  SUB_SETTINGS_ENABLE_HYDRAULIC_3,
  SUB_SETTINGS_ENABLE_FREQUENCY_1,
  SUB_SETTINGS_ENABLE_FREQUENCY_2,
  SUB_SETTINGS_ENABLE_FREQUENCY_3,
  SUB_SETTINGS_ENABLE_RELAY_1,
  SUB_SETTINGS_ENABLE_RELAY_2,
  SUB_SETTINGS_ENABLE_RELAY_3,
  SUB_SETTINGS_ENABLE_RELAY_4,
  SUB_SETTINGS_ENABLE_PLATFORM,
  SUB_SETTINGS_ENABLE_12V,
  SUB_SETTINGS_ENABLE_EMPTY
};

enum MASTER_STATE_ENUM {
  MASTER_STATE_IDLE,
  MASTER_STATE_START,
  MASTER_STATE_CYCLE_1,
  MASTER_STATE_CYCLE_2,
  MASTER_STATE_CYCLE_3,
  MASTER_STATE_STOP
};


enum MANUAL_CONTROL_VALVE_STATE_ENUM {
  MANUAL_CONTROLE_VALVE_IDLE,
  MANUAL_CONTROLE_VALVE_OUT,
  MANUAL_CONTROLE_VALVE_IN,
  MANUAL_CONTROLE_VALVE_STOP
};

enum AUTOMATIC_MODE_STATE_ENUM {
  AUTOMATIC_MODE_IDLE,
  AUTOMATIC_MODE_START,
  AUTOMATIC_MODE_RUN,
  AUTOMATIC_MODE_PAUSE,
  AUTOMATIC_MODE_STOP
};

enum AUTOMATIC_MODE_2020_ENUM {
  AUTOMATIC_MODE_2020_IDLE,
  AUTOMATIC_MODE_2020_START,
  AUTOMATIC_MODE_2020_RUN,
  AUTOMATIC_MODE_2020_PAUSE,
  AUTOMATIC_MODE_2020_STOP
};

enum AUTOMATIC_MODE_2020_STARTUP_ENUM {
  AUTOMATIC_MODE_20202_STARTUP_FR1,
  AUTOMATIC_MODE_20202_STARTUP_FR2,
  AUTOMATIC_MODE_20202_STARTUP_FR3,
  AUTOMATIC_MODE_20202_STARTUP_RELAY1,
  AUTOMATIC_MODE_20202_STARTUP_RELAY2,
  AUTOMATIC_MODE_20202_STARTUP_RELAY3,
  AUTOMATIC_MODE_20202_STARTUP_RELAY4,
  AUTOMATIC_MODE_20202_STARTUP_PLATFORM,
  AUTOMATIC_MODE_20202_STARTUP_12V,
  AUTOMATIC_MODE_20202_STARTUP_EMPTY
};
LCD_SUB_SETTINGS_2020_ENUM LCD_SUB_SETTINGS_2020;
AUTOMATIC_MODE_2020_STARTUP_ENUM AUTOMATIC_MODE_2020_STARTUP;
AUTOMATIC_MODE_2020_ENUM AUTOMATIC_MODE_2020;
LCD_SUB_SETTINGS_ENABLE_ENUM LCD_SUB_SETTINGS_ENABLE;
LCD_SUB_MANUAL_OTHER_ENUM LCD_SUB_MANUAL_OTHER;
LCD_SUB_SETTINGS_ENUM LCD_SUB_SETTINGS;
AUTOMATIC_MODE_STATE_ENUM AUTOMATIC_MODE_STATE;
LCD_SUB_MANUAL_FRQ_ENUM LCD_SUB_MANUAL_FRQ;
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
  Wire.begin();
  Wire.beginTransmission(0x27); //Your LCD Address
  lcd.begin(20, 4); // initialize the lcd
  lcd_start();
  lcd.print("Loading");
  LCD_LIGHT_ON();
  Serial.begin(115200);
  determine_arrays(false);
  controle_light_controle(1, true); // set controle light stop
  load_eeprom();


}

/************************************************************************************/
void loop()
/************************************************************************************/
{

  check_buttons();
  update_lcd();
  cycle_state();
  manual_control_valve();
  error();


  // TEST_function();
}

/************************************************************************************/
void determine_arrays(bool back_to_default)   // Set all pins to array new outputs & inputs needs to be add here.
/************************************************************************************/
{

#if defined(DEBUG_DETERMINE_ARRAYS)
  Serial.println("set hydraulic time to default");
#endif

  // set hydraulic time
  hydraulic_time[1] = hydraulic_valve_1_time_default;
  hydraulic_time[2] = hydraulic_valve_2_time_default;
  hydraulic_time[3] = hydraulic_valve_3_time_default;

#if defined(DEBUG_DETERMINE_ARRAYS)
  Serial.println("set frequency startup delay to default");
#endif
  frequency_invertor_startup_delay = frequency_invertor_startup_delay_default;

#if defined(DEBUG_DETERMINE_ARRAYS)
  Serial.println("set pause time to default");
#endif
  automatic_mode_pause_time = automatic_mode_pause_time_default;

#if defined(DEBUG_DETERMINE_ARRAYS)
  Serial.println("set homeing delay to default");
#endif
  automatic_homeing_time = automatic_homeing_time_default;

#if defined(DEBUG_DETERMINE_ARRAYS)
  Serial.println("unable homeing function");
#endif
  automatic_mode_homeing_enable = true;

#if defined(DEBUG_DETERMINE_ARRAYS)
  Serial.println("Turnoff manual mode fixtures");
#endif

  for (int i = 0; i < 5 ; i++) {
    automatic_mode_hydraulic_used[i] = 0;
    automatic_mode_frequency_used[i] = 0;
  }
  for (int i = 0; i < 8 ; i++) {
    automatic_mode_other_used[i] = 0;
  }

#if defined(DEBUG_DETERMINE_ARRAYS)
  Serial.println("Default autmaticmode 2020");
#endif

  automatic_mode_2020_pause_flashlight_timer_delay = automatic_mode_2020_pause_flashlight_timer_delay_default;
  automatic_mode_2020_pause_timer_delay = automatic_mode_2020_pause_timer_delay_default;
  automatic_mode_2020_startup_delay = automatic_mode_2020_startup_delay_default;

  if (!back_to_default) {

#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.println("Set manual mode functions to 0");
#endif

    for (int i = 0; i < 5 ; i++) {
      automatic_mode_hydraulic_used[i] = 0;
      automatic_mode_frequency_used[i] = 0;
    }
    for (int i = 0; i < 8 ; i++) {
      automatic_mode_other_used[i] = 0;
    }


#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.println("set hydraulic open");
#endif

    // set hydraulic open
    hydraulic_open[1] = hydraulic_valve_1_open;
    hydraulic_open[2] = hydraulic_valve_2_open;
    hydraulic_open[3] = hydraulic_valve_3_open;

    for (int i = 1 ; i < 4 ; i++) {
      pinMode(hydraulic_open[i], OUTPUT);
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
    }

#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.println("set frequency invertor on");
#endif

    // set frequency invertor on
    frequency_invertor_on_off[1] = frequency_invertor_1_on_off;
    frequency_invertor_on_off[2] = frequency_invertor_2_on_off;
    frequency_invertor_on_off[3] = frequency_invertor_3_on_off;


    for (int i = 1 ; i < 4 ; i++) {
      pinMode(frequency_invertor_on_off[i], OUTPUT);

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

    }

#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.println("set buttons");
#endif

    // set button
    button[up] = button_up;
    button[down] = button_down;
    button[select] = button_select;
    button[Stop] = button_stop;
    button[pause] = button_pause;

    for (int i = 0 ; i < 5 ; i++) {
      pinMode(button[i], INPUT);
    }

#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.println("set 3 fase relays");
#endif

    fase_relay[1] = relay_1;
    fase_relay[2] = relay_2;
    fase_relay[3] = relay_3;
    fase_relay[4] = relay_4;

    for (int i = 1 ; i < 6 ; i++) {
      pinMode(fase_relay[i], OUTPUT);
    }

#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.println("set other relays");
#endif
    pinMode(small_motor_system, OUTPUT);
    pinMode(stewardplatform, OUTPUT);
    pinMode(empty_relay, OUTPUT);


#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.println("set controle lights");
#endif
    pinMode(controle_light_stop, OUTPUT);
    pinMode(controle_light_run, OUTPUT);
    pinMode(controle_light_pause, OUTPUT);


#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.println("Relay 1,2,3,4 state 0");
#endif

    for (int i = 0; i < 6; i++) {
      relay_state[i] = 0;
    }


#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.println("Set other components false");
#endif
    stewardplatform_state = false;
    empty_relay_state = false;
    system_12V_state = false;
    automatic_mode_2020_pause = false;
  }
}
