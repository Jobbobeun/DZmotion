/************************************************************************************/
void update_lcd()
/************************************************************************************/
{
  int update_var;
  switch (LCD_STATE) {

    /*-----------------------------------------WELCOME----------------------------------*/
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

    /*-----------------------------------------AUTOMATIC MODE----------------------------------*/
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

    /*-----------------------------------------MANUAL HYDRAULIC----------------------------------*/
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

    /*-----------------------------------------MANUAL FREQUENCY INVERTER----------------------------------*/

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
            LCD_STATE = LCD_STATE_MANUAL_OTHER;
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

    /*-----------------------------------------MANUAL OTHER----------------------------------*/
    case LCD_STATE_MANUAL_OTHER:

      switch (LCD_SUB_MANUAL_OTHER) {
        case SUB_MANUAL_OTHER_IDLE:

          if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("Manual mode"); // set action after select case
            lcd.setCursor(0, 1);
            lcd.print("Other components");
          }

          if (LCD_BUTTON_DOWN()) {
            LCD_STATE = LCD_STATE_MANUAL_FREQUENCY_INVERTER;
          } else if (LCD_BUTTON_UP()) {
            LCD_STATE = LCD_STATE_SETTINGS;
          } else if (LCD_BUTTON_SELECT()) {
            LCD_SUB_MANUAL_OTHER = SUB_MANUAL_OTHER_SET;
          }


          break;

        case SUB_MANUAL_OTHER_SET:

          if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("Select component:"); // set action after select case
            lcd.setCursor(0, 1);

            switch (sub_manual_other_component_nr) {
              case 1:
                lcd.print("Relay 1");
                break;

              case 2:
                lcd.print("Relay 2");
                break;

              case 3:
                lcd.print("Relay 3");
                break;

              case 4:
                lcd.print("Relay 4");
                break;

              case 5:
                lcd.print("stewardplatform");
                break;

              case 6:
                lcd.print("12V motors");
                break;

              case 7:
                lcd.print("EMPTY RELAY");
                break;
            }
          }



          if (LCD_BUTTON_DOWN()) {

            sub_manual_other_component_nr --;
            if (sub_manual_other_component_nr < 1) {
              sub_manual_other_component_nr = other_components_amount;
            }

          } else if (LCD_BUTTON_UP()) {
            sub_manual_other_component_nr++;
            if (sub_manual_other_component_nr > other_components_amount) {
              sub_manual_other_component_nr = 1;
            }


          } else if (LCD_BUTTON_SELECT()) {
            LCD_SUB_MANUAL_OTHER = SUB_MANUAL_OTHER_RUN;
          } else if (LCD_BUTTON_STOP()) {
            LCD_SUB_MANUAL_OTHER = SUB_MANUAL_OTHER_IDLE;
          }

          break;

        case SUB_MANUAL_OTHER_RUN:

          if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("RUNNING:"); // set action after select case
            lcd.setCursor(0, 1);

            if (sub_manual_other_component_nr == 1) {
              lcd.print("Relay 1");
              controle_relay(sub_manual_other_component_nr, true);
            } else if (sub_manual_other_component_nr == 2) {
              lcd.print("Relay 2");
              controle_relay(sub_manual_other_component_nr, true);
            } else if (sub_manual_other_component_nr == 3) {
              lcd.print("Relay 3");
              controle_relay(sub_manual_other_component_nr, true);
            } else if (sub_manual_other_component_nr == 4) {
              lcd.print("Relay 4");
              controle_relay(sub_manual_other_component_nr, true);
            } else if (sub_manual_other_component_nr == 5) {
              lcd.print("Stewardplatform");
              controle_stewardplatform(true);
            } else if (sub_manual_other_component_nr == 6) {
              lcd.print("12V motors");
              controle_12v_system(true);
            } else if (sub_manual_other_component_nr == 7) {
              lcd.print("Empty relay");
            }

          }


          if (LCD_BUTTON_STOP()) {
            LCD_SUB_MANUAL_OTHER = SUB_MANUAL_OTHER_SET;
            if (sub_manual_other_component_nr == 1) {
              lcd.print("Relay 1");
              controle_relay(sub_manual_other_component_nr, false);
            } else if (sub_manual_other_component_nr == 2) {
              lcd.print("Relay 2");
              controle_relay(sub_manual_other_component_nr, false);
            } else if (sub_manual_other_component_nr == 3) {
              lcd.print("Relay 3");
              controle_relay(sub_manual_other_component_nr, false);
            } else if (sub_manual_other_component_nr == 4) {
              lcd.print("Relay 4");
              controle_relay(sub_manual_other_component_nr, false);
            } else if (sub_manual_other_component_nr == 5) {
              lcd.print("Stewardplatform");
              controle_stewardplatform(false);
            } else if (sub_manual_other_component_nr == 6) {
              lcd.print("12V motors");
              controle_12v_system(false);
            } else if (sub_manual_other_component_nr == 7) {
              lcd.print("Empty relay");
            }
          }

          break;

      }

      break;

    /*-----------------------------------------SETTINGS--------------------------------------*/
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
        /***************************************************************************************/
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
        /***************************************************************************************/
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
        /***************************************************************************************/
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
        /***************************************************************************************/
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
        /***************************************************************************************/
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
              LCD_SUB_SETTINGS = SUB_SETTINGS_ENABLE_FEATURE;
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
        /***************************************************************************************/
        case SUB_SETTINGS_ENABLE_FEATURE:

          switch (LCD_SUB_SETTINGS_ENABLE) {

            case SUB_SETTINGS_ENABLE_IDLE:
              if (CHECK_UPDATE_LCD()) {
                lcd_start();
                lcd.print("Enable hardware");
              }

              if (LCD_BUTTON_UP()) {
                LCD_SUB_SETTINGS = SUB_SETTINGS_HOMEING;
              } else if (LCD_BUTTON_DOWN()) {
                LCD_SUB_SETTINGS = SUB_SETTINGS_FREQUENCY_STARTUP_DELAY;
              } else if (LCD_BUTTON_SELECT()) {
                LCD_SUB_SETTINGS_ENABLE = SUB_SETTINGS_ENABLE_HYDRAULIC_1;
              } else if (LCD_BUTTON_STOP()) {
                LCD_SUB_SETTINGS = SUB_SETTINGS_SAVE;
              }

              break;

            case SUB_SETTINGS_ENABLE_HYDRAULIC_1:
              update_var = automatic_mode_hydraulic_used[1];
              if (CHECK_UPDATE_LCD()) {
                lcd_start();
                lcd.print("Enable hardware");
                lcd.setCursor(0, 1);
                lcd.print("Hydraulic 1:");
                if (update_var == 1) {
                  lcd.setCursor(13, 1);
                  lcd.print("On");
                } else if (update_var == 0) {
                  lcd.setCursor(13, 1);
                  lcd.print("Off");
                } else {
                  lcd.setCursor(13, 1);
                  lcd.print("ERROR LOADING STATUS");
                }
              }
              if (LCD_BUTTON_SELECT()) {
                update_var = 1;
                lcd.setCursor(13, 1);
                lcd.print("On ");
              } else if (LCD_BUTTON_STOP()) {
                update_var = 0;
                lcd.setCursor(13, 1);
                lcd.print("Off");
              } else if (LCD_BUTTON_DOWN()) {
                LCD_SUB_SETTINGS_ENABLE = SUB_SETTINGS_ENABLE_IDLE;
              } else if (LCD_BUTTON_UP()) {
                LCD_SUB_SETTINGS_ENABLE = SUB_SETTINGS_ENABLE_HYDRAULIC_2;
              }

              automatic_mode_hydraulic_used[1] = update_var;

              break;

            case SUB_SETTINGS_ENABLE_HYDRAULIC_2:

              update_var = automatic_mode_hydraulic_used[2];
              if (CHECK_UPDATE_LCD()) {
                lcd_start();
                lcd.print("Enable hardware");
                lcd.setCursor(0, 1);
                lcd.print("Hydraulic 2:");
                if (update_var == 1) {
                  lcd.setCursor(13, 1);
                  lcd.print("On");
                } else if (update_var == 0) {
                  lcd.setCursor(13, 1);
                  lcd.print("Off");
                } else {
                  lcd.setCursor(13, 1);
                  lcd.print("ERROR LOADING STATUS");
                }
              }
              if (LCD_BUTTON_SELECT()) {
                update_var = 1;
                lcd.setCursor(13, 1);
                lcd.print("On ");
              } else if (LCD_BUTTON_STOP()) {
                update_var = 0;
                lcd.setCursor(13, 1);
                lcd.print("Off");
              } else if (LCD_BUTTON_DOWN()) {
                LCD_SUB_SETTINGS_ENABLE = SUB_SETTINGS_ENABLE_HYDRAULIC_1;
              } else if (LCD_BUTTON_UP()) {
                LCD_SUB_SETTINGS_ENABLE = SUB_SETTINGS_ENABLE_HYDRAULIC_3;
              }

              automatic_mode_hydraulic_used[2] = update_var;
              break;

            case SUB_SETTINGS_ENABLE_HYDRAULIC_3:
              update_var = automatic_mode_hydraulic_used[3];
              if (CHECK_UPDATE_LCD()) {
                lcd_start();
                lcd.print("Enable hardware");
                lcd.setCursor(0, 1);
                lcd.print("Hydraulic 3:");
                if (update_var == 1) {
                  lcd.setCursor(13, 1);
                  lcd.print("On");
                } else if (update_var == 0) {
                  lcd.setCursor(13, 1);
                  lcd.print("Off");
                } else {
                  lcd.setCursor(13, 1);
                  lcd.print("ERROR LOADING STATUS");
                }
              }
              if (LCD_BUTTON_SELECT()) {
                update_var = 1;
                lcd.setCursor(13, 1);
                lcd.print("On ");
              } else if (LCD_BUTTON_STOP()) {
                update_var = 0;
                lcd.setCursor(13, 1);
                lcd.print("Off");
              } else if (LCD_BUTTON_DOWN()) {
                LCD_SUB_SETTINGS_ENABLE = SUB_SETTINGS_ENABLE_HYDRAULIC_2;
              } else if (LCD_BUTTON_UP()) {
                LCD_SUB_SETTINGS_ENABLE = SUB_SETTINGS_ENABLE_FREQUENCY_1;
              }

              automatic_mode_hydraulic_used[3] = update_var;

              break;

            case SUB_SETTINGS_ENABLE_FREQUENCY_1:
              update_var = automatic_mode_frequency_used[1];
              if (CHECK_UPDATE_LCD()) {
                lcd_start();
                lcd.print("Enable hardware");
                lcd.setCursor(0, 1);
                lcd.print("Frequency 1:");
                if (update_var == 1) {
                  lcd.setCursor(13, 1);
                  lcd.print("On");
                } else if (update_var == 0) {
                  lcd.setCursor(13, 1);
                  lcd.print("Off");
                } else {
                  lcd.setCursor(13, 1);
                  lcd.print("ERROR LOADING STATUS");
                }
              }
              if (LCD_BUTTON_SELECT()) {
                update_var = 1;
                lcd.setCursor(13, 1);
                lcd.print("On ");
              } else if (LCD_BUTTON_STOP()) {
                update_var = 0;
                lcd.setCursor(13, 1);
                lcd.print("Off");
              } else if (LCD_BUTTON_DOWN()) {
                LCD_SUB_SETTINGS_ENABLE = SUB_SETTINGS_ENABLE_HYDRAULIC_3;
              } else if (LCD_BUTTON_UP()) {
                LCD_SUB_SETTINGS_ENABLE = SUB_SETTINGS_ENABLE_FREQUENCY_2;
              }

              automatic_mode_frequency_used[1] = update_var;
              break;

            case SUB_SETTINGS_ENABLE_FREQUENCY_2:
              update_var = automatic_mode_frequency_used[2];
              if (CHECK_UPDATE_LCD()) {
                lcd_start();
                lcd.print("Enable hardware");
                lcd.setCursor(0, 1);
                lcd.print("Frequency 2:");
                if (update_var == 1) {
                  lcd.setCursor(13, 1);
                  lcd.print("On");
                } else if (update_var == 0) {
                  lcd.setCursor(13, 1);
                  lcd.print("Off");
                } else {
                  lcd.setCursor(13, 1);
                  lcd.print("ERROR LOADING STATUS");
                }
              }
              if (LCD_BUTTON_SELECT()) {
                update_var = 1;
                lcd.setCursor(13, 1);
                lcd.print("On ");
              } else if (LCD_BUTTON_STOP()) {
                update_var = 0;
                lcd.setCursor(13, 1);
                lcd.print("Off");
              } else if (LCD_BUTTON_DOWN()) {
                LCD_SUB_SETTINGS_ENABLE = SUB_SETTINGS_ENABLE_FREQUENCY_1;
              } else if (LCD_BUTTON_UP()) {
                LCD_SUB_SETTINGS_ENABLE = SUB_SETTINGS_ENABLE_FREQUENCY_3;
              }

              automatic_mode_frequency_used[2] = update_var;
              break;

            case SUB_SETTINGS_ENABLE_FREQUENCY_3:

              update_var = automatic_mode_frequency_used[3];
              if (CHECK_UPDATE_LCD()) {
                lcd_start();
                lcd.print("Enable hardware");
                lcd.setCursor(0, 1);
                lcd.print("Frequency 3:");
                if (update_var == 1) {
                  lcd.setCursor(13, 1);
                  lcd.print("On");
                } else if (update_var == 0) {
                  lcd.setCursor(13, 1);
                  lcd.print("Off");
                } else {
                  lcd.setCursor(13, 1);
                  lcd.print("ERROR LOADING STATUS");
                }
              }
              if (LCD_BUTTON_SELECT()) {
                update_var = 1;
                lcd.setCursor(13, 1);
                lcd.print("On ");
              } else if (LCD_BUTTON_STOP()) {
                update_var = 0;
                lcd.setCursor(13, 1);
                lcd.print("Off");
              } else if (LCD_BUTTON_DOWN()) {
                LCD_SUB_SETTINGS_ENABLE = SUB_SETTINGS_ENABLE_FREQUENCY_2;
              } else if (LCD_BUTTON_UP()) {
                LCD_SUB_SETTINGS_ENABLE = SUB_SETTINGS_ENABLE_RELAY_1;
              }

              automatic_mode_frequency_used[3] = update_var;
              break;

            case SUB_SETTINGS_ENABLE_RELAY_1:

              update_var = automatic_mode_other_used[1];
              if (CHECK_UPDATE_LCD()) {
                lcd_start();
                lcd.print("Enable hardware");
                lcd.setCursor(0, 1);
                lcd.print("Relay 1:");
                if (update_var == 1) {
                  lcd.setCursor(9, 1);
                  lcd.print("On");
                } else if (update_var == 0) {
                  lcd.setCursor(9, 1);
                  lcd.print("Off");
                } else {
                  lcd.setCursor(13, 1);
                  lcd.print("ERROR LOADING STATUS");
                }
              }
              if (LCD_BUTTON_SELECT()) {
                update_var = 1;
                lcd.setCursor(9, 1);
                lcd.print("On ");
              } else if (LCD_BUTTON_STOP()) {
                update_var = 0;
                lcd.setCursor(9, 1);
                lcd.print("Off");
              } else if (LCD_BUTTON_DOWN()) {
                LCD_SUB_SETTINGS_ENABLE = SUB_SETTINGS_ENABLE_FREQUENCY_3;
              } else if (LCD_BUTTON_UP()) {
                LCD_SUB_SETTINGS_ENABLE = SUB_SETTINGS_ENABLE_RELAY_2;
              }

              automatic_mode_other_used[1] = update_var;
              break;

            case SUB_SETTINGS_ENABLE_RELAY_2:
              update_var = automatic_mode_other_used[2];
              if (CHECK_UPDATE_LCD()) {
                lcd_start();
                lcd.print("Enable hardware");
                lcd.setCursor(0, 1);
                lcd.print("Relay 2:");
                if (update_var == 1) {
                  lcd.setCursor(9, 1);
                  lcd.print("On");
                } else if (update_var == 0) {
                  lcd.setCursor(9, 1);
                  lcd.print("Off");
                } else {
                  lcd.setCursor(13, 1);
                  lcd.print("ERROR LOADING STATUS");
                }
              }
              if (LCD_BUTTON_SELECT()) {
                update_var = 1;
                lcd.setCursor(9, 1);
                lcd.print("On ");
              } else if (LCD_BUTTON_STOP()) {
                update_var = 0;
                lcd.setCursor(9, 1);
                lcd.print("Off");
              } else if (LCD_BUTTON_DOWN()) {
                LCD_SUB_SETTINGS_ENABLE = SUB_SETTINGS_ENABLE_RELAY_1;
              } else if (LCD_BUTTON_UP()) {
                LCD_SUB_SETTINGS_ENABLE = SUB_SETTINGS_ENABLE_RELAY_3;
              }

              automatic_mode_other_used[2] = update_var;

              break;

            case SUB_SETTINGS_ENABLE_RELAY_3:
              update_var = automatic_mode_other_used[3];
              if (CHECK_UPDATE_LCD()) {
                lcd_start();
                lcd.print("Enable hardware");
                lcd.setCursor(0, 1);
                lcd.print("Relay 3:");
                if (update_var == 1) {
                  lcd.setCursor(9, 1);
                  lcd.print("On");
                } else if (update_var == 0) {
                  lcd.setCursor(9, 1);
                  lcd.print("Off");
                } else {
                  lcd.setCursor(13, 1);
                  lcd.print("ERROR LOADING STATUS");
                }
              }
              if (LCD_BUTTON_SELECT()) {
                update_var = 1;
                lcd.setCursor(9, 1);
                lcd.print("On ");
              } else if (LCD_BUTTON_STOP()) {
                update_var = 0;
                lcd.setCursor(9, 1);
                lcd.print("Off");
              } else if (LCD_BUTTON_DOWN()) {
                LCD_SUB_SETTINGS_ENABLE = SUB_SETTINGS_ENABLE_RELAY_2;
              } else if (LCD_BUTTON_UP()) {
                LCD_SUB_SETTINGS_ENABLE = SUB_SETTINGS_ENABLE_RELAY_4;
              }

              automatic_mode_other_used[3] = update_var;
              break;

            case SUB_SETTINGS_ENABLE_RELAY_4:
              update_var = automatic_mode_other_used[4];
              if (CHECK_UPDATE_LCD()) {
                lcd_start();
                lcd.print("Enable hardware");
                lcd.setCursor(0, 1);
                lcd.print("Relay 4:");
                if (update_var == 1) {
                  lcd.setCursor(9, 1);
                  lcd.print("On");
                } else if (update_var == 0) {
                  lcd.setCursor(9, 1);
                  lcd.print("Off");
                } else {
                  lcd.setCursor(13, 1);
                  lcd.print("ERROR LOADING STATUS");
                }
              }
              if (LCD_BUTTON_SELECT()) {
                update_var = 1;
                lcd.setCursor(9, 1);
                lcd.print("On ");
              } else if (LCD_BUTTON_STOP()) {
                update_var = 0;
                lcd.setCursor(9, 1);
                lcd.print("Off");
              } else if (LCD_BUTTON_DOWN()) {
                LCD_SUB_SETTINGS_ENABLE = SUB_SETTINGS_ENABLE_RELAY_3;
              } else if (LCD_BUTTON_UP()) {
                LCD_SUB_SETTINGS_ENABLE = SUB_SETTINGS_ENABLE_PLATFORM;
              }

              automatic_mode_other_used[4] = update_var;

              break;

            case SUB_SETTINGS_ENABLE_PLATFORM:
              update_var = automatic_mode_other_used[5];
              if (CHECK_UPDATE_LCD()) {
                lcd_start();
                lcd.print("Enable hardware");
                lcd.setCursor(0, 1);
                lcd.print("Platform:");
                if (update_var == 1) {
                  lcd.setCursor(10, 1);
                  lcd.print("On");
                } else if (update_var == 0) {
                  lcd.setCursor(10, 1);
                  lcd.print("Off");
                } else {
                  lcd.setCursor(10, 1);
                  lcd.print("ERROR LOADING STATUS");
                }
              }
              if (LCD_BUTTON_SELECT()) {
                update_var = 1;
                lcd.setCursor(10, 1);
                lcd.print("On ");
              } else if (LCD_BUTTON_STOP()) {
                update_var = 0;
                lcd.setCursor(10, 1);
                lcd.print("Off");
              } else if (LCD_BUTTON_DOWN()) {
                LCD_SUB_SETTINGS_ENABLE = SUB_SETTINGS_ENABLE_RELAY_4;
              } else if (LCD_BUTTON_UP()) {
                LCD_SUB_SETTINGS_ENABLE = SUB_SETTINGS_ENABLE_12V;
              }

              automatic_mode_other_used[5] = update_var;
              break;

            case SUB_SETTINGS_ENABLE_12V:

              update_var = automatic_mode_other_used[6];
              if (CHECK_UPDATE_LCD()) {
                lcd_start();
                lcd.print("Enable hardware");
                lcd.setCursor(0, 1);
                lcd.print("12V system:");
                if (update_var == 1) {
                  lcd.setCursor(12, 1);
                  lcd.print("On");
                } else if (update_var == 0) {
                  lcd.setCursor(12, 1);
                  lcd.print("Off");
                } else {
                  lcd.setCursor(12, 1);
                  lcd.print("ERROR LOADING STATUS");
                }
              }
              if (LCD_BUTTON_SELECT()) {
                update_var = 1;
                lcd.setCursor(12, 1);
                lcd.print("On ");
              } else if (LCD_BUTTON_STOP()) {
                update_var = 0;
                lcd.setCursor(12, 1);
                lcd.print("Off");
              } else if (LCD_BUTTON_DOWN()) {
                LCD_SUB_SETTINGS_ENABLE = SUB_SETTINGS_ENABLE_PLATFORM;
              } else if (LCD_BUTTON_UP()) {
                LCD_SUB_SETTINGS_ENABLE = SUB_SETTINGS_ENABLE_EMPTY;
              }

              automatic_mode_other_used[6] = update_var;
              break;

            case SUB_SETTINGS_ENABLE_EMPTY:

              update_var = automatic_mode_other_used[7];
              if (CHECK_UPDATE_LCD()) {
                lcd_start();
                lcd.print("Enable hardware");
                lcd.setCursor(0, 1);
                lcd.print("EMPTY:");
                if (update_var == 1) {
                  lcd.setCursor(7, 1);
                  lcd.print("On");
                } else if (update_var == 0) {
                  lcd.setCursor(7, 1);
                  lcd.print("Off");
                } else {
                  lcd.setCursor(7, 1);
                  lcd.print("ERROR LOADING STATUS");
                }
              }
              if (LCD_BUTTON_SELECT()) {
                update_var = 1;
                lcd.setCursor(7, 1);
                lcd.print("On ");
              } else if (LCD_BUTTON_STOP()) {
                update_var = 0;
                lcd.setCursor(7, 1);
                lcd.print("Off");
              } else if (LCD_BUTTON_DOWN()) {
                LCD_SUB_SETTINGS_ENABLE = SUB_SETTINGS_ENABLE_PLATFORM;
              } else if (LCD_BUTTON_UP()) {
                LCD_SUB_SETTINGS_ENABLE = SUB_SETTINGS_ENABLE_HYDRAULIC_1;
              }

              automatic_mode_other_used[7] = update_var;
              break;

          }

          break;
        /***************************************************************************************/
        case SUB_SETTINGS_HOMEING:

          if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("Homeing:");
            lcd.setCursor(0, 1);
            if (automatic_mode_homeing_enable) {

              lcd.print("Enabled   ");
            } else if (!automatic_mode_homeing_enable) {

              lcd.print("Disabled");
            } else {

              lcd.print("ERROR LOADING STATUS");
            }
          }

          if (LCD_BUTTON_SELECT()) {
            automatic_mode_homeing_enable = true;
            lcd.setCursor(0, 1);
            lcd.print("Enabled   ");
          } else if (LCD_BUTTON_STOP()) {
            automatic_mode_homeing_enable = false;
            lcd.setCursor(0, 1);
            lcd.print("Disabled");
          } else if (LCD_BUTTON_DOWN()) {
            LCD_SUB_SETTINGS = SUB_SETTINGS_ENABLE_FEATURE;
          } else if (LCD_BUTTON_UP()) {
            LCD_SUB_SETTINGS = SUB_SETTINGS_DEFAULT;
          }

          break;
        /***************************************************************************************/
        case SUB_SETTINGS_DEFAULT:
          if (CHECK_UPDATE_LCD()) {
            lcd_start();
            lcd.print("Back to default?");
          }

          if (LCD_BUTTON_SELECT()) {

            determine_arrays(true);

          } else if (LCD_BUTTON_DOWN()) {
            LCD_SUB_SETTINGS = SUB_SETTINGS_HOMEING;


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
  } else if (LCD_SUB_MANUAL_OTHER != LCD_SUB_MANUAL_OTHER_OLD) {
    LCD_SUB_MANUAL_OTHER_OLD = LCD_SUB_MANUAL_OTHER;
    return true;
  } else if (sub_manual_other_component_nr != sub_manual_other_component_nr_old) {
    sub_manual_other_component_nr_old = sub_manual_other_component_nr;
    return true;
  } else if (LCD_SUB_SETTINGS_ENABLE != LCD_SUB_SETTINGS_ENABLE_OLD) {
    LCD_SUB_SETTINGS_ENABLE_OLD = LCD_SUB_SETTINGS_ENABLE;
    return true;
  }
  else {
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
        lcd.print("PRESS STOP TO RESET ERROR");
      }

      if (LCD_BUTTON_STOP()) {
        state_idle();
        error_nr = 0;
        lcd_start();
        lcd.print("ERROR RESET");
        delay(2000);
      }
      break;

    case 2:
      if (CHECK_UPDATE_LCD()) {
        lcd_start();
        stop_all_hydraulic();
        lcd.print("WRONG 12V SYSTEM COMMAND");
        lcd.setCursor(0, 1);
        lcd.print("PRESS STOP TO RESET ERROR");
      }

      if (LCD_BUTTON_STOP()) {
        state_idle();
        error_nr = 0;
        lcd_start();
        lcd.print("ERROR RESET");
        delay(2000);
      }
      break;

    case 3:
      if (CHECK_UPDATE_LCD()) {
        lcd_start();
        stop_all_hydraulic();
        lcd.print("WRONG 3 FASE RELAY COMMAND");
        lcd.setCursor(0, 1);
        lcd.print("PRESS STOP TO RESET ERROR");
      }

      if (LCD_BUTTON_STOP()) {
        state_idle();
        error_nr = 0;
        lcd_start();
        lcd.print("ERROR RESET");
        delay(2000);
      }
      break;

    case 4:
      if (CHECK_UPDATE_LCD()) {
        lcd_start();
        stop_all_hydraulic();
        lcd.print("WRONG STEWARDPLATFORM COMMAND");
        lcd.setCursor(0, 1);
        lcd.print("PRESS STOP TO RESET ERROR");
      }

      if (LCD_BUTTON_STOP()) {
        state_idle();
        error_nr = 0;
        lcd_start();
        lcd.print("ERROR RESET");
        delay(2000);
      }
      break;
  }

}



/************************************************************************************/
void state_idle()
/************************************************************************************/
{
  LCD_SUB_RUN = SUB_RUN_IDLE;
  LCD_SUB_MANUAL_HYDRAULIC = SUB_MANUAL_HYDRAULIC_IDLE;
  LCD_SUB_MANUAL_FRQ = SUB_MANUAL_FRQ_IDLE;
  LCD_SUB_MANUAL_OTHER = SUB_MANUAL_OTHER_IDLE;
  LCD_SUB_SETTINGS = SUB_SETTINGS_IDLE;
  MASTER_STATE = MASTER_STATE_IDLE;
  MANUAL_CONTROLE_VALVE_STATE = MANUAL_CONTROLE_VALVE_IDLE;
  AUTOMATIC_MODE_STATE = AUTOMATIC_MODE_IDLE;

}
