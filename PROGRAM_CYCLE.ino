/************************************************************************************/
void CYCLE_FUNCTION_1()
/************************************************************************************/
{
  set_hydraulic_valve(1, true, false);
  set_hydraulic_valve(2, true, false);
  set_hydraulic_valve(3, true, false);
}

/************************************************************************************/
void CYCLE_FUNCTION_2()
/************************************************************************************/
{

  set_frequency_inverter(1, true, true);
  set_frequency_inverter(2, true, true);
  set_frequency_inverter(3, true, true);
  set_hydraulic_valve(1, false, false);
  set_hydraulic_valve(2, false, false);
  set_hydraulic_valve(3, false, false);

}

/************************************************************************************/
void CYCLE_FUNCTION_3()
/************************************************************************************/
{

  set_frequency_inverter(1, false, false);
  set_frequency_inverter(2, false, false);
  set_frequency_inverter(3, false, false);
  set_hydraulic_valve(1, true, true);
  set_hydraulic_valve(2, true, true);
  set_hydraulic_valve(3, true, true);

}

/************************************************************************************/
void cycle_state()
/************************************************************************************/
{

  switch (MASTER_STATE) {

    case MASTER_STATE_IDLE:  // idle


      if (AUTOMATIC_CYCLE_START == true) {
        MASTER_STATE = MASTER_STATE_START;
      }
      break;

    case MASTER_STATE_START:

      if (AUTOMATIC_CYCLE_COUNTER < 5000) {
        set_hydraulic_valve(1, true, false);
        set_hydraulic_valve(2, true, false);
        set_hydraulic_valve(3, true, false);
        AUTOMATIC_CYCLE_COUNTER ++;
      }
      else {
        MASTER_STATE = MASTER_STATE_CYCLE_1;
        AUTOMATIC_CYCLE_COUNTER = 0;
      }


      break;

    case MASTER_STATE_CYCLE_1:  // cycle 1

      if (AUTOMATIC_CYCLE_COUNTER < 5000) {
        set_frequency_inverter(1, true, true);
        set_frequency_inverter(2, true, true);
        set_frequency_inverter(3, true, true);
        set_hydraulic_valve(1, false, false);
        set_hydraulic_valve(2, false, false);
        set_hydraulic_valve(3, false, false);
        AUTOMATIC_CYCLE_COUNTER ++;
      }
      else {
        MASTER_STATE = MASTER_STATE_STOP;
        AUTOMATIC_CYCLE_COUNTER = 0;
      }

      break;

    case MASTER_STATE_CYCLE_2:  // cycle 2
      CYCLE_FUNCTION_2();
      break;

    case MASTER_STATE_CYCLE_3:  // cycle 3
      CYCLE_FUNCTION_3();
      break;

    case MASTER_STATE_STOP:
      set_frequency_inverter(1, false, false);
      set_frequency_inverter(2, false, false);
      set_frequency_inverter(3, false, false);
      set_hydraulic_valve(1, true, true);
      set_hydraulic_valve(2, true, true);
      set_hydraulic_valve(3, true, true);
      MASTER_STATE = MASTER_STATE_IDLE;

      break;
  }

}

/************************************************************************************/
void manual_control_valve()
/************************************************************************************/
{

#if defined DEBUG_MANUAL_CONTROLE
  Serial.println(MANUAL_CONTROLE_VALVE_STATE);
  Serial.println("MANUAL_CONTROLE_VALVE FUNCTION");
#endif
  switch (MANUAL_CONTROLE_VALVE_STATE) {

    case MANUAL_CONTROLE_VALVE_IDLE:

      break;

    case MANUAL_CONTROLE_VALVE_OUT:
#if defined DEBUG_MANUAL_CONTROLE
      Serial.println("MANUAL_CONTROLE_VALVE_OUT STATE");
#endif
      set_hydraulic_valve(sub_manual_cylinder_nr, true, false);
      MANUAL_CONTROLE_VALVE_STATE = MANUAL_CONTROLE_VALVE_STOP;

      break;

    case MANUAL_CONTROLE_VALVE_IN:
#if defined DEBUG_MANUAL_CONTROLE
      Serial.println("MANUAL_CONTROLE_VALVE_IN STATE");
#endif
      set_hydraulic_valve(sub_manual_cylinder_nr, false, false);
      MANUAL_CONTROLE_VALVE_STATE = MANUAL_CONTROLE_VALVE_STOP;
      break;

    case MANUAL_CONTROLE_VALVE_STOP:
#if defined DEBUG_MANUAL_CONTROLE
      Serial.println("MANUAL_CONTROLE_VALVE_STOP STATE");
#endif
      set_hydraulic_valve(sub_manual_cylinder_nr, false, true);
      MANUAL_CONTROLE_VALVE_STATE = MANUAL_CONTROLE_VALVE_IDLE;

      break;
  }

}

