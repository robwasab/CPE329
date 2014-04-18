#include <msp430.h> 
#include "lcd.h"
#include "delay.h"
#include "print.h"

//unsigned 8 bit
#define u8 unsigned char
//unsigned 16 bit
#define u16 unsigned short
#define BUTTON BIT3

int main(void) {

    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    //16Mhz

     if (CALBC1_16MHZ==0xFF)					// If calibration constant erased
     {
       while(1);                               // do not load, trap CPU!!
     }
     DCOCTL = 0;                               // Select lowest DCOx and MODx settings
     BCSCTL1 = CALBC1_16MHZ;                   // Set range
     DCOCTL = CALDCO_16MHZ;                    // Set DCO step + modulation*/

     //red led on launchpad
     P1DIR = 0x01;

     //configure P1.3 to switch on falling edge interrupts
     //force the pin to use the typical GPIO
     P1SEL  &= ~BUTTON;
     P1SEL2 &= ~BUTTON;

     //select HIGH to LOW transition
     P1IES |= BUTTON;
     P1IE  |= BUTTON;
     P1IFG &= ~BUTTON;
     P1DIR &= ~BUTTON;
     P1REN |= BUTTON;

     lcd_init();

     //why not, just go back to home clear everything again
     display_reset();

     lcd_print("Push the button!");

     _enable_interrupts();

     _bis_SR_register(GIE | LPM4);

     while(1)
     {
    	 P1OUT ^= 0x01;
    	 delay_ms(1000);
     }

     return 0;
}

#pragma vector=PORT1_VECTOR
__interrupt void button_isr(void)
{
  static u8 counter = 0;
  static char str[] = "hello world! ";

  delay_us(500);
  if (!(P1IN & BUTTON))
  {
	  return;
  }

  ++counter;

  display_reset();

  lcd_print(str);

  //go to the second line
  const u8 row = 1;
  const u8 col = 0;
  cursor_cordinate(row, col);

  PRINT_INT(counter);

  P1IFG &= ~BUTTON;                           // P1.4 IFG cleared
}
