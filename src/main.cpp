#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
int timer_counter = 2;
int estado = 0;
ISR(WDT_vect) {
// Toggle Port B pin 3 output state
 PORTB = 1<<PB3;
 delay(1);
 PORTB ^= 1<<PB3;
 if (timer_counter == 0) {
  if (estado) {
    PORTB |= 1<<PB2;
    timer_counter = 1;
  } else {
    PORTB ^= (1<<PB2);
    timer_counter = 2;
  }
  estado = !estado;
 } else {
  timer_counter--;
 }
}

void setup(){
// Set up Port B as Input
DDRB = 0; // usually not necessary but it will save current
// Set Port B pin 3 mode back to output
DDRB = 1<<DDB3;
DDRB |= 1<<DDB2;

//set timer to 1 sec
//WDTCR |= (0<<WDP3) | (1<<WDP2) | (1<<WDP1) | (0<<WDP0);
// set timer to 0.5s
// WDTCR |= (1<<WDP2) | (1<<WDP0);
// set timer to 4 sec
 WDTCR |= (1<<WDP3);

// Set watchdog timer in interrupt mode
WDTCR |= (1<<WDTIE);
WDTCR |= (0<<WDE);
sei(); // Enable global interrupts

set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

void loop() {
 sleep_mode();   // go to sleep and wait for interrupt...
}

