#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#define APAGADO 0
#define ENCENDIDO 1
#define DEBUG

int timer_counter = 4;
int estado = 0;
int hora = 0;

void blink(int time, int repeat);

ISR(WDT_vect) {
  // Encender y apagar led de inicio de ciclo
  PORTB |= 1<<PB4;
  delay(5);
  PORTB ^= 1<<PB4;

  if (timer_counter == 0) {
    // Leer humedad
    PORTB |= 1<<PB1;
    delay(5);
    int a=0;
    int humedad = analogRead(PB3);
    PORTB ^= 1<<PB1;
#ifdef DEBUG
    if (a>500) {
      blink(20, 3);
    }
    else if (a<250) {
      blink(30, 1);
    }
#endif

  if (estado == ENCENDIDO) {
    if (hora > 2) {
      PORTB &= (0<<PB2);
      hora = 0;
      estado = APAGADO;
    }
  } else {
    if (humedad >500) { // seco es mayor que 500
      if (hora >2) {
        PORTB |= (1<<PB2);
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