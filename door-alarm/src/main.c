#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

volatile int counter = 0;



// https://exploreembedded.com/wiki/AVR_Timer_Interrupts

// http://www.atmel.com/webdoc/avrlibcreferencemanual/FAQ_1faq_use_bv.html

// https://arduino.stackexchange.com/questions/3929/attiny85-interrupt-id-vs-pin-when-programming-with-arduino

// http://www.elecrom.com/avr-tutorial-2-avr-input-output/

// http://www.atmel.com/webdoc/avrlibcreferencemanual/group__avr__sleep.html




/* uint8_t tones[] = { 0x20, 0x40, 0x60, 0x00, 0x10, 0x28 }; */
/* int delays[]    = { 500, 500, 500, 1000, 250, 250 }; */
uint8_t tones[] = { 0x20, 0x40, 0x60  };
int delays[]    = { 50, 50, 50 };

// TIFR



ISR(PCINT0_vect)
{
   counter++;

   if (counter % 2)
   {
      PORTB &= ~_BV(PORTB1);
   }
   else
   {
      PORTB |= _BV(PORTB1);
   }
}


ISR(TIM0_OVF_vect)
{
   PORTB ^= _BV(PB1);
   TCNT0=0x80;
}


void
setup()
{

   // setup pin change interrupt for door switch in pin 2

   PCMSK |= _BV(PCINT0); // pin change mask
   GIMSK |= _BV(PCIE);  // pin change interrupt enable
   sei();

   // http://www.technoblogy.com/show?LE0
   //TCCR0A = _PV(COM0A0) | 3<<COM0B0 | 3<<WGM00;


   DDRB |= _BV(PB1);  // PB1 -> output



}


void
pwm_init()
{
   TCCR1 = _BV(PWM1A) | _BV(COM1A0) | _BV(CS10) | _BV(CS12);
}

void
timer_init()
{
   TCCR0A = 0;
   TCCR0B = _BV(CS00);
   TIMSK |= _BV(TOIE0);
   sei();
}


void
my_delay_ms(unsigned int delay)
{
   for (; delay; delay--)
   {
      _delay_ms(1);
   }
}

int
main(void)
{
   DDRB |= _BV(PB1);  // PB1 -> output
   //   setup();

   pwm_init();
   OCR1A = 0x80;

   while (1)
   {
      for (int i=0; i < sizeof(tones); i++)
      {
         TCNT1 = 0x01;
         OCR1C = 0x80 + tones[i];
         my_delay_ms(delays[i]);
         PORTB |= _BV(PB1);
         my_delay_ms(5);
      }

      /* while ((TIFR & _BV(TOV0)) == 0); */
      /* TCNT0=0x00; */
      /* TIFR |= _BV(TOV0); //clear timer1 overflow flag */


      /* PORTB ^= _BV(PB1); */
   //      sleep_mode();
   }

   return 0;
}
