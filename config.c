/**************************************************************
This file is written by Erik Gustavsson and Jonathan Gustafson.
The code is marked with a comment where Erik and Jonathans code
starts.
--------------------------------------------------------------
This file includes code from LAB3
**************************************************************/

/* mipslabfunc.c
This file written 2015 by F Lundevall
some of this original code written by Axel Isaksson
some of this file written 2021 Erik Gustavsson & Jonathan Gustafson

For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "header.h"  /* Declatations for these labs */

void config(void) {
  /*
	  This will set the peripheral bus clock to the same frequency
	  as the sysclock. That means 80 MHz, when the microcontroller
	  is running at 80 MHz. Changed 2017, as recommended by Axel.
	*/
	SYSKEY = 0xAA996655;  /* Unlock OSCCON, step 1 */
	SYSKEY = 0x556699AA;  /* Unlock OSCCON, step 2 */
	while(OSCCON & (1 << 21)); /* Wait until PBDIV ready */
	OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
	while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
	SYSKEY = 0x0;  /* Lock OSCCON */

	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;

	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;

	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);

	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	/* SPI2STAT bit SPIROV = 0; */
	SPI2STATCLR = 0x40;
	/* SPI2CON bit CKP = 1; */
        SPI2CONSET = 0x40;
	/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x20;
	/* SPI2CON bit ON = 1; */
	SPI2CONSET = 0x8000;

	display_init();

	/**************************************************************
	Here code by Erik and Jonathan starts.
	**************************************************************/

  /* set btns and switches as inputs (taken from own lab3 labwork) */
  TRISD |= 0xfe0; //Set bits 11-5 to 1 (0000 1111 1110 0000);
  PORT_EGEN &= ~0xff; // set led as output (using our own pointer)

}

/*** timmer start ***/
void timerstart(){
  // gravity timer
  T2CON = 0x0; // clr t2
  T2CONSET = 0x7<<4; // 256
  PR2 = (80000000 / 256) / 10; // ca 31k
  TMR2 = 0; //
  T2CONSET = 0x8000;// start

  IEC(0) |= 0x100; //enable timer interrupt
  IPC(2) |= 0x4;	 //set interrupt priority

  enable_interrupt(); //enable interrupts globally
}
