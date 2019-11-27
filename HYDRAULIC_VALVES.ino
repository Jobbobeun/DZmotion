/************************************************************************************/
void set_hydraulic_valve(int nr, bool state, bool reset) // define which cylinder which control needed. New cylinders need to be add here.
/************************************************************************************/
{
  if (nr == 1) {
    set_hydraulic_valve_4_3(nr, state, reset);
#if defined(DEBUG_HYDRAULIC)
    Serial.println("SET VALVE 1");
#endif
  } else if (nr == 2) {
    set_hydraulic_valve_2_2(nr, state, reset);
#if defined(DEBUG_HYDRAULIC)
    Serial.println("SET VALVE 2");
#endif
  }
  else if (nr == 3) {
    set_hydraulic_valve_2_2(nr, state, reset);
#if defined(DEBUG_HYDRAULIC)
    Serial.println("SET VALVE 3");
#endif
  }

}

/************************************************************************************/
void set_hydraulic_valve_2_2(int nr, bool state, bool reset)    // State true = Open, False = close --> Hydraulic_valve_open = direction  --> Hydraulic_valve_close = flow.
/************************************************************************************/
{

  if (reset == true) {
    digitalWrite(hydraulic_close[nr], LOW);
    digitalWrite(hydraulic_open[nr], LOW);
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
bool check_hydraulic_time(int nr) // true = smaler then actual time, false = bigger then actual time
/************************************************************************************/
{

  if (hydraulic_time[nr] < hydraulic_actual_time[nr]) {

#if defined(DEBUG_HYDRAULIC)
    Serial.println("Over time");
#endif

    return false;

  } else {

#if defined(DEBUG_HYDRAULIC)
    Serial.print("Time to go:");
    Serial.println(hydraulic_time[nr] - hydraulic_actual_time[nr]);
#endif

    return true;

  }
}

/************************************************************************************/
void troggel_hydraulic_direction(int nr)
/************************************************************************************/
{
  if (hydraulic_state[nr] == true) {

    set_hydraulic_valve(nr, false, false);

#if defined(DEBUG_HYDRAULIC)
    Serial.println("Troggeld to close");
#endif

  } else if (hydraulic_state[nr] == false) {

    set_hydraulic_valve(nr, true, false);

#if defined(DEBUG_HYDRAULIC)
    Serial.println("Troggeld to open");
#endif

  }else if (hydraulic_state[nr] == 3) {
  error_nr = 1;
  }


}

/************************************************************************************/
void stop_all_hydraulic()
/************************************************************************************/
{
  
    for (int i = 1 ; i < (cylinder_amount + 1) ; i++){

    set_hydraulic_valve(i , false, true);
    }
}
