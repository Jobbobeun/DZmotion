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

      if (LCD_BUTTON_UP() == true) {
        LCD_STATE = LCD_STATE_START;
      }

      break;

    case LCD_STATE_START:

      switch (LCD_SUB_RUN) {

        case SUB_RUN_IDLE:

          if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("Press select to");
            lcd.setCursor(0,1);
            lcd.print("start");
          }

          if (LCD_BUTTON_SELECT()) {
            LCD_SUB_RUN = SUB_RUN_START;
          } else if (LCD_BUTTON_UP()) {
            LCD_STATE = LCD_STATE_SELECT;
          } else if (LCD_BUTTON_DOWN()) {
            LCD_STATE = LCD_STATE_WELCOME;
          }


          break;

        case SUB_RUN_START:
          if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("Automatic running");
          }
          AUTOMATIC_CYCLE_START = true;

          if (LCD_BUTTON_STOP()) {
            LCD_SUB_RUN = SUB_RUN_STOP;
          }

          break;

        case SUB_RUN_STOP:
        if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("Automatic Stoped");
          }
          AUTOMATIC_CYCLE_START = false;
          LCD_SUB_RUN = SUB_RUN_IDLE;
          break;

      }

      break;
    case LCD_STATE_SELECT:
      if (CHECK_UPDATE_LCD()) {
        lcd_start();
        lcd.print("Select state"); // set action after select case
      }

      if (LCD_BUTTON_UP()) {
        LCD_STATE = LCD_STATE_SETTINGS;
      } else if (LCD_BUTTON_DOWN()) {
        LCD_STATE = LCD_STATE_START;
      }

      break;
    case LCD_STATE_SETTINGS:

      if (CHECK_UPDATE_LCD()) {
        lcd_start();
        lcd.print("Case 4"); // set action after select case
      }

      if (LCD_BUTTON_DOWN()) {
        LCD_STATE = LCD_STATE_SELECT;
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

  for (int i = 0 ; i < 4; i++) {
    if (button_state[i] == 1) {
      return true;
    }
  }

  // check LCD state is changed
  if (LCD_STATE != LCD_STATE_OLD) {
    LCD_STATE_OLD = LCD_STATE;
    return true;
  } else if (LCD_SUB_RUN != LCD_SUB_RUN_OLD) {
    LCD_SUB_RUN_OLD = LCD_SUB_RUN;
    return true;
  }
  else {
    return false;
  }


}
