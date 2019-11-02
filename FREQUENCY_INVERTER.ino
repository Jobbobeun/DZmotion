void set_frequency_inverter(int nr, bool state, bool Direction)
{

  if ( state != frequency_invertor_state[nr]) {

    if (state == true) {

      digitalWrite(frequency_invertor_on_off[nr], HIGH);
      frequency_invertor_state[nr] = true;

    }

    else if (state == false) {

      digitalWrite(frequency_invertor_on_off[nr], LOW);
      frequency_invertor_state[nr] = false;

    }
  }

  if (Direction != frequency_invertor_state_direction[nr]) {

    if (Direction == true) {

      digitalWrite(frequency_invertor_direction[nr], HIGH);
      frequency_invertor_state_direction[nr] = true;

    }

    else  if (Direction == false) {

      digitalWrite(frequency_invertor_direction[nr], LOW);
      frequency_invertor_state_direction[nr] = false;

    }
  }
}
