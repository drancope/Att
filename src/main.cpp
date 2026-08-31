#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#define APAGADO 0
#define ENCENDIDO 1
//#define DEBUG

#define LED PB4 //pin 3
#define SENSOR_PWR PB1 //pin 6
#define SENSOR PB3 //pin 2
#define MOTOR PB2 //pin 7

int estado = 1;

void blink(int time, int repeat);

ISR(WDT_vect) {

  // Encender o apagar las tres salidas (led, 7 y 6)
if (estado) {
  PORTB |= (1<<SENSOR_PWR) | (1<<LED) | (1<<MOTOR);
} else {
  PORTB = 0;
}
 estado = !estado;
}

void setup(){
// Set up Port B as Input
DDRB = 0; // usually not necessary but it will save current
// Poner los pines 3, 7 y 6 de vuelta a modo salida. 
DDRB = 1<<LED;
DDRB |= 1<<MOTOR;
DDRB |= 1<<SENSOR_PWR; // 

PORTB |= (1<<SENSOR_PWR);
//set timer to 1 sec
// WDTCR |= (0<<WDP3) | (1<<WDP2) | (1<<WDP1) | (0<<WDP0);
// set timer to 0.5s
// WDTCR |= (1<<WDP2) | (1<<WDP0);
// set timer to 4 sec
WDTCR |= (1<<WDP3);
// set timer to 8 sec
// WDTCR |= (1<<WDP3) | (1<<WDP0);


// Set watchdog timer in interrupt mode
WDTCR |= (1<<WDTIE);
WDTCR |= (0<<WDE);
sei(); // Enable global interrupts

set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

void loop() {
 sleep_mode();   // go to sleep and wait for interrupt...
}
