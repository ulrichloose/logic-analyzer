/*
   LA.cpp

   Created: 11/12/2016 19.35.51
   Author : Vincenzo
*/

#define baudrate 115200 //check if it is the same in processing
#define samples 400

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint16_t timer1_overflow_count;
uint8_t pinChanged[samples], ovl[samples], initial, state, old_state;
uint16_t timer[samples], event = 0;

void init_board()
{
  PORTC = (0 << 0); DDRC |= (1 << 0); // led A0
  DDRB |= 0x00;     // pin 8-13 input
  PORTB |= 0x3F;    // pull-up
}

void init_timer()
{
  cli();
  
  //clear
  TCCR1A = 0b00000000;
  TCCR1B = 0b00000000;
  TIMSK1 = 0b00000000;

  //settings
  TCCR1A |= (0 << COM1A1) | (0 << COM1A0) | (0 << COM1B1) | (0 << COM1B0); //normal port operation
  TCCR1A |= (0 << WGM11) | (0 << WGM10); //normal operation
  TCCR1B |= (0 << WGM13) | (0 << WGM12); //normal operation
  TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10); //clock prescaler

  sei();
  
  TIMSK1 |= (1 << TOIE1);   // enable overflow interrupt
}

ISR(TIMER1_OVF_vect)
{
  timer1_overflow_count++;
}

uint16_t myMicros ()
{
  cli();

  if (TIFR1 & (1 << TOV1)) {
    TIFR1 = (0 << TOV1);
    timer1_overflow_count++;
  }

  sei();
  
  return TCNT1;
}

void start()
{
  _delay_ms(500);
  PORTC = (1 << 0);             // turn on led
  TCNT1 = 0;                    // reset TCN1
  timer1_overflow_count = 0;    // reset ovl counter
  event = 0;                    // reset event counter
  initial = PINB;
  state = initial;              
}

void sendData()
{
  PORTC = (0 << 0);   //turn off led

  //initial data
  Serial.println("S");
  Serial.print(initial); Serial.print(":");
  Serial.println(samples);

  //data
  for (int i = 0; i < samples; i++)
  {
    Serial.print(pinChanged[i]); Serial.print(":");
    Serial.println((65536 * ovl[i] + timer[i]) / 2);
  }
}

int main(void)
{
  Serial.begin(baudrate);

  init_board();
  init_timer();

  start();

  while (1)
  {

    old_state = state;
    state = PINB;

    if (old_state != state)
    {
      timer[event] = myMicros();
      ovl[event] = timer1_overflow_count;
      pinChanged[event] = state ^ old_state;
      event++;

      if (event == samples)
      {
        sendData();
        while (Serial.read() != 'G') ;  //wait for the "go"
        start();
      }
    }
  }
}
