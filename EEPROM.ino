/************************************************************************************/
void EEPROMWritelong(int address, long value)
/************************************************************************************/
{

  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);


  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}


/************************************************************************************/
long EEPROMReadlong(long address)
/************************************************************************************/
{

  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);

  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

/************************************************************************************/
void update_eeprom()
/************************************************************************************/
{

  EEPROMWritelong(eeprom_automatic_homeing_time, automatic_homeing_time);
  EEPROMWritelong(eeprom_hydraulic_valve_1_time, hydraulic_time[1]);
  EEPROMWritelong(eeprom_hydraulic_valve_2_time, hydraulic_time[2]);
  EEPROMWritelong(eeprom_hydraulic_valve_3_time, hydraulic_time[3]);
  EEPROMWritelong(eeprom_frequency_invertor_startup_delay, frequency_invertor_startup_delay);
  EEPROMWritelong(eeprom_automatic_mode_pause_time, automatic_mode_pause_time);
  EEPROMWritelong(eeprom_automatic_mode_2020_pause_flashlight_timer_delay, automatic_mode_2020_pause_flashlight_timer_delay);
  EEPROMWritelong(eeprom_automatic_mode_2020_pause_timer_delay, automatic_mode_2020_pause_timer_delay);
  EEPROMWritelong(eeprom_automatic_mode_2020_startup_delay, automatic_mode_2020_startup_delay);
  EEPROMWritelong(eeprom_automatic_mode_2020_smoke_time, automatic_mode_2020_smoke_time);
  EEPROM.write(eeprom_automatic_mode_hydraulic_1, automatic_mode_hydraulic_used[1]);
  EEPROM.write(eeprom_automatic_mode_hydraulic_2, automatic_mode_hydraulic_used[2]);
  EEPROM.write(eeprom_automatic_mode_hydraulic_3, automatic_mode_hydraulic_used[3]);
  EEPROM.write(eeprom_automatic_mode_frequency_1, automatic_mode_frequency_used[1]);
  EEPROM.write(eeprom_automatic_mode_frequency_2, automatic_mode_frequency_used[2]);
  EEPROM.write(eeprom_automatic_mode_frequency_3, automatic_mode_frequency_used[3]);
  EEPROM.write(eeprom_automatic_mode_other_relay_1, automatic_mode_other_used[1]);
  EEPROM.write(eeprom_automatic_mode_other_relay_2, automatic_mode_other_used[2]);
  EEPROM.write(eeprom_automatic_mode_other_relay_3, automatic_mode_other_used[3]);
  EEPROM.write(eeprom_automatic_mode_other_relay_4, automatic_mode_other_used[4]);
  EEPROM.write(eeprom_automatic_mode_other_platform, automatic_mode_other_used[5]);
  EEPROM.write(eeprom_automatic_mode_other_12V, automatic_mode_other_used[6]);
  EEPROM.write(eeprom_automatic_mode_other_empty, automatic_mode_other_used[7]);
  EEPROM.write(eeprom_automatic_mode_homing_enable,automatic_mode_homeing_enable);
 
}