/************************************************************************************/
void Test_cycle()
/************************************************************************************/
{
  if (test_cycle_start) {

    for (int i = 1 ; i < (cylinder_amount + 1); i++) {

      if (check_hydraulic_time(i) == false) {

        troggel_hydraulic_direction(i);
        hydraulic_actual_time[i] = 0;

      } else {

        if (cycle_test_approve_counter[i] == large_number_time_delay) { // solved issue with large numbers

          hydraulic_actual_time[i]++;

          cycle_test_approve_counter[i] = 0;

        } else {
          cycle_test_approve_counter[i]++ ;
        }

      }

    }
  } else {
    for (int i = 1 ; i < (cylinder_amount + 1); i++) {
      set_hydraulic_valve(i, true, false);
      test_cycle_start = true;
    }

  }
}

/************************************************************************************/
void automatic_mode()
/************************************************************************************/
{


  switch (AUTOMATIC_MODE_STATE) {

    /*---------------------------------------IDLE-----------------------------------*/
    case AUTOMATIC_MODE_IDLE:

      break;
    /*--------------------------------------START------------------------------------*/
    case AUTOMATIC_MODE_START:    // set all back to home position

      automatic_mode_state = false;
      lcd.setCursor(0, 1);
      lcd.print("Homeing");
#if defined(DEBUG_AUTOMATIC_MODE)
      Serial.println("automatic mode start");
#endif

      for (int timer = 0; timer < automatic_homeing_time ; timer++)
      {

#if defined(DEBUG_AUTOMATIC_MODE)
        Serial.print("Timer: ");
        Serial.println(timer);
#endif

        for (int i = 0 ; i < (cylinder_amount + 1); i ++) {
          set_hydraulic_valve(i, true, false);
        }

      }
      AUTOMATIC_MODE_STATE = AUTOMATIC_MODE_RUN;
      automatic_mode_state = true;
      lcd.setCursor(0, 1);
      lcd.print("RUN    ");
      break;

    /*--------------------------------------RUN--------------------------------------*/
    case AUTOMATIC_MODE_RUN:

      if (LCD_BUTTON_STOP()) {

        for (int i = 0; i < (cylinder_amount + 1); i++) {
          set_hydraulic_valve(i, false, true);
        }

        for (int z = 0; z < (frequency_inverter_amount + 1) ; z++) {
          set_frequency_inverter(z, false, false);
        }

        automatic_frequency_inverter_running = false;
        automatic_frequency_inverter_startup_delay = 0;
        AUTOMATIC_MODE_STATE = AUTOMATIC_MODE_STOP;

      }  else  if (LCD_BUTTON_PAUSE()) {
        lcd.setCursor(0, 1);
        lcd.print("PAUSE");
        for (int i = 0; i < (cylinder_amount + 1); i++) {
          set_hydraulic_valve(i, false, true);
        }

        for (int z = 0; z < (frequency_inverter_amount + 1) ; z++) {
          set_frequency_inverter(z, false, false);
        }
        AUTOMATIC_MODE_STATE = AUTOMATIC_MODE_PAUSE;
        automatic_frequency_inverter_running = false;
        automatic_frequency_inverter_startup_delay = 0;

      } else {


        // Hydraulic valve control
        for (int i = 1 ; i < (cylinder_amount + 1); i++) {

          if (check_hydraulic_time(i) == false) {

            troggel_hydraulic_direction(i);
            hydraulic_actual_time[i] = 0;

          } else {

            if (cycle_test_approve_counter[i] == large_number_time_delay) { // solved issue with large numbers

              hydraulic_actual_time[i]++;

              cycle_test_approve_counter[i] = 0;

            } else {
              cycle_test_approve_counter[i]++ ;
            }
          }
        }

        // Frequency inverter control

        if (!automatic_frequency_inverter_running) {
          //Serial.println(frequency_invertor_startup_delay);
          automatic_frequency_inverter_startup_delay ++;
          int frequency_invertor_startup_delay2 = frequency_invertor_startup_delay * 2;
          int frequency_invertor_startup_delay3 = frequency_invertor_startup_delay * 3;

          if (automatic_frequency_inverter_startup_delay == frequency_invertor_startup_delay) {
            //Serial.println("Start f 1");
            set_frequency_inverter(1, true, false);

          } else if (automatic_frequency_inverter_startup_delay == frequency_invertor_startup_delay2) {
            //Serial.println("Start f 2");
            set_frequency_inverter(2, true, false);

          } else if (automatic_frequency_inverter_startup_delay == frequency_invertor_startup_delay3) {
            //Serial.println("Start f 3");
            set_frequency_inverter(3, true, false);
            automatic_frequency_inverter_running = true;
            automatic_frequency_inverter_startup_delay = 0;

          }
        }
      }


      break;

    /*-----------------------------------------PAUSE---------------------------------*/
    case AUTOMATIC_MODE_PAUSE:
      if (LCD_BUTTON_STOP()) {

        AUTOMATIC_MODE_STATE = AUTOMATIC_MODE_STOP;
        automatic_mode_pause_time_counter = 0;

      } else {

        if (automatic_mode_pause_time_counter == automatic_mode_pause_time) {
          automatic_mode_pause_time_counter = 0;
          AUTOMATIC_MODE_STATE = AUTOMATIC_MODE_RUN;

          for (int i = 0 ; i < (cylinder_amount + 1); i ++) {
            set_hydraulic_valve(i, false, false);
            lcd.setCursor(0, 1);
            lcd.print("RUN  ");
          }

        } else {
          automatic_mode_pause_time_counter ++;
        }
      }
      break;

    /*-----------------------------------------STOP----------------------------------*/
    case AUTOMATIC_MODE_STOP:

      AUTOMATIC_MODE_STATE = AUTOMATIC_MODE_IDLE;
      automatic_mode_state = false;
      break;
  }

}

