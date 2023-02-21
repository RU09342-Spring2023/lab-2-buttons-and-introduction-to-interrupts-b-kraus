/*
 * OccupancyDetector.c
 *
 *  Created on: Feb. 12, 2023
 *      Author: Beth Kraus
 */

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#define ARMED 0
#define WARNING 1
#define ALARM 2
char ALARM_SYSTEM;
void gpioInit();

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    gpioInit();                 // Initialize all GPIO Pins for the project


    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode// to activate previously configured port settings
  int i =0;
    while(1)
    {
        switch(ALARM_SYSTEM) {
        case ARMED: {
          if (!(P2IN & BIT3))
              ALARM_SYSTEM = WARNING;
        else
            { P1OUT &= ~BIT0;
            P6OUT ^= BIT6;
         __delay_cycles(3000000);}
          break;
      }
      case WARNING: {
          P1OUT ^= BIT0;
          P6OUT &= ~BIT6;

          if (i>=20)
             {ALARM_SYSTEM = ALARM;
             i = 0;}
          else if (P2IN & BIT3)
          {ALARM_SYSTEM = ARMED;}
          else
              {i++;
          __delay_cycles(500000);}

          break;
    }
      case ALARM: {
          if ((P4IN & BIT1))//BUTTON PRESSED
          { P1OUT |= BIT0;}
          else if (!(P4IN & BIT1))//NOT PRESSED
            { ALARM_SYSTEM = ARMED; }

       break;

      }


        }
    }
}

void gpioInit()
     { P1DIR |= BIT0;             // Configure P1.0 to an Output
       P6DIR |= BIT6;              // Configure P6.6 to an Output
       P2DIR &= ~BIT3;             // Configure P2.3 to an Input
       P4DIR &= ~BIT1;             // Configure P4.1 to an Input

       P2REN |= BIT3;               // Enable Resistor on P2.3
       P2OUT |= BIT3;               // Configure Resistor on P2.3 to be Pullup
       P4REN |= BIT1;               // Enable Resistor on P4.1
       P4OUT |= BIT1;               // Configure Resistor on P4.1 to be Pullup
    }
