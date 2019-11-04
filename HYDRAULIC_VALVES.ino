/************************************************************************************/
void set_hydraulic_valve(int nr, bool state, bool reset)
/************************************************************************************/
{
  if (nr == 1) {
    set_hydraulic_valve_4_3(nr, state, reset);
  } else if (nr == 2) {
    set_hydraulic_valve_2_2(nr, state, reset);
  }
  else if (nr == 3) {
    set_hydraulic_valve_2_2(nr, state, reset);
  }

}

/************************************************************************************/
void set_hydraulic_valve_2_2(int nr, bool state, bool reset)    // State true = Open, False = close --> Hydraulic_valve_open = direction  --> Hydraulic_valve_close = flow.
/************************************************************************************/
{
  if (reset == true) {
    digitalWrite(hydraulic_close[nr], LOW);
    hydraulic_state[nr] = 3;
  }
  else {
    digitalWrite(hydraulic_close[nr], HIGH);
    if (hydraulic_state[nr] != state) {

      if (state == true)    // open
      {

        digitalWrite(hydraulic_open[nr], HIGH);
        hydraulic_state[nr] = true;

      } else if (state == false)  // close
      {

        digitalWrite(hydraulic_open[nr], LOW);
        hydraulic_state[nr] = false;

      }

    }
  }
}

/************************************************************************************/
void set_hydraulic_valve_4_3(int nr, bool state, bool reset)    // State true = Open, False = close
/************************************************************************************/
{

  if (reset == true) {

    digitalWrite(hydraulic_open[nr], LOW);
    digitalWrite(hydraulic_close[nr], LOW);
    hydraulic_state[nr] = 3;


#if defined(DEBUG_HYDRAULIC)
    Serial.print("Reset hydraulic valve nr: ");
    Serial.print(nr);
#endif
  }
  else {

    if (hydraulic_state[nr] != state) {

      if (state == true) {  // open

        digitalWrite(hydraulic_open[nr], HIGH);
        digitalWrite(hydraulic_close[nr], LOW);
        hydraulic_state[nr] = true;

#if defined(DEBUG_HYDRAULIC)
        Serial.print("Open hydraulic valve nr: ");
        Serial.println(nr);
#endif

      }
      else if (state == false) {  // close

        digitalWrite(hydraulic_open[nr], LOW);
        digitalWrite(hydraulic_close[nr], HIGH);
        hydraulic_state[nr] = false;

#if defined(DEBUG_HYDRAULIC)
        Serial.print("Close hydraulic valve nr: ");
        Serial.println(nr);
#endif

      }
    }
  }
}

/************************************************************************************/
void check_hydraulic_time(int nr, int max_time)
/************************************************************************************/
{

}
