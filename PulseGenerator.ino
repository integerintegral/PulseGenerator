#include "configuration.h"

bool input_activation, input_flag, pulse_state;
#if (DEBOUNCE == 1)
unsigned long input_timer;
#endif
unsigned long pulse_timer;
byte counter;

void setup()
{
  pinMode(INPUT_PIN, INPUT);
  pinMode(OUTPUT_PIN, OUTPUT);
}

void loop()
{
  button_tick();
  change_pin_state();
}

void button_tick()
{
#if (INPUT_TYPE == 1)
  bool input_state = !digitalRead(INPUT_PIN); 
#elif (INPUT_TYPE == 0)
  bool input_state = digitalRead(INPUT_PIN);
#endif

#if (DEBOUNCE == 1)
  if (input_state && !input_flag && (millis() - input_timer > INPUT_DELAY))
  {
    input_flag = true;    
    input_timer = millis();
  }
#elif (DEBOUNCE == 0)
  input_flag = true;
#endif

#if (WAITING_FOR_THE_END == 0)
  if (!input_state && input_flag)
  {
    input_flag = false;
#if (DEBOUNCE == 1)
    input_timer = millis();
#endif
    input_activation = true;
    pulse_timer = millis();
    pulse_state = 0;
    counter = 1;
  }
#elif (WAITING_FOR_THE_END == 1)
  if (!input_state && input_flag && !input_activation)
  {
    input_flag = false;
#if (DEBOUNCE == 1)
    input_timer = millis();
#endif
    input_activation = true;
    pulse_timer = millis();
    pulse_state = 0;
    counter = 1;
  }
#endif
}

void change_pin_state()
{
  if (input_activation)
  {
    if (counter > PULSES_AMOUNT)
    {
      input_activation = 0;
      counter = 1;
    }
    else
    {
      if (pulse_state == 0 && millis() - pulse_timer > OUTPUT_ACTIVE_DELAY)
      {
        pulse_state = 1;
        pulse_timer = millis();
      }
      else if (pulse_state == 1 && millis() - pulse_timer > OUTPUT_PAUSE_DELAY)
      {
        pulse_state = 0;
        pulse_timer = millis();
        counter++;
      }
    }
  }
  digitalWrite(OUTPUT_PIN, pulse_state);
}
