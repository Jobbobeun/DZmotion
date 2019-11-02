void set_hydraulic_valve(int nr, bool state) {

  if (hydraulic_state[nr] != state) {

    if (state == true) {

      digitalWrite(hydraulic_open[nr], HIGH);
      digitalWrite(hydraulic_close[nr], LOW);
      hydraulic_state[nr] = true;

    }
    else if (state == false) {

      digitalWrite(hydraulic_open[nr], LOW);
      digitalWrite(hydraulic_close[nr], HIGH);
      hydraulic_state[nr] = false;

    }
  }
}

void check_hydraulic_time(int nr, int max_time){
  
}
