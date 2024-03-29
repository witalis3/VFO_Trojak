/*
 * Yack_atmega32.c
 *
 * Created: 2016-10-16 14:34:13
 * Author : witek
 * port keyera Yack dla procesora atmega32A
 */ 

#define F_CPU 16000000UL
#ifndef F_CPU
#error F_CPU undefined!! Please define in Makefile
#endif

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "yack.h"

// Time after which callsign training is assumed complete
#define     TRAINTIMEOUT    10      // 10 Seconds
#define     PITCHREPEAT     10      // 10 e's will be played for pitch adjust
#define     FARNSREPEAT     10      // 10 a's will be played for Farnsworth

// Some texts in Flash used by the application
const char  txok[] PROGMEM      = "OK";
const char  vers[] PROGMEM      = "V0.7";

/*
void pitch(void)
{
	word    timer=PITCHREPEAT;
	
	while (timer)           // while not yet timed out
	{
		timer--;
		yackchar('E');      // play an 'e'
		
		if(!(KEYINP & (1<<DITPIN))) // if DIT was keyed
		{
			yackpitch(DOWN);        // increase the pitch
			timer=PITCHREPEAT;
		}
		
		if(!(KEYINP & (1<<DAHPIN))) // if DAH was keyed
		{
			yackpitch(UP);  // lower the pitch
			timer=PITCHREPEAT;
		}
		
	}
	
}
*/


void setfarns(void)
{
	byte    timer=0;
	
	while (timer++ != FARNSREPEAT)          // while not yet timed out
	{
		
		if(!(KEYINP & (1<<DITPIN))) // if DIT was keyed
		{
			yackspeed(DOWN,FARNSWORTH);     // increase the pitch
			timer=0;
		}
		
		else if(!(KEYINP & (1<<DAHPIN))) // if DAH was keyed
		{
			yackspeed(UP,FARNSWORTH);   // lower the pitch
			timer=0;
		}
		
		else
		yackdelay(IWGLEN);
		
	}
	
}




word lfsr(byte n)
{
	
	static word     lfsr = 0xACE1;
	byte            random;
	
	lfsr = (lfsr >> 1) ^ (-(lfsr & 1u) & 0xB400u);
	
	random = lfsr >> 8; // Byte = upper byte of word
	
	while (random >= n) random -= n; // Cheap modulo :-)
	
	return random;
	
}





void rndcall(char* call)
{
	byte    i;
	
	// Generate a random callsign 2 Char, 1 Digit, 2 Char
	
	for (i=0;i<5;i++)
	{
		if (i == 2)
		call[i]=lfsr(10) + '0';
		else
		call[i]=lfsr(26) + 'A';
		
	}
}




void cstrain(void)
{
	char    call[5];    // A buffer to store the callsign
	char    c;          // The character returned by IAMBIC keyer
	byte    i;          // Counter
	byte    n;          // Playback counter
	word    timer;      // Timeout timer
	
	while(1)    // Endless loop will exit throught RETURN statement only
	
	{
		rndcall(call); // Make up a callsign
		
		i=0; // i counts the number of chracters correctly guessed
		
		while(i<5)
		{
			if (!i) // If nothing guessed yet, play the callsign
			{
				yackdelay(2 * IWGLEN); // Give him some time to breathe b4 next callsign
				for (n=0;n<5;n++)
				{
					yackchar(call[n]);
					yackfarns(); // Add potential farnsworth delays
					if(yackctrlkey(TRUE))
					return; // Abort if requested..
				}
			}
			
			timer = YACKSECS(TRAINTIMEOUT);
			
			do
			{
				
				c=yackiambic(OFF);  // Wait for a character
				yackbeat();         // FSM heartbeat
				timer--;            // Countdown
				
			} while ((!c) && timer && !(yackctrlkey(FALSE))); // Stop when character or timeout
			
			if (timer == 0 || yackctrlkey(TRUE))    // If termination because of timeout
			return;             // then return
			
			if (call[i] == c)       // Was it the right character?
			i++;                // then increment counter
			else
			{
				yackerror();        // Send an error prosign
				i=0;                // And reset the counter
			}
			
		}
		
		yackchar ('R');
		
	}
}