/************************************************************************************/
void load_eeprom()
/************************************************************************************/
{
  if (automatic_homeing_time < 100) {
#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.println("Eeprom empty set all values back to default.");
#endif

  } else {
#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.println("load eeprom:");
#endif
    automatic_homeing_time = EEPROMReadlong(eeprom_automatic_homeing_time);
#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.print("Load automatic_homeing_time:");  Serial.println(automatic_homeing_time);
#endif
    hydraulic_time[1] = EEPROMReadlong(eeprom_hydraulic_valve_1_time);
#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.print("Load hydraulic valve time 1:");  Serial.println(hydraulic_time[1]);
#endif
    hydraulic_time[2] = EEPROMReadlong(eeprom_hydraulic_valve_2_time);
#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.print("Load hydraulic valve time 2:");  Serial.println(hydraulic_time[2]);
#endif
    hydraulic_time[3] = EEPROMReadlong(eeprom_hydraulic_valve_3_time);
#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.print("Load hydraulic valve time 3:");  Serial.println(hydraulic_time[3]);
#endif
    frequency_invertor_startup_delay = EEPROMReadlong(eeprom_frequency_invertor_startup_delay);
#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.print("Load frequency_invertor_startup_delay:");  Serial.println(frequency_invertor_startup_delay);
#endif
    automatic_mode_pause_time = EEPROMReadlong(eeprom_automatic_mode_pause_time);
#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.print("Load automatic_mode_pause_time: "); Serial.println(automatic_mode_pause_time);
#endif
    automatic_mode_hydraulic_used[1] = EEPROM.read(eeprom_automatic_mode_hydraulic_1);
#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.print("Load enable hydraulic 1: "); Serial.println(automatic_mode_pause_time);
#endif
    automatic_mode_hydraulic_used[2] = EEPROM.read(eeprom_automatic_mode_hydraulic_2);
#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.print("Load enable hydraulic 2: "); Serial.println(automatic_mode_pause_time);
#endif
    automatic_mode_hydraulic_used[3] = EEPROM.read(eeprom_automatic_mode_hydraulic_3);
#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.print("Load enable hydraulic 3: "); Serial.println(automatic_mode_pause_time);
#endif
    automatic_mode_frequency_used[1] = EEPROM.read(eeprom_automatic_mode_frequency_1);
#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.print("Load enable Frequency inverter 1: "); Serial.println(automatic_mode_pause_time);
#endif
    automatic_mode_frequency_used[2] = EEPROM.read(eeprom_automatic_mode_frequency_2);
#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.print("Load enable Frequency inverter 2: "); Serial.println(automatic_mode_pause_time);
#endif
    automatic_mode_frequency_used[3] = EEPROM.read(eeprom_automatic_mode_frequency_3);
#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.print("Load enable Frequency inverter 3: "); Serial.println(automatic_mode_pause_time);
#endif
    automatic_mode_other_used[1] = EEPROM.read(eeprom_automatic_mode_other_relay_1);
#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.print("Load enable relay 1: "); Serial.println(automatic_mode_pause_time);
#endif
    automatic_mode_other_used[2] = EEPROM.read(eeprom_automatic_mode_other_relay_2);
#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.print("Load enable relay 2: "); Serial.println(automatic_mode_pause_time);
#endif
    automatic_mode_other_used[3] = EEPROM.read(eeprom_automatic_mode_other_relay_3);
#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.print("Load enable relay 3: "); Serial.println(automatic_mode_pause_time);
#endif
    automatic_mode_other_used[4] = EEPROM.read(eeprom_automatic_mode_other_relay_4);
#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.print("Load enable relay 4: "); Serial.println(automatic_mode_pause_time);
#endif
    automatic_mode_other_used[5] = EEPROM.read(eeprom_automatic_mode_other_platform);
#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.print("Load enable platform: "); Serial.println(automatic_mode_pause_time);
#endif
    automatic_mode_other_used[6] = EEPROM.read(eeprom_automatic_mode_other_12V);
#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.print("Load enable 12V system: "); Serial.println(automatic_mode_pause_time);
#endif
    automatic_mode_other_used[7] = EEPROM.read(eeprom_automatic_mode_other_empty);
#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.print("Load enable empty "); Serial.println(automatic_mode_pause_time);
#endif
automatic_mode_homeing_enable = EEPROM.read(eeprom_automatic_mode_homing_enable);
#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.print("Load homeing enabled"); Serial.println(automatic_mode_homeing_enable);
#endif
automatic_mode_2020_pause_flashlight_timer_delay = EEPROMReadlong(eeprom_automatic_mode_2020_pause_flashlight_timer_delay);
#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.print("Load flashlight time"); Serial.println(automatic_mode_2020_pause_flashlight_timer_delay);
#endif
automatic_mode_2020_pause_timer_delay = EEPROMReadlong(eeprom_automatic_mode_2020_pause_timer_delay);
#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.print("Load automatic mode pause delay"); Serial.println(automatic_mode_2020_pause_timer_delay);
#endif
automatic_mode_2020_startup_delay = EEPROMReadlong(eeprom_automatic_mode_2020_startup_delay);
#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.print("Load automatic mode startup delay"); Serial.println(automatic_mode_2020_startup_delay);
#endif

automatic_mode_2020_smoke_time = EEPROMReadlong(eeprom_automatic_mode_2020_smoke_time);
#if defined(DEBUG_DETERMINE_ARRAYS)
    Serial.print("Load automatic mode smoke time"); Serial.println(automatic_mode_2020_smoke_time);
#endif

  }
}
