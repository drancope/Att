#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#define APAGADO 0
#define ENCENDIDO 1
//2#define DEBUG

#define LED PB4
#define SENSOR_PWR PB1
#define SENSOR PB3
#define MOTOR PB2

int timer_counter = 4;
int estado = 0;
int hora = 0;

void blink(int time, int repeat);

ISR(WDT_vect) {
  // Encender y apagar led de inicio de ciclo
PORTB |= 1<<SENSOR_PWR;
  //PORTB &= ~(1<<SENSOR_PWR);
#ifndef DEBUG
  PORTB |= 1<<LED;
  delay(5);
  PORTB ^= 1<<LED;
#endif

  if (timer_counter == 0) {
    // Leer humedad
    PORTB |= 1<<SENSOR_PWR;  // Encender alimentación del sensor
    delay(5);
    int16_t humedad = analogRead((analog_pin_t)SENSOR);
//    PORTB &= ~(1<<SENSOR_PWR);
#ifdef DEBUG
    if (humedad>500) {
      blink(20, 3);
    }
    else if (humedad<250) {
      blink(30, 1);
    }
#endif

  if (estado == ENCENDIDO) {
    if (hora > 2) {
      PORTB &= (0<<MOTOR);
      hora = 0;
      estado = APAGADO;
    }
  } else {
    if (humedad >500) { // seco es mayor que 500
      if (hora >2) {
        PORTB |= (1<<MOTOR);
        estado = ENCENDIDO;
        hora = 0;
      }
    }
    hora++;
  }
 } else {
  timer_counter--;
 }
}

void setup(){
// Set up Port B as Input
DDRB = 0; // usually not necessary but it will save current
// Poner los pines 3, 7 y 6 de vuelta a modo salida. 
DDRB = 1<<LED;
DDRB |= 1<<MOTOR;
DDRB |= 1<<SENSOR_PWR; // 

PORTB &= ~(1<<SENSOR_PWR);
//set timer to 1 sec
//WDTCR |= (0<<WDP3) | (1<<WDP2) | (1<<WDP1) | (0<<WDP0);
// set timer to 0.5s
// WDTCR |= (1<<WDP2) | (1<<WDP0);
// set timer to 4 sec
// WDTCR |= (1<<WDP3);
// set timer to 8 sec
WDTCR |= (1<<WDP3) | (1<<WDP0);


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
    PORTB |= 1<<LED;
    delay(time);
    PORTB ^= 1<<LED;
    delay(time);
  }
}