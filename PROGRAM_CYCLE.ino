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
