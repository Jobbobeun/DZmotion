/************************************************************************************/
void update_lcd()
/************************************************************************************/
{
  int case_max = 4;
  int case_min = 2;
  bool select_bool = false;

  // check if butten released
  if ((button_state[up] == 1) && (button_last_state[up] == 0)) {
    LCD_STATE++;
    button_last_state[up] = 1;
    if (LCD_STATE == (case_max + 1)) {
      LCD_STATE = case_min;
    }
  }

  if ((button_state[down] == 1) && (button_last_state[down] == 0)) {
    LCD_STATE--;
    button_last_state[down] = 1;
    if (LCD_STATE == (case_min - 1)) {
      LCD_STATE = case_max;
    }
  }


  if ((button_state[select] == 1) && (button_last_state[select] == 0)) {
    button_last_state[select] == 1;
    select_bool = true;
  }


  if (LCD_STATE != LCD_STATE_OLD) {
    LCD_STATE_OLD = LCD_STATE;
    LCD_TXT_UPDATE = true;
  }

  switch (LCD_STATE) {
    case 1: // Welcome state

      if (LCD_TXT_UPDATE) {

        lcd_start();
        lcd.print("Welcome DZmotion");
        lcd.setCursor(3, 1);    lcd.print("2019-2020");

      }

      break;
    case 2:

      if (LCD_TXT_UPDATE) {

        lcd_start();
        lcd.print("Case 2");

      }

      if (select_bool) {

        CYCLE_STATE = 1; // set action after select case

      }

      break;
    case 3:

      if (LCD_TXT_UPDATE) {
        lcd_start();
        lcd.print("Case 3"); // set action after select case
      }

      if (select_bool) {

        CYCLE_STATE = 2;

      }

      break;
    case 4:

      if (LCD_TXT_UPDATE) {
        lcd_start();
        lcd.print("Case 4"); // set action after select case
      }

      if (select_bool) {
        CYCLE_STATE = 3;
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

  for (int i = 0 ; i < 3; i++) {
    if (digitalRead(button[i]) == HIGH) {
      button_state[i] = 1;
      Serial.print("Change status button: ");
      Serial.println(i);
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
  LCD_TXT_UPDATE = false;
}
