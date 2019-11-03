/************************************************************************************/
void CYCLE_1()
/************************************************************************************/
{
  set_hydraulic_valve(1, true, false);
  set_hydraulic_valve(2, true, false);
  set_hydraulic_valve(3, true, false);
}

/************************************************************************************/
void CYCLE_2()
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
void CYCLE_3()
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

  switch (CYCLE_STATE) {

    case 0:  // idle

      break;

    case 1:  // cycle 1
      CYCLE_1();
      break;

    case 2:  // cycle 2
      CYCLE_2();
      break;

    case 3:  // cycle 3
      CYCLE_3();
      break;
  }
}