/************************************************************************************/
void automatic_mode_2020()
/************************************************************************************/
{

  switch (AUTOMATIC_MODE_2020) {
    /*-----------------------------------------IDLE-----------------------------------*/
    case AUTOMATIC_MODE_2020_IDLE:


      break;
    /*-----------------------------------------START----------------------------------*/
    case AUTOMATIC_MODE_2020_START:

      if (CHECK_UPDATE_LCD()) {

        lcd.setCursor(0, 1);
        lcd.print("Start:");
      }

      switch (AUTOMATIC_MODE_2020_STARTUP) {

        case AUTOMATIC_MODE_20202_STARTUP_FR1:

          if (CHECK_UPDATE_LCD()) {
            Serial.println("LCD SET");
            lcd.setCursor(0, 2);
            lcd.print("Frequency inverter1");
          }

          if ((automatic_mode_frequency_used[1] == 1) && (frequency_invertor_state[1] == 0)) {

            if (automatic_mode_2020_startup_delay_timer == automatic_mode_2020_startup_delay) {

              set_frequency_inverter(1, true, false);
              automatic_mode_2020_startup_delay_timer = 0;
              AUTOMATIC_MODE_2020_STARTUP = AUTOMATIC_MODE_20202_STARTUP_FR2;

            } else {
              automatic_mode_2020_startup_delay_timer ++;

            }
          } else {

            AUTOMATIC_MODE_2020_STARTUP = AUTOMATIC_MODE_20202_STARTUP_FR2;

          }

          break;
        case AUTOMATIC_MODE_20202_STARTUP_FR2:
          if (CHECK_UPDATE_LCD()) {
            lcd.setCursor(0, 2);
            lcd.print("Frequency inverter2");
          }
          if ((automatic_mode_frequency_used[2] == 1) && (frequency_invertor_state[2] == 0)) {

            if (automatic_mode_2020_startup_delay_timer == automatic_mode_2020_startup_delay) {

              set_frequency_inverter(2, true, false);
              automatic_mode_2020_startup_delay_timer = 0;
              AUTOMATIC_MODE_2020_STARTUP = AUTOMATIC_MODE_20202_STARTUP_FR3;

            } else {
              automatic_mode_2020_startup_delay_timer ++;

            }
          } else {

            AUTOMATIC_MODE_2020_STARTUP = AUTOMATIC_MODE_20202_STARTUP_FR3;

          }
          break;
        case AUTOMATIC_MODE_20202_STARTUP_FR3:
          if (CHECK_UPDATE_LCD()) {
            lcd.setCursor(0, 2);
            lcd.print("Frequency inverter3");
          }
          if ((automatic_mode_frequency_used[3] == 1) && (frequency_invertor_state[3] == 0)) {

            if (automatic_mode_2020_startup_delay_timer == automatic_mode_2020_startup_delay) {

              set_frequency_inverter(3, true, false);
              automatic_mode_2020_startup_delay_timer = 0;
              AUTOMATIC_MODE_2020_STARTUP = AUTOMATIC_MODE_20202_STARTUP_RELAY1;

            } else {
              automatic_mode_2020_startup_delay_timer ++;

            }
          } else {

            AUTOMATIC_MODE_2020_STARTUP = AUTOMATIC_MODE_20202_STARTUP_RELAY1;

          }
          break;
        case AUTOMATIC_MODE_20202_STARTUP_RELAY1:
          if (CHECK_UPDATE_LCD()) {
            lcd.setCursor(0, 2);
            lcd.print("Relay 1");
          }
          if ((automatic_mode_other_used[1] == 1) && (relay_state[1] == 0)) {

            if (automatic_mode_2020_startup_delay_timer == automatic_mode_2020_startup_delay) {

              controle_relay(1, true);
              automatic_mode_2020_startup_delay_timer = 0;
              AUTOMATIC_MODE_2020_STARTUP = AUTOMATIC_MODE_20202_STARTUP_RELAY2;

            } else {
              automatic_mode_2020_startup_delay_timer ++;

            }
          } else {

            AUTOMATIC_MODE_2020_STARTUP = AUTOMATIC_MODE_20202_STARTUP_RELAY2;

          }

          break;
        case AUTOMATIC_MODE_20202_STARTUP_RELAY2:
          if (CHECK_UPDATE_LCD()) {
            lcd.setCursor(0, 2);
            lcd.print("Relay 2");
          }
          if ((automatic_mode_other_used[2] == 1) && (relay_state[2] == 0)) {

            if (automatic_mode_2020_startup_delay_timer == automatic_mode_2020_startup_delay) {

              controle_relay(2, true);
              automatic_mode_2020_startup_delay_timer = 0;
              AUTOMATIC_MODE_2020_STARTUP = AUTOMATIC_MODE_20202_STARTUP_RELAY3;

            } else {
              automatic_mode_2020_startup_delay_timer ++;

            }
          } else {

            AUTOMATIC_MODE_2020_STARTUP = AUTOMATIC_MODE_20202_STARTUP_RELAY3;

          }

          break;
        case AUTOMATIC_MODE_20202_STARTUP_RELAY3:
          if (CHECK_UPDATE_LCD()) {
            lcd.setCursor(0, 2);
            lcd.print("Relay 3");
          }
          if ((automatic_mode_other_used[3] == 1) && (relay_state[3] == 0)) {

            if (automatic_mode_2020_startup_delay_timer == automatic_mode_2020_startup_delay) {

              controle_relay(3, true);
              automatic_mode_2020_startup_delay_timer = 0;
              AUTOMATIC_MODE_2020_STARTUP = AUTOMATIC_MODE_20202_STARTUP_RELAY4;

            } else {
              automatic_mode_2020_startup_delay_timer ++;

            }
          } else {

            AUTOMATIC_MODE_2020_STARTUP = AUTOMATIC_MODE_20202_STARTUP_RELAY4;

          }

          break;
        case AUTOMATIC_MODE_20202_STARTUP_RELAY4:
          if (CHECK_UPDATE_LCD()) {
            lcd.setCursor(0, 2);
            lcd.print("Relay 4");
          }
          if ((automatic_mode_other_used[4] == 1) && (relay_state[4] == 0)) {

            if (automatic_mode_2020_startup_delay_timer == automatic_mode_2020_startup_delay) {

              controle_relay(4, true);
              automatic_mode_2020_startup_delay_timer = 0;
              AUTOMATIC_MODE_2020_STARTUP = AUTOMATIC_MODE_20202_STARTUP_PLATFORM;

            } else {
              automatic_mode_2020_startup_delay_timer ++;

            }
          } else {

            AUTOMATIC_MODE_2020_STARTUP = AUTOMATIC_MODE_20202_STARTUP_PLATFORM;

          }

          break;
        case AUTOMATIC_MODE_20202_STARTUP_PLATFORM:     // used without timer
          if (CHECK_UPDATE_LCD()) {
            lcd.setCursor(0, 2);
            lcd.print("Stewartplatform");
          }
          if ((automatic_mode_other_used[5] == 1) && (stewardplatform_state == false)) {

            controle_stewardplatform(true);
            automatic_mode_2020_startup_delay_timer = 0;
            AUTOMATIC_MODE_2020_STARTUP = AUTOMATIC_MODE_20202_STARTUP_12V;


          } else {

            AUTOMATIC_MODE_2020_STARTUP = AUTOMATIC_MODE_20202_STARTUP_12V;

          }

          break;
        case AUTOMATIC_MODE_20202_STARTUP_12V:            // used without timer
          if (CHECK_UPDATE_LCD()) {
            lcd.setCursor(0, 2);
            lcd.print("Start 12V system ");
          }
          if ((automatic_mode_other_used[6] == 1) && (system_12V_state == false)) {


            controle_12v_system(true);
            automatic_mode_2020_startup_delay_timer = 0;
            AUTOMATIC_MODE_2020_STARTUP = AUTOMATIC_MODE_20202_STARTUP_EMPTY;


          } else {

            AUTOMATIC_MODE_2020_STARTUP = AUTOMATIC_MODE_20202_STARTUP_EMPTY;

          }

          break;
        case AUTOMATIC_MODE_20202_STARTUP_EMPTY:
          if (CHECK_UPDATE_LCD()) {
            lcd.setCursor(0, 2);
            lcd.print("Start Empty relay");
          }
          if ((automatic_mode_other_used[7] == 1) && (empty_relay_state == false)) {

            if (automatic_mode_2020_startup_delay_timer == automatic_mode_2020_startup_delay) {

              controle_empty_relay(true);
              automatic_mode_2020_startup_delay_timer = 0;
              AUTOMATIC_MODE_2020_STARTUP = AUTOMATIC_MODE_20202_STARTUP_FR1;
              AUTOMATIC_MODE_2020 = AUTOMATIC_MODE_2020_RUN;
            } else {
              automatic_mode_2020_startup_delay_timer ++;

            }
          } else {

            AUTOMATIC_MODE_2020_STARTUP = AUTOMATIC_MODE_20202_STARTUP_FR1;
            AUTOMATIC_MODE_2020 = AUTOMATIC_MODE_2020_RUN;
          }

          break;
      }

      break;
    /*------------------------------------------RUN-----------------------------------*/
    case AUTOMATIC_MODE_2020_RUN:
      if (CHECK_UPDATE_LCD()) {
        lcd.setCursor(0, 2);
        lcd.print("Wait for pause signa");
      }
      if (LCD_BUTTON_PAUSE()) {
        controle_empty_relay(true);
        AUTOMATIC_MODE_2020 = AUTOMATIC_MODE_2020_PAUSE;
      }

      break;
    /*-----------------------------------------PAUSE----------------------------------*/
    case AUTOMATIC_MODE_2020_PAUSE:
      if (CHECK_UPDATE_LCD()) {
        lcd.setCursor(0, 2);
        lcd.print("Pause               ");
      }
      if (automatic_mode_2020_pause_flashlight_timer == automatic_mode_2020_pause_flashlight_timer_delay) {
        if (!automatic_mode_2020_pause) {
          stop_all();
          automatic_mode_2020_pause = true;
        }

        if (automatic_mode_2020_pause_timer == automatic_mode_2020_pause_timer_delay) {
          AUTOMATIC_MODE_2020 = AUTOMATIC_MODE_2020_START;
          automatic_mode_2020_pause_timer = 0;
          automatic_mode_2020_pause_flashlight_timer = 0;
          automatic_mode_2020_pause = false;
        } else {
          automatic_mode_2020_pause_timer++;
        }
      } else {
        automatic_mode_2020_pause_flashlight_timer ++;
      }


      break;
    /*-----------------------------------------STOP-----------------------------------*/
    case AUTOMATIC_MODE_2020_STOP:

      stop_all();
      AUTOMATIC_MODE_2020 = AUTOMATIC_MODE_2020_IDLE;
      break;
  }


}
// automatic_mode_hydraulic_used[3] // 1-2-3
//automatic_mode_frequency_used[1] // 1-2-3
//automatic_mode_other_used[1] // 1-2-3-4-5-6-7

void TEST_function() {
  int state = 1;

  for (int i = 1 ; i < 5 ; i ++) {
    digitalWrite(fase_relay[i], state);
    digitalWrite(hydraulic_open[i], state);
    digitalWrite(hydraulic_close[i], state);
    digitalWrite(frequency_invertor_on_off[i], state);
  }
  digitalWrite(small_motor_system, state);
  digitalWrite(stewardplatform, state);
  digitalWrite(empty_relay, state);

}

void stop_all() {

  for (int i = 0; i < 5 ; i++) {
    if (frequency_invertor_state[i] == 1) {
      set_frequency_inverter(i, false, false);
    }
    if (relay_state[i] == 1) {
      controle_relay(i, false);
    }
  }
  if (stewardplatform_state) {
    controle_stewardplatform(false);
  }
  if (system_12V_state) {
    controle_12v_system(false);
  }

  if (empty_relay_state ) {
    controle_empty_relay(false);
  }
}
