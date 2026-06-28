#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
int timer_counter = 2;
int estado = 0;
void blink(int time, int repeat);
ISR(WDT_vect) {
// Toggle Port B pin 3 output state
 PORTB |= 1<<PB4;
 delay(10);
 PORTB ^= 1<<PB4;
 PORTB |= 1<<PB1;
 delay(10);
 int a=0;
 a = analogRead(PB3);
 if (a>500) {
  blink(20, 3);
 }
 else if (a<250) {
  blink(30, 1);
 }
 if (timer_counter == 0) {
  if (estado) {
    PORTB |= 1<<PB2;
    timer_counter = 1;
  } else {
    PORTB &= (0<<PB2);
    timer_counter = 2;
  }
  estado = !estado;
 } else {
  timer_counter--;
 }
 delay(10);
 PORTB ^= 1<<PB1;
 delay(10);
}

void setup(){
// Set up Port B as Input
DDRB = 0; // usually not necessary but it will save current
// Set Port B pin 3, pin 7 and pin 6 mode back to output
DDRB = 1<<DDB4;
DDRB |= 1<<DDB2;
DDRB |= 1<<DDB1;
DDRB |= 1<<DDB0; //solo para led de debug.

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

void blink(int time, int repeat) {
  for(int i = repeat; i>0; i--) {
    PORTB |= 1<<PB0;
    delay(time);
    PORTB ^= 1<<PB0;
    delay(time);
  }
}