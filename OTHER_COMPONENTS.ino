/************************************************************************************/
void controle_12v_system(bool state)
/************************************************************************************/
{
  if (state == true) {
    digitalWrite(small_motor_system, HIGH);
    system_12V_state = true;
  } else if (state == false) {
    digitalWrite(small_motor_system, LOW);
    system_12V_state = false;
  } else {
    error_nr = 2;
  }
}

/************************************************************************************/
void controle_relay(int relay_number, bool state)
/************************************************************************************/
{

  if (state == true) {
    digitalWrite(fase_relay[relay_number], HIGH);
    relay_state[relay_number] = 1;
  } else if (state == false) {
    digitalWrite(fase_relay[relay_number], LOW);
    relay_state[relay_number] = 0;
  } else {
    error_nr = 3;
  }

}

/************************************************************************************/
void controle_stewardplatform(bool state)
/************************************************************************************/
{
  if (state == true) {
    digitalWrite(stewardplatform, HIGH);
    stewardplatform_state = true;
  } else if (state == false) {
    digitalWrite(stewardplatform, LOW);
    stewardplatform_state = false;
  } else {
    error_nr = 4;
  }
}


/************************************************************************************/
void controle_empty_relay(bool state)
/************************************************************************************/
{
  if (state == true) {
    digitalWrite(empty_relay, true);
    empty_relay_state = true;
  } else if (state == false) {
    digitalWrite(empty_relay, false);
    empty_relay_state = false;
  } else {
    error_nr = 5;
  }
}

/************************************************************************************/
void smoke(bool state)
/************************************************************************************/
{

}

/************************************************************************************/
void controle_light_controle(int lightnr, bool state) // 1 = STOP , 2 = RUN , 3 = PAUSE, 4 = error
/************************************************************************************/
{
  if ((lightnr == 1) && (state == true)) {
    digitalWrite(controle_light_stop, HIGH);
    digitalWrite(controle_light_run, LOW);
#if defined(DEBUG_CONTROLE_LIGHTS)
    Serial.println("Set controle licht stop high");
#endif
  } else if ((lightnr == 1) && (state == false)) {
    digitalWrite(controle_light_stop, LOW);
#if defined(DEBUG_CONTROLE_LIGHTS)
    Serial.println("Set controle licht stop low");
#endif
  } else if ((lightnr == 2) && (state == true)) {
    digitalWrite(controle_light_run, HIGH);
    digitalWrite(controle_light_stop, LOW);
    digitalWrite(controle_light_pause, LOW);
#if defined(DEBUG_CONTROLE_LIGHTS)
    Serial.println("Set controle licht run high");
#endif
  } else if ((lightnr == 2) && (state == false)) {
    digitalWrite(controle_light_run, LOW);
#if defined(DEBUG_CONTROLE_LIGHTS)
    Serial.println("Set controle licht run low");
#endif
  } else if ((lightnr == 3) && (state == true)) {
    digitalWrite(controle_light_pause, HIGH);
    digitalWrite(controle_light_run, LOW);
#if defined(DEBUG_CONTROLE_LIGHTS)
    Serial.println("Set controle licht pause high");
#endif
  } else if ((lightnr == 3) && (state == false)) {
    digitalWrite(controle_light_pause, LOW);
#if defined(DEBUG_CONTROLE_LIGHTS)
    Serial.println("Set controle licht pause low");
#endif
  }  else if ((lightnr == 4) && (state == false)) {
    digitalWrite(controle_light_pause, LOW);
    digitalWrite(controle_light_run, LOW);
  } else if ((lightnr == 4) && (state == true)) {
    digitalWrite(controle_light_pause, HIGH);
    digitalWrite(controle_light_run, HIGH);
    digitalWrite(controle_light_stop, HIGH);
#if defined(DEBUG_CONTROLE_LIGHTS)
    Serial.println("Set controle licht all high");
#endif
  }
}

/************************************************************************************/
void horn()
/************************************************************************************/
{
  if (horn_state == true) {

    if (horn_counter == horn_amount) {
      horn_state = false;
      horn_counter = 0;
      digitalWrite(hydraulic_open[1], LOW);
    } else {

      if (horn_timer_counter == horn_timer) {
        horn_timer_counter = 0;

        if (horn_on_off == true) {
          digitalWrite(hydraulic_open[1], LOW);
          horn_on_off = false;
        } else if (horn_on_off == false) {
          digitalWrite(hydraulic_open[1], HIGH);
          horn_on_off = true;
        }
        horn_counter ++;
      } else {

        horn_timer_counter ++;

      }

    }
  }
}
