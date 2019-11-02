void update_lcd() {

  switch (LCD_STATE) {
    case 1:
      LCD_TOP("WELKOM");
      LCD_BOTTOM("DZ MOTION");
      break;
    case 2:
      break;
    case 3:
      break;
  }
}

void LCD_TOP(char text) {
  lcd.setCursor(0, 0);
  lcd.print(text);
}

void LCD_BOTTOM(char text) {
  lcd.setCursor(0, 1);
  lcd.print(text);
}

void LCD_LIGHT_ON() {
  lcd.setBacklight(HIGH);
}

void LCD_LIGHT_OFF() {
  lcd.setBacklight(LOW);
}

void check_buttons() {
  
  for (int i = 0 ; i = 3; i++) {
    if (digitalRead(button[i]) == HIGH) {
      button_status[i] = 1;
    }
    else {
      button_status[i] = 0;
    }
  }

}
