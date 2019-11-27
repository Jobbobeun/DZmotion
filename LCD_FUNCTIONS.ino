/************************************************************************************/
void update_lcd()
/************************************************************************************/
{
  switch (LCD_STATE) {

    case LCD_STATE_WELCOME: // Welcome state

      if (CHECK_UPDATE_LCD() || first_update_lcd) {
        first_update_lcd = false;
        lcd_start();
        lcd.print("Welcome DZmotion");
        lcd.setCursor(3, 1);    lcd.print("2019-2020");
      }

      if (LCD_WELCOME_DZ_FLASH_RESET) {
        if (LCD_WELCOME_DZ_COUNTER > 5000) {

          if (LCD_WELCOME_DZ_FLASH == true) {
            LCD_LIGHT_ON();
            LCD_WELCOME_DZ_FLASH = false;
            LCD_WELCOME_DZ_COUNTER = 0;
          } else if (LCD_WELCOME_DZ_FLASH == false) {
            LCD_LIGHT_OFF();
            LCD_WELCOME_DZ_FLASH = true;
            LCD_WELCOME_DZ_COUNTER = 0;
          }
        }
        LCD_WELCOME_DZ_COUNTER ++;
      }

      if (LCD_BUTTON_UP()) {
        LCD_STATE = LCD_STATE_START;
        LCD_WELCOME_DZ_FLASH_RESET = false;
        LCD_LIGHT_ON();
      } else if (LCD_BUTTON_SELECT() || LCD_BUTTON_DOWN() || LCD_BUTTON_STOP()) {
        LCD_WELCOME_DZ_FLASH_RESET = false;
        LCD_LIGHT_ON();
      }

      break;

    case LCD_STATE_START:

      switch (LCD_SUB_RUN) {

        case SUB_RUN_IDLE:

          if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("Press select to");
            lcd.setCursor(0, 1);
            lcd.print("start");
          }
          /*----       RUN IDLE        ----*/
          if (LCD_BUTTON_SELECT()) {
            LCD_SUB_RUN = SUB_RUN_START;
            AUTOMATIC_MODE_STATE = AUTOMATIC_MODE_START;    // set state for automatic program
          } else if (LCD_BUTTON_UP()) {
            LCD_STATE = LCD_STATE_MANUAL_HYDRAULIC;
          } else if (LCD_BUTTON_DOWN()) {
          //  LCD_STATE = LCD_STATE_WELCOME;
          }
          /*-------------------------------*/
          break;
        case SUB_RUN_START:
          if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("Automatic running");
          }
          /*----       RUN START      ----*/
          automatic_mode();                      // Run function with program statemachine

          if (!automatic_mode_state) {
            LCD_SUB_RUN = SUB_RUN_STOP;

          }

          /*-------------------------------*/
          break;

        case SUB_RUN_STOP:
          if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("Automatic Stoped");
          }
          /*----       RUN STOP      ----*/

          LCD_SUB_RUN = SUB_RUN_IDLE;
          /*-------------------------------*/
          break;

      }

      break;
    case LCD_STATE_MANUAL_HYDRAULIC:

      switch (LCD_SUB_MANUAL_HYDRAULIC) {

        case SUB_MANUAL_HYDRAULIC_IDLE:

          if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("Manual mode"); // set action after select case
            lcd.setCursor(0, 1);
            lcd.print("Hydraulic");
          }

          if (LCD_BUTTON_UP()) {
            LCD_STATE = LCD_STATE_MANUAL_FREQUENCY_INVERTER;
          } else if (LCD_BUTTON_DOWN()) {
            LCD_STATE = LCD_STATE_START;
          } else  if (LCD_BUTTON_SELECT()) {
            LCD_SUB_MANUAL_HYDRAULIC = SUB_MANUAL_HYDRAULIC_SET;
          }

          break;

        case SUB_MANUAL_HYDRAULIC_SET:

          if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("Cylinder:");
            lcd.setCursor(10, 0);
            lcd.print(sub_manual_cylinder_nr);
          }

          if (LCD_BUTTON_UP()) {
            sub_manual_cylinder_nr ++;

            if (sub_manual_cylinder_nr > cylinder_amount) {
              sub_manual_cylinder_nr = 1;
            }
            lcd.setCursor(10, 0);
            lcd.print(sub_manual_cylinder_nr);
          } else if (LCD_BUTTON_DOWN()) {
            sub_manual_cylinder_nr --;

            if (sub_manual_cylinder_nr == 0) {
              sub_manual_cylinder_nr = cylinder_amount;
            }
            lcd.setCursor(10, 0);
            lcd.print(sub_manual_cylinder_nr);
          } else if (LCD_BUTTON_SELECT()) {
            LCD_SUB_MANUAL_HYDRAULIC = SUB_MANUAL_HYDRAULIC_RUN;
          }
          else if (LCD_BUTTON_STOP()) {
            LCD_SUB_MANUAL_HYDRAULIC = SUB_MANUAL_HYDRAULIC_IDLE;
          }


          break;

        case SUB_MANUAL_HYDRAULIC_RUN:

          if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("Cylinder: ");
            lcd.setCursor(10, 0);
            lcd.print(sub_manual_cylinder_nr);
            lcd.setCursor(0, 1);
            lcd.print("Direction:");
          }
          if (button_state[up]) {
            MANUAL_CONTROLE_VALVE_STATE = MANUAL_CONTROLE_VALVE_OUT;
            if (LCD_BUTTON_UP()) {
              lcd.setCursor(11, 1);
              lcd.print("OUT");
              LCD_SUB_MANUAL_DIRECTION = true;
            }
          } else if (button_state[down]) {
            MANUAL_CONTROLE_VALVE_STATE = MANUAL_CONTROLE_VALVE_IN;
            if (LCD_BUTTON_DOWN()) {
              lcd.setCursor(11, 1);
              lcd.print("IN ");
              LCD_SUB_MANUAL_DIRECTION = true;
            }
          }  else if (LCD_BUTTON_STOP()) {
            LCD_SUB_MANUAL_HYDRAULIC = SUB_MANUAL_HYDRAULIC_SET;
          } else if (LCD_SUB_MANUAL_DIRECTION) {
            lcd.setCursor(11, 1);
            lcd.print("   ");
          }

          break;

      }
      break;

    case LCD_STATE_MANUAL_FREQUENCY_INVERTER:

      switch (LCD_SUB_MANUAL_FRQ) {

        case SUB_MANUAL_FRQ_IDLE:

          if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("Manual mode"); // set action after select case
            lcd.setCursor(0, 1);
            lcd.print("Frequency inverter");
          }

          if (LCD_BUTTON_DOWN()) {
            LCD_STATE = LCD_STATE_MANUAL_HYDRAULIC;
          } else if (LCD_BUTTON_UP()) {
            LCD_STATE = LCD_STATE_SETTINGS;
          } else if (LCD_BUTTON_SELECT()) {
            LCD_SUB_MANUAL_FRQ = SUB_MANUAL_FRQ_SET;
          }

          break;

        case SUB_MANUAL_FRQ_SET:

          if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("Frq nr:");
            lcd.setCursor(8, 0);
            lcd.print(sub_manual_frequency_nr);
          }
          if (LCD_BUTTON_UP()) {
            sub_manual_frequency_nr ++;
            if (sub_manual_frequency_nr > frequency_inverter_amount) {
              sub_manual_frequency_nr = 1;
            }
            lcd.setCursor(8, 0);
            lcd.print(sub_manual_frequency_nr);
          } else if (LCD_BUTTON_DOWN()) {
            sub_manual_frequency_nr --;
            if (sub_manual_frequency_nr < 1) {
              sub_manual_frequency_nr = frequency_inverter_amount;
            }
            lcd.setCursor(8, 0);
            lcd.print(sub_manual_frequency_nr);
          } else if (LCD_BUTTON_SELECT()) {
            LCD_SUB_MANUAL_FRQ = SUB_MANUAL_FRQ_DIRECTION;
          } else if (LCD_BUTTON_STOP()) {
            LCD_SUB_MANUAL_FRQ = SUB_MANUAL_FRQ_IDLE;
          }

          break;

        case SUB_MANUAL_FRQ_DIRECTION:

          if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("Frq nr: ");
            lcd.setCursor(8, 0);
            lcd.print(sub_manual_frequency_nr);
            lcd.setCursor(10, 0);
            lcd.print("Direction: ");
            if (sub_manual_frequency_direction) {
              lcd.setCursor(0, 1);
              lcd.print("Clockwise");
            } else {
              lcd.setCursor(0, 1);
              lcd.print("Counterclockwise");
            }
          }

          if (LCD_BUTTON_SELECT()) {
            LCD_SUB_MANUAL_FRQ = SUB_MANUAL_FRQ_START;
          } else if (LCD_BUTTON_UP()) {
            sub_manual_frequency_direction = true;
          } else if (LCD_BUTTON_DOWN()) {
            sub_manual_frequency_direction = false;
          } else if (LCD_BUTTON_STOP()) {
            LCD_SUB_MANUAL_FRQ = SUB_MANUAL_FRQ_SET;
          }


          break;

        case SUB_MANUAL_FRQ_START:

          if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("Frq nr: ");
            lcd.setCursor(8, 0);
            lcd.print(sub_manual_frequency_nr);
            lcd.setCursor(10, 0);
            lcd.print(" is ");
            lcd.setCursor(0, 1);
            lcd.print("now running");
          }

          set_frequency_inverter(sub_manual_frequency_nr, true, sub_manual_frequency_direction);
          if (LCD_BUTTON_STOP()) {
            LCD_SUB_MANUAL_FRQ = SUB_MANUAL_FRQ_STOP;
          }


          break;

        case SUB_MANUAL_FRQ_STOP:

          if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("Stop Frq nr:");
            lcd.setCursor(13, 0);
            lcd.print(sub_manual_frequency_nr);
          }

          set_frequency_inverter(sub_manual_frequency_nr, false, false);
          LCD_SUB_MANUAL_FRQ = SUB_MANUAL_FRQ_SET;
          break;
      }
      break;

    case LCD_STATE_SETTINGS:

      switch (LCD_SUB_SETTINGS) {

        case SUB_SETTINGS_IDLE:
          if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("Settings");
          }

          if (LCD_BUTTON_DOWN()) {
            LCD_STATE = LCD_STATE_MANUAL_HYDRAULIC;
          } else if (LCD_BUTTON_SELECT()) {
            LCD_SUB_SETTINGS = SUB_SETTINGS_AUTOMATIC_STARTUP_DELAY;
          }

          break;

        case SUB_SETTINGS_AUTOMATIC_STARTUP_DELAY:

          if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("Homing delay:");
            lcd.setCursor(0, 1);
            lcd.print((automatic_homeing_time));
          }


          if (edit_mode) {
            if (LCD_BUTTON_UP()) {

              automatic_homeing_time = automatic_homeing_time + settings_accuracy;
              lcd.setCursor(0, 1);
              lcd.print((automatic_homeing_time));

            }  else if (LCD_BUTTON_DOWN()) {

              automatic_homeing_time = automatic_homeing_time - settings_accuracy;
              lcd.setCursor(0, 1);
              lcd.print((automatic_homeing_time));

            } else if (LCD_BUTTON_STOP()) {

              edit_mode = false;
              lcd.noBlink();

            }
          } else {

            if (LCD_BUTTON_UP()) {

              LCD_SUB_SETTINGS = SUB_SETTINGS_HYDRAULIC_VALVE_1;

            }  else if (LCD_BUTTON_SELECT()) {

              edit_mode = true;
              lcd.blink();

            } else if (LCD_BUTTON_STOP()) {
              LCD_SUB_SETTINGS = SUB_SETTINGS_SAVE;
            }
          }


          break;

        case SUB_SETTINGS_HYDRAULIC_VALVE_1:
          if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("Hydraulic valve 1:");
            lcd.setCursor(0, 1);
            lcd.print(hydraulic_time[1]);
          }

          if (edit_mode) {
            if (LCD_BUTTON_UP()) {

              hydraulic_time[1] = hydraulic_time[1] + settings_accuracy;
              lcd.setCursor(0, 1);
              lcd.print(hydraulic_time[1]);

            }  else if (LCD_BUTTON_DOWN()) {

              hydraulic_time[1] = hydraulic_time[1] - settings_accuracy;
              lcd.setCursor(0, 1);
              lcd.print(hydraulic_time[1]);

            } else if (LCD_BUTTON_STOP()) {

              edit_mode = false;
              lcd.noBlink();

            }
          } else {
            if (LCD_BUTTON_UP()) {
              LCD_SUB_SETTINGS = SUB_SETTINGS_HYDRAULIC_VALVE_2;
            } else if (LCD_BUTTON_DOWN()) {
              LCD_SUB_SETTINGS = SUB_SETTINGS_AUTOMATIC_STARTUP_DELAY;
            } else if (LCD_BUTTON_SELECT()) {

              edit_mode = true;
              lcd.blink();

            } else if (LCD_BUTTON_STOP()) {
              LCD_SUB_SETTINGS = SUB_SETTINGS_SAVE;
            }
          }
          break;

        case SUB_SETTINGS_HYDRAULIC_VALVE_2:
          if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("Hydraulic valve 2:");
            lcd.setCursor(0, 1);
            lcd.print(hydraulic_time[2]);
          }

          if (edit_mode) {
            if (LCD_BUTTON_UP()) {

              hydraulic_time[2] = hydraulic_time[2] + settings_accuracy;
              lcd.setCursor(0, 1);
              lcd.print(hydraulic_time[2]);

            }  else if (LCD_BUTTON_DOWN()) {

              hydraulic_time[2] = hydraulic_time[2] - settings_accuracy;
              lcd.setCursor(0, 1);
              lcd.print(hydraulic_time[2]);

            } else if (LCD_BUTTON_STOP()) {

              edit_mode = false;
              lcd.noBlink();

            }
          } else {
            if (LCD_BUTTON_UP()) {
              LCD_SUB_SETTINGS = SUB_SETTINGS_HYDRAULIC_VALVE_3;
            } else if (LCD_BUTTON_DOWN()) {
              LCD_SUB_SETTINGS = SUB_SETTINGS_HYDRAULIC_VALVE_1;
            } else if (LCD_BUTTON_SELECT()) {

              edit_mode = true;
              lcd.blink();

            } else if (LCD_BUTTON_STOP()) {
              LCD_SUB_SETTINGS = SUB_SETTINGS_SAVE;
            }
          }
          break;

        case SUB_SETTINGS_HYDRAULIC_VALVE_3:
          if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("Hydraulic valve 3:");
            lcd.setCursor(0, 1);
            lcd.print((hydraulic_time[3]));
          }

          if (edit_mode) {
            if (LCD_BUTTON_UP()) {

              hydraulic_time[3] = hydraulic_time[3] + settings_accuracy;
              lcd.setCursor(0, 1);
              lcd.print(hydraulic_time[3]);

            }  else if (LCD_BUTTON_DOWN()) {

              hydraulic_time[3] = hydraulic_time[3] - settings_accuracy;
              lcd.setCursor(0, 1);
              lcd.print(hydraulic_time[3]);

            } else if (LCD_BUTTON_STOP()) {

              edit_mode = false;
              lcd.noBlink();

            }
          } else {
            if (LCD_BUTTON_UP()) {
              LCD_SUB_SETTINGS = SUB_SETTINGS_FREQUENCY_STARTUP_DELAY;
            } else if (LCD_BUTTON_DOWN()) {

              LCD_SUB_SETTINGS = SUB_SETTINGS_HYDRAULIC_VALVE_2;

            } else if (LCD_BUTTON_SELECT()) {

              edit_mode = true;
              lcd.blink();

            } else if (LCD_BUTTON_STOP()) {
              LCD_SUB_SETTINGS = SUB_SETTINGS_SAVE;
            }
          }
          break;

        case SUB_SETTINGS_FREQUENCY_STARTUP_DELAY:

          if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("Frequency startup delay:");
            lcd.setCursor(0, 1);
            lcd.print(frequency_invertor_startup_delay);
          }

          if (edit_mode) {
            if (LCD_BUTTON_UP()) {

              frequency_invertor_startup_delay = (frequency_invertor_startup_delay + settings_accuracy);
              lcd.setCursor(0, 1);
              lcd.print(frequency_invertor_startup_delay);

            }  else if (LCD_BUTTON_DOWN()) {

              frequency_invertor_startup_delay = (frequency_invertor_startup_delay - settings_accuracy);
              lcd.setCursor(0, 1);
              lcd.print(frequency_invertor_startup_delay);

            } else if (LCD_BUTTON_STOP()) {

              edit_mode = false;
              lcd.noBlink();

            }
          } else {
            if (LCD_BUTTON_UP()) {
 LCD_SUB_SETTINGS = SUB_SETTINGS_DEFAULT;
            } else if (LCD_BUTTON_DOWN()) {
              LCD_SUB_SETTINGS = SUB_SETTINGS_HYDRAULIC_VALVE_3;
            } else if (LCD_BUTTON_SELECT()) {
              edit_mode = true;
              lcd.blink();

            } else if (LCD_BUTTON_STOP()) {
              LCD_SUB_SETTINGS = SUB_SETTINGS_SAVE;
            }
          }

          break;

        case SUB_SETTINGS_DEFAULT:
          if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("Back to default?");
          }

          if (LCD_BUTTON_SELECT()) {
            
            determine_arrays(true);
            
          } else if (LCD_BUTTON_DOWN()) {
            
            LCD_SUB_SETTINGS = SUB_SETTINGS_FREQUENCY_STARTUP_DELAY;
            
          } else if (LCD_BUTTON_STOP()) {
            
            LCD_SUB_SETTINGS = SUB_SETTINGS_SAVE;
            
          }
          break;

        case SUB_SETTINGS_SAVE:

          if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("Save press select");
            lcd.setCursor(0, 1);
            lcd.print("Back press stop");
          }

          if (LCD_BUTTON_STOP()) {

            LCD_SUB_SETTINGS = SUB_SETTINGS_IDLE;

          } else if (LCD_BUTTON_SELECT()) {

            update_eeprom();
            lcd_start();
            lcd.print("save eeprom");
          }
          break;
      }

      break;

  }
}


