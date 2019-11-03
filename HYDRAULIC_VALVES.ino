/************************************************************************************/
void set_hydraulic_valve(int nr, bool state, bool reset)
/************************************************************************************/
{
  Serial.print(nr);
  Serial.print("   ");
  Serial.println(hydraulic_state[nr]);

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

      if ((state == true) ||(state == 3)) {

        digitalWrite(hydraulic_open[nr], HIGH);
        digitalWrite(hydraulic_close[nr], LOW);
        hydraulic_state[nr] = true;

#if defined(DEBUG_HYDRAULIC)
        Serial.print("Open hydraulic valve nr: ");
        Serial.println(nr);
#endif

      }
      else if ((state == false) ||(state == 3)) {

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
