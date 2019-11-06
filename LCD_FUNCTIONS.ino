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
      } else if (LCD_BUTTON_SELECT()) {
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
          } else if (LCD_BUTTON_UP()) {
            LCD_STATE = LCD_STATE_MANUAL_HYDRAULIC;
          } else if (LCD_BUTTON_DOWN()) {
            LCD_STATE = LCD_STATE_WELCOME;
          }
          /*-------------------------------*/
          break;
        case SUB_RUN_START:
          if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("Automatic running");
          }
          /*----       RUN START      ----*/
          AUTOMATIC_CYCLE_START = true;

          if (LCD_BUTTON_STOP()) {
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
          AUTOMATIC_CYCLE_START = false;
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

      if (CHECK_UPDATE_LCD()) {
       lcd.print("Manual mode"); // set action after select case
            lcd.setCursor(0, 1);
            lcd.print("Frequency inverter");
      }

      if (LCD_BUTTON_DOWN()) {
        LCD_STATE = LCD_STATE_MANUAL_HYDRAULIC;
      } else if (LCD_BUTTON_UP()) {
        LCD_STATE = LCD_STATE_SETTINGS;
      }

      break;
    case LCD_STATE_SETTINGS:

      if (CHECK_UPDATE_LCD()) {
        lcd_start();
        lcd.print("Settings"); // set action after select case
      }

      if (button_state[select]) {
        MANUAL_CONTROLE_VALVE_STATE = MANUAL_CONTROLE_VALVE_OUT;
      }

      if (LCD_BUTTON_DOWN()) {
        LCD_STATE = LCD_STATE_MANUAL_HYDRAULIC;
      }

      break;

  }
}


/************************************************************************************/
void LCD_TOP(char text)
/************************************************************************************/
{
  lcd.setBacklight(255);
  lcd.home(); lcd.clear();
  lcd.print(text);
  lcd.setCursor(0, 1);    lcd.print(text);
}

/************************************************************************************/
void LCD_BOTTOM(char text)
/************************************************************************************/
{
  lcd.setCursor(0, 1);
  lcd.print(text);
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

  for (int i = 0 ; i < 4; i++) {

    if (digitalRead(button[i]) == HIGH) {
      button_state[i] = 1;


    }
    else {
      button_state[i] = 0;
      button_last_state[i] = 0;

    }
  }


}

/************************************************************************************/
void lcd_start()
/************************************************************************************/
{
  lcd.home(); lcd.clear();

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
  }
  else {
    return false;
  }


}