void commandmode(void)
{
	
	char    c;              // Character from Morse key
	word    timer;          // Exit timer
	
	yackinhibit(ON);        // Sidetone = on, Keyer = off

	tft.setTextSize(2);
	tft.setTextColor(ILI9341_MAGENTA, ILI9341_BLACK);
	tft.setCursor(278, 215);
	tft.print("CMD");
	
	yackchar('?');          // Play Greeting

	timer = YACKSECS(DEFTIMEOUT); // Time out after 10 seconds
	
	while ((yackctrlkey(TRUE)==0) && (timer-- > 0))
	{
		
		c=yackiambic(OFF);
		if (c) timer = YACKSECS(DEFTIMEOUT); // Reset timeout if character read
		
		yackbeat();
		
		lfsr(255);          // Keep seeding the LFSR so we get different callsigns
		
		if (!yackflag(CONFLOCK)) // No Configuration lock?
		{
			switch (c) // These are the lockable configuration commands
			{
				
				case    'R': // Reset
				yackreset();
				c = TRUE;
				break;
				
				case    'A': // IAMBIC A
				yackmode(IAMBICA);
				c = TRUE;
				break;
				
				case    'B': // IAMBIC B
				yackmode(IAMBICB);
				c = TRUE;
				break;

				case    'L': // ULTIMATIC
				yackmode(ULTIMATIC);
				c = TRUE;
				break;
				
				case    'D': // DAHPRIO
				yackmode(DAHPRIO);
				c = TRUE;
				break;
				
				case    'X': // Paddle swapping
				yacktoggle(PDLSWAP);
				c = TRUE;
				break;
				
				case    'S': // Sidetone toggle
				yacktoggle(SIDETONE);
				c = TRUE;
				break;
				
				case    'K': // TX keying toggle
				yacktoggle(TXKEY);
				c = TRUE;
				break;
				
				case    'Z': // Farnsworth pause
				setfarns();
				c = TRUE;
				break;
				
				case    'I': // TX level inverter toggle
				yacktoggle(TXINV);
				c = TRUE;
				break;
				
				case    '1': // Record Macro 1
				yackchar('1');
				yackmessage(RECORD, 1);
				c = TRUE;
				break;
				
				case    '2': // Record Macro 2
				yackchar('2');
				yackmessage(RECORD, 2);
				c = TRUE;
				break;

				case    '3': // Record Macro 2
				yackchar('3');
				yackmessage(RECORD, 3);
				c = TRUE;
				break;
			}
			
		}
		
		switch (c) // Commands that can be used anytime
		{
			
			case    'V': // Version
			yackstring(vers);
			c = TRUE;
			break;
			
			/*
			case    'P': // Pitch
			pitch();
			c = TRUE;
			break;
			*/
			case    'U': // Tune
			yackinhibit(OFF);
			yacktune();
			yackinhibit(ON);
			c = TRUE;
			break;
			
			
			case    'C': // Callsign training
			cstrain();
			c = TRUE;
			break;
			
			
			case    '0': // Lock changes
			yacktoggle(CONFLOCK);
			c = TRUE;
			break;
			
			case    'E': // Playback Macro 1
			yackinhibit(OFF);
			yackmessage(PLAY,1);
			yackinhibit(ON);
			timer = YACKSECS(MACTIMEOUT);
			c = FALSE;
			break;
			
			case    'T': // Playback Macro 2
			yackinhibit(OFF);
			yackmessage(PLAY,2);
			yackinhibit(ON);
			timer = YACKSECS(MACTIMEOUT);
			c = FALSE;
			break;

			
			case    'W': // Query WPM
			yacknumber(yackwpm());
			c = TRUE;
			break;
			
			
		}
		
		if (c == TRUE) // If c still contains a string, the command was not handled properly
		yackstring(txok);
		else if (c)
		yackerror();
		
		
	}

	yackchar('E');          // Sign off

	tft.setTextSize(2);
	tft.setTextColor(ILI9341_DARKGREY, ILI9341_BLACK);
	tft.setCursor(278, 215);
	tft.print("CMD");

	yackinhibit(OFF);       // Back to normal mode
	
}



void yack_loop(void)

{
	if (yackctrlkey(TRUE)) // If command key pressed, go to command mode
	{
		commandmode();
	}
	yackbeat();
	yackiambic(OFF);
}
