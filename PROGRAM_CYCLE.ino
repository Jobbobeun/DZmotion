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

void manual_control_valve() {

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