/************************************************************************************/
void LCD_LIGHT_ON()
/************************************************************************************/
{
  lcd.setBacklight(255);
}

/************************************************************************************/
void LCD_LIGHT_OFF()
/************************************************************************************/
{
  lcd.setBacklight(0);
}

/************************************************************************************/
void check_buttons()
/************************************************************************************/
{

  if (check_buttons_delay == 50) {
    for (int i = 0 ; i < 5; i++) {

      if (digitalRead(button[i]) == HIGH) {
        button_state[i] = 1;


      }
      else {
        button_state[i] = 0;
        button_last_state[i] = 0;

      }
    }
    check_buttons_delay = 0;
  } else {
    check_buttons_delay++;
  }

}

/************************************************************************************/
void lcd_start()
/************************************************************************************/
{
  lcd.home();
  lcd.clear();
  lcd.noBlink();

}

/************************************************************************************/
bool LCD_BUTTON_PAUSE()
/************************************************************************************/
{

  if ((button_state[pause] == 1) && (button_last_state[pause] == 0)) {
    button_last_state[pause] = 1;
    return true;

  } else {
    return false;
  }

}

/************************************************************************************/
bool LCD_BUTTON_UP()
/************************************************************************************/
{

  if ((button_state[up] == 1) && (button_last_state[up] == 0)) {
    button_last_state[up] = 1;
    return true;

  } else {
    return false;
  }

}

