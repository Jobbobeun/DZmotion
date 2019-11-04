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
      set_frequency_inverter(1, false, false);
      set_frequency_inverter(2, false, false);
      set_frequency_inverter(3, false, false);
      set_hydraulic_valve(1, true, true);
      set_hydraulic_valve(2, true, true);
      set_hydraulic_valve(3, true, true);

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
        MASTER_STATE = MASTER_STATE_IDLE;
        AUTOMATIC_CYCLE_COUNTER = 0;
      }

      break;

    case MASTER_STATE_CYCLE_2:  // cycle 2
      CYCLE_FUNCTION_2();
      break;

    case MASTER_STATE_CYCLE_3:  // cycle 3
      CYCLE_FUNCTION_3();
      break;
  }

}