/************************************************************************************/
bool LCD_BUTTON_DOWN()
/************************************************************************************/
{

  if ((button_state[down] == 1) && (button_last_state[down] == 0)) {
    button_last_state[down] = 1;
    return true;
  } else {
    return false;
  }
}

/************************************************************************************/
bool LCD_BUTTON_SELECT()
/************************************************************************************/
{

  if ((button_state[select] == 1) && (button_last_state[select] == 0)) {
    button_last_state[select] = 1;
    return true;
  } else {
    return false;
  }
}

/************************************************************************************/
bool LCD_BUTTON_STOP()
/************************************************************************************/
{

  if ((button_state[Stop] == 1) && (button_last_state[Stop] == 0)) {
    button_last_state[Stop] = 1;
    return true;
  } else {
    return false;
  }
}

/************************************************************************************/
bool CHECK_UPDATE_LCD()
/************************************************************************************/
{

  // check LCD state is changed
  if (LCD_STATE != LCD_STATE_OLD) {
    LCD_STATE_OLD = LCD_STATE;
    return true;
  } else if (LCD_SUB_RUN != LCD_SUB_RUN_OLD) {
    LCD_SUB_RUN_OLD = LCD_SUB_RUN;
    return true;
  } else if (LCD_SUB_MANUAL_HYDRAULIC != LCD_SUB_MANUAL_HYDRAULIC_OLD) {
    LCD_SUB_MANUAL_HYDRAULIC_OLD = LCD_SUB_MANUAL_HYDRAULIC;
    return true;
  } else if (LCD_SUB_MANUAL_FRQ != LCD_SUB_MANUAL_FRQ_OLD) {
    LCD_SUB_MANUAL_FRQ_OLD = LCD_SUB_MANUAL_FRQ;
    return true;
  } else if (sub_manual_frequency_direction != sub_manual_frequency_direction_old) {
    sub_manual_frequency_direction_old = sub_manual_frequency_direction;
    return true;
  } else if (error_nr != error_nr_old) {
    error_nr_old = error_nr;
    return true;
  } else if (LCD_SUB_SETTINGS != LCD_SUB_SETTINGS_OLD) {
    LCD_SUB_SETTINGS_OLD = LCD_SUB_SETTINGS;
    return true;
  }  else {
    return false;
  }


}

/************************************************************************************/
void error()
/************************************************************************************/
{
  // stop all





  switch (error_nr) {

    case 1:
      if (CHECK_UPDATE_LCD()) {
        lcd_start();
        stop_all_hydraulic();
        lcd.print("Troggel Hydraulic direction");
        lcd.setCursor(0, 1);
        lcd.print("ERROR state in reset.");
      }

      if (LCD_BUTTON_STOP()) {
        error_nr = 0;
        lcd_start();
        lcd.print("ERROR RESET");
        delay(2000);
      }
      break;
  }

}
