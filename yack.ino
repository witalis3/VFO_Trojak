#include <avr/io.h> 
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdint.h>
#include "yack.h"

// Forward declaration of private functions
static      void key( byte mode); 
static      char morsechar(byte buffer);
static      void keylatch(void);

// Enumerations

enum FSMSTATE { 
                IDLE,   
                KEYED,  
                IEG     
              };   

// Module local definitions

static      byte    yackflags;      // Permanent (stored) status of module flags
//static      byte    volflags=0;     // Temporary working flags (volatile)
static		byte	volbfr;		// ...
//static      word    ctcvalue;       // Pitch
//static      word    wpmcnt;         // Speed
//static      byte    wpm;            // Real wpm
static      byte    farnsworth;     // Additional Farnsworth pause

// EEPROM Data

byte        magic EEMEM = MAGPAT;   // Needs to contain 'A5' if mem is valid
byte        flagstor EEMEM = ( IAMBICB | TXKEY | SIDETONE); //  Defaults
//word        ctcstor EEMEM = DEFCTC; // Pitch = 800Hz
byte        wpmstor EEMEM = DEFWPM; // 20 WPM
byte        fwstor  EEMEM = 0; // No farnsworth pause
//word		user1 EEMEM = 0; // User storage
//word        user2 EEMEM = 0; // User storage
char        eebuffer1[RBSIZE] EEMEM = "sp3jdz";
char        eebuffer2[RBSIZE] EEMEM = "r 5nn tu";
char        eebuffer3[RBSIZE] EEMEM = "cq cq de sp3jdz sp3jdz pse k";

// Flash data



const byte morse[] PROGMEM = 
{
    
    0b11111100, // 0
    0b01111100, // 1
    0b00111100, // 2
    0b00011100, // 3
    0b00001100, // 4
    0b00000100, // 5
    0b10000100, // 6
    0b11000100, // 7
    0b11100100, // 8
    0b11110100, // 9
    0b01100000, // A
    0b10001000, // B
    0b10101000, // C
    0b10010000, // D
    0b01000000, // E
    0b00101000, // F
    0b11010000, // G
    0b00001000, // H
    0b00100000, // I                                
    0b01111000, // J
    0b10110000, // K
    0b01001000, // L
    0b11100000, // M
    0b10100000, // N
    0b11110000, // O
    0b01101000, // P
    0b11011000, // Q
    0b01010000, // R
    0b00010000, // S
    0b11000000, // T
    0b00110000, // U
    0b00011000, // V
    0b01110000, // W
    0b10011000, // X
    0b10111000, // Y
    0b11001000, // Z
    0b00110010, // ?
    0b10001100, // =
    0b01010110, // .
    0b00010110, // SK
    0b01010100, // AR
    0b10010100  // /
    
};


// The special characters at the end of the above table can not be decoded
// without a small table to define their content. # stands for SK, $ for AR

// To add new characters, add them in the code table above at the end and below
// Do not forget to increase the legth of the array..

const char spechar[6] PROGMEM = "?=.#$/";



// Functions

// ***************************************************************************
// Control functions
// ***************************************************************************

void yackreset (void)
{

    //ctcvalue=DEFCTC; // Initialize to 800 Hz
    wpm = DEFWPM; // Init to default speed
    wpmcnt = (1200/YACKBEAT)/DEFWPM; // default speed
    farnsworth = 0; // No Farnsworth gap
    yackflags = FLAGDEFAULT;  

    volflags |= DIRTYFLAG;
    yacksave(); // Store them in EEPROM

}


void yackinit (void)
{
    
    byte magval;
    /*
    // Configure DDR. Make OUT and ST output ports
    SETBIT (OUTDDR,OUTPIN);    
    SETBIT (STDDR,STPIN);
    
    // Raise internal pullups for all inputs
    SETBIT (KEYPORT, DITPIN);  
    SETBIT (KEYPORT, DAHPIN);
    SETBIT (BTNPORT, BTNPIN);
    */

    magval = eeprom_read_byte(&magic); // Retrieve magic value
#if defined(DEBEK)
    tft.print("magval: ");
    tft.println(magval);
#endif
    
    if (magval == MAGPAT) // Is memory valid
    {
        //ctcvalue = eeprom_read_word(&ctcstor); // Retrieve last ctc setting
        wpm = eeprom_read_byte(&wpmstor); // Retrieve last wpm setting
        last_wpm = wpm;
#if defined(DEBEK)
        tft.print("wpm: ");
        tft.println(wpm);
#endif
        yackflags = eeprom_read_byte(&flagstor); // Retrieve last flags
        wpmcnt=(1200/YACKBEAT)/wpm; // Calculate speed
        farnsworth = eeprom_read_byte(&fwstor); // Retrieve last wpm setting
    }
    else
    {
        yackreset();
    }   
    yackinhibit(OFF);
    
    // Initialize timer1 to serve as the system heartbeat
    // CK runs at 1MHz. Prescaling by 64 makes that 15625 Hz.
    // Counting 78 cycles of that generates an overflow every 5ms
    /*
    OCR1C = 78; // 77 counts per cycle
    TCCR1 |= (1<<CTC1) | 0b00000111 ; // Clear Timer on match, prescale ck by 64
    OCR1A = 1; // CTC mode does not create an overflow so we use OCR1A
	*/
	
	// wersja dla Atmega32A timer0 CK = 16MHz prescaling 1024 (64*16) - konflikt
	/*
	OCR0 = 78; // 78 counts per cycle
	TCCR0 |= (1<<WGM01) | (1<<CS02) | (1<<CS00) | (1<<COM00); // CTC mode; prescale ck by 1024
	TCNT0 = 0;
	
	TCCR1B |= (1<<CS12) | (1<<WGM12) ;		// prescaler set to 256 and CTC for OC1A, OC1B
	*/
	// wersja dla Atmega32A timer1 CK = 16MHz prescaling 1024 (64*16) 
	//OCR1B = 312; // 78*4 counts per cycle - zegar 16MHz zamiast 1MHz - tu 4 razy więcej i prescaler 4 razy więcej
	// temp OC1B output
	//TCCR1A |= (1<<COM1B0);
	
	//TCCR1B |= (1<<CS12) | (1<<WGM12); // CTC mode; prescale ck by 256
    //TCNT1 = 0;
	//--
	//SETBIT(DDRD, 4);	// PD4 output
	//SETBIT(DDRB, 3);	// PB3 OC0 output
	//CLEARBIT(PORTD, 4);
				//OCR1B = 39;
				//TCCR1A |= (1<<COM1B0);	// toggle OC1B - nie działa
				//TCCR1B |= (1<<CS12) | (1<<WGM12) ;		// prescaler set to 256; CTC dla OC1A, OC1B

}

void yacksave (void)
{

    if(volflags & DIRTYFLAG) // Dirty flag set?
    {   

        eeprom_write_byte(&magic, MAGPAT);
        //eeprom_write_word(&ctcstor, ctcvalue);
        eeprom_write_byte(&wpmstor, wpm);
#if defined(DEBEK)
        tft.print("zapisano wpm = ");
        tft.println(wpm);
#endif
        eeprom_write_byte(&flagstor, yackflags);
        eeprom_write_byte(&fwstor, farnsworth);

        volflags &= ~DIRTYFLAG; // Clear the dirty flag
    }
}



void yackinhibit (byte mode)
{
    
    if (mode)
    {
        volflags &= ~(TXKEY | SIDETONE);
        volflags |= SIDETONE;
    }
    
    else
        
    {
        volflags &= ~(TXKEY | SIDETONE);
        volflags |= (yackflags & (TXKEY | SIDETONE));
        key(UP);
        //clk_disable(2);
    }
    
}


/*
word yackuser (byte func, byte nr, word content)
{
    
    
    if (func == READ)
    {
        if (nr == 1) 
            return (eeprom_read_word(&user1));
        else if (nr == 2)
            return (eeprom_read_word(&user2));
    }
    
    if (func == WRITE)
    {
        
        if (nr == 1)
            eeprom_write_word(&user1, content);
        else if (nr == 2)
            eeprom_write_word(&user2, content);
    }

    return (FALSE);
    
}
*/


word yackwpm(void)
{
    
    return wpm; 
    
}


void yackspeed (byte dir, byte mode)
{
    
    if (mode == FARNSWORTH)
    {
        if ((dir == UP) && (farnsworth > 0))
            farnsworth--;
        
        if ((dir == DOWN) && (farnsworth < MAXFARN))
            farnsworth++;
    }
    else // WPMSPEED
    {
        if ((dir == UP) && (wpm < MAXWPM))
            wpm++;
        
        if ((dir == DOWN) && (wpm > MINWPM))
            wpm--;
        
        wpmcnt=(1200/YACKBEAT)/wpm; // Calculate beats

    }
    
    volflags |= DIRTYFLAG; // Set the dirty flag    
    volbfr |= DIRTYFLAG; // Set the dirty flag

    yackplay(DIT);
    yackdelay(IEGLEN);  // Inter Element gap  
    yackplay(DAH);
    yackdelay(ICGLEN);  // Inter Character gap  
    yackfarns(); // Additional Farnsworth delay
    
}



void yackbeat(void)
{
	/* Attiny45
    while((TIFR & (1<<OCF1A)) == 0);	// Wait for Timeout
    TIFR |= (1<<OCF1A);					// Reset output compare flag
	*/
	// Atmega32
	// timer1
	//while((TIFR & (1<<OCF1B)) == 0);		// wait for timeout
	//TIFR |= (1<<OCF1B);					// reset output compare flag

	//timer 0
	//while((TIFR & (1<<OCF0)) == 0);		// wait for timeout
	//TIFR |= (1<<OCF0);					// reset output compare flag
	delay(YACKBEAT);
}


/*
void yackpitch (byte dir)
{
    if (dir == UP)
        ctcvalue--;
    if (dir == DOWN)
        ctcvalue++;
    
    if (ctcvalue < MAXCTC)
        ctcvalue = MAXCTC;
    
    if (ctcvalue > MINCTC)
        ctcvalue = MINCTC;
    
    volflags |= DIRTYFLAG; // Set the dirty flag    
    
}
*/



void yacktune (void)
{
    word timer = YACKSECS(TUNEDURATION);
    
    key(DOWN);
    
    while(timer && (KEYINP & (1<<DITPIN)) && (KEYINP & (1<<DAHPIN)) && !yackctrlkey(TRUE) )
    {
        timer--;
        yackbeat();
    }
    
    key(UP);
}




void yackmode (byte mode)
{

    yackflags &= ~MODE;
    yackflags |= mode;
    
    volflags |= DIRTYFLAG; // Set the dirty flag    

}


byte yackflag(byte flag)
{
    return yackflags & flag;
}



void yacktoggle(byte flag)
{
    
    yackflags ^= flag;      // Toggle the feature bit
    volflags |= DIRTYFLAG;  // Set the dirty flag   

}




void yackerror (void)
{
    byte i;
    
    for (i=0;i<8;i++)
    {
        yackplay(DIT);
        yackdelay(DITLEN);
    }
    yackdelay(DAHLEN);
    
}




// ***************************************************************************
// CW Playback related functions
// ***************************************************************************

static void key(byte mode)
{
    if (mode == DOWN) 
    {
        if (volflags & SIDETONE) // Are we generating a Sidetone?
        {
            /* Attiny45
			OCR0A = ctcvalue;       // Then switch on the Sidetone generator
            OCR0B = ctcvalue;		// to jest istotne pewnie
            
            // Activate CTC mode
            TCCR0A |= (1<<COM0B0 | 1<<WGM01);		// Toggle OC0B (PB1 - sound out) on Compare Match; mode CTC TOP OCRA
            
            // Configure prescaler
            TCCR0B = 1<<CS01;		// prescaler set to 8
			*/
			
			//**********************************
			// Atmega32
			analogWrite(TONE_CW_PIN, 127); // <- ręcznie - timer1 (wyjście OC1A - PD5):
			/*
			OCR1A = ctcvalue;
			//OCR1B = ctcvalue;
			OCR1A = 39;
			TCCR1A |= (1<<COM1A0);
			*/
			//TCCR1B |= (1<<CS12) | (1<<WGM12) ;		// prescaler set to 256; CTC dla OC1A, OC1B
        }

        key_mode = true;
		VOX_time = millis();
        
        if (volflags & TXKEY) // Are we keying the TX?
        {
            if (yackflags & TXINV) // Do we need to invert keying?
                CLEARBIT(OUTPORT,OUTPIN);
            else
                SETBIT(OUTPORT,OUTPIN);
    		ptt_on = true;
#if defined(USE_OE_MIX)
				clk_enable(CWO);
#endif
   		if(last_ptt_state != ptt_on)	//sprawdzam czy zmienił się stan PTT jeśli tak to robię update na LCD
    		{
				digitalWrite(NO_PIN, HIGH);
				if (split_on)
				{
					if (active_vfo == 0) // jeśli aktualnie jest wybrane VFO_A - przełączenie na VFO_B, jeśli jest B - nie rób nic
					{
						active_vfo = 1;
						change_vfo();
					}
    			}
    			set_frequency(0);
    			last_ptt_state = ptt_on;               //uaktualniam poprzedni stan PTT
    		}
    		show_NO();
        }

    }
    
    if (mode == UP) 
    {
        if (volflags & SIDETONE) // Sidetone active?
        {
			analogWrite(TONE_CW_PIN, 0); // <- ręcznie - timer1 (wyjście OC1A - PD5):
        }
        if (volflags & TXKEY) // Are we keying the TX?
        {
        	if (yackflags & TXINV) // Do we need to invert keying?
        	{
                SETBIT(OUTPORT,OUTPIN);
        	}
            else
            {
                CLEARBIT(OUTPORT,OUTPIN);
            }
        }
    }
}



void yackfarns(void)
{
    
    word i=farnsworth;
    
    while (i--)
    {
        yackdelay(1);
    }
    
}



void yackdelay(byte n)
{
    
    byte i=n;
    byte x;
    
    while (i--)
    {
        x=wpmcnt;
        while (x--)    
            yackbeat();
    }
    
}




void yackplay(byte i) 

{
    
    key(DOWN); 

    switch (i)
    {
        case DAH:
            yackdelay(DAHLEN);
            break;
            
        case DIT:
            yackdelay(DITLEN);
            break;
    }
    
#if defined(USE_OE_MIX)
				clk_disable(CWO);
#endif
    key(UP);
}




void yackchar(char c)
{
    byte    code=0x80; // 0x80 is an empty morse character (just eoc bit set)
    byte    i; // a counter
    
    // First we need to map the actual character to the encoded morse sequence in
    // the array "morse"
    if(c>='0' && c<='9') // Is it a numerical digit?
        code = pgm_read_byte(&morse[c-'0']); // Find it in the beginning of array
    
    if(c>='a' && c<='z') // Is it a character?
        code = pgm_read_byte(&morse[c-'a'+10]); // Find it from position 10
    
    if(c>='A' && c<='Z') // Is it a character in upper case?
        code = pgm_read_byte(&morse[c-'A'+10]); // Same as above
    
    // Last we need to handle special characters. There is a small char
    // array "spechar" which contains the characters for the morse elements
    // at the end of the "morse" array (see there!)
    for(i=0;i<sizeof(spechar);i++) // Read through the array
        if (c == pgm_read_byte(&spechar[i])) // Does it contain our character
            code = pgm_read_byte(&morse[i+36]); // Map it to morse code
    
    if(c==' ') // Do they want us to transmit a space (a gap of 7 dots)
        yackdelay(IWGLEN-ICGLEN); // ICG was already played after previous char
    else
    {
        while (code != 0x80) // Stop when EOC bit has reached MSB
        {
            if (yackctrlkey(FALSE)) // Stop playing if someone pushes key
                return;
            
            if (code & 0x80)    // MSB set ?
                yackplay(DAH);      // ..then play a dash
            else                // MSB cleared ?
                yackplay(DIT);      // .. then play a dot
            
            yackdelay(IEGLEN);  // Inter Element gap  
            
            code = code << 1;   // Shift code on position left (to next element)
        }
        
        yackdelay(ICGLEN - IEGLEN); // IEG was already played after element

        yackfarns(); // Insert another gap for farnsworth keying

    }
    
}



void yackstring(const char *p)
{
    
    char c;
    
    while ((c = pgm_read_byte(p++))&& !(yackctrlkey(FALSE)) )
        // While end of string in flash not reached and ctrl not pressed
        yackchar(c);            // Play the read character
                                // abort now if someone presses command key
    
}



void yacknumber(word n)
{
    
    char buffer[5];
    byte i = 0;
    
    while (n) // Until nothing left
    {
        buffer[i++] = n%10+'0'; // Store rest of division by 10
        n /= 10;                // Divide by 10
    }
    
    while (i)
    {
        yackchar(buffer[--i]);
    }
    
    yackchar (' ');
    
}



// ***************************************************************************
// CW Keying related functions
// ***************************************************************************


static void keylatch(void)
{
    
    byte    swap;    // Status of swap flag
    
    swap    = ( yackflags & PDLSWAP);
    
    if (!( KEYINP & (1<<DITPIN)))
        volflags |= (swap?DAHLATCH:DITLATCH);
    
    if (!( KEYINP & (1<<DAHPIN)))
        volflags |= (swap?DITLATCH:DAHLATCH);
    
}



byte yackctrlkey(byte mode)
{
    
    //byte volbfr;
    
    volbfr = volflags; // Remember current volatile settings
    
    //if (!(BTNINP & (1<<BTNPIN))) // If command button is pressed
    if(getKey() == M1_KEY)
    {
        
        volbfr |= CKLATCH; // Set control key latch
        
        // Apparently the control key has been pressed. To avoid bouncing
        // We will now wait a short while and then busy wait until the key is
        // released.
        // Should we find that someone is keying the paddle, let him change
        // the speed and pretend ctrl was never pressed in the first place..

        yackinhibit(ON); // Stop keying, switch on sidetone.
        
        _delay_ms(50);
        
        //while(!(BTNINP & (1<<BTNPIN))) // Busy wait for release
        while(getKey() == M1_KEY)
        {
            
            if (!( KEYINP & (1<<DITPIN))) // Someone pressing DIT paddle
            {
                yackspeed(DOWN,WPMSPEED);
                volbfr &= ~(CKLATCH); // Ignore that control key was pressed
            }   
            
            if (!( KEYINP & (1<<DAHPIN))) // Someone pressing DAH paddle
            {
                yackspeed(UP,WPMSPEED);
                volbfr &= ~(CKLATCH);
            }   
        }
        
        _delay_ms(50); // Trailing edge debounce    
        
    }

    volflags = volbfr; // Restore previous state - nadpsuje bit DIRYFLAG ustawiony w yackspeed - bug!

    if (mode==TRUE) // Does caller want us to reset latch?
    {
        volflags &= ~(CKLATCH);
    }
    
    yacksave(); // In case we had a speed change
    
    return((volbfr&CKLATCH)!=0); // Tell caller if we had a ctrl button press
    
}



static char morsechar(byte buffer)
{
    byte i;
    
    for(i=0;i<sizeof(morse);i++)
    {
        
        if (pgm_read_byte(&morse[i]) == buffer)
        {
            if (i < 10) return ('0' + i);       // First 10 chars are digits
            if (i < 36) return ('A' + i - 10);  // Then follow letters
            return (pgm_read_byte(&spechar[i - 36])); // Then special chars
        }
        
    }
    
    return '\0';
}



void yackmessage(byte function, byte msgnr)
{
    unsigned char   rambuffer[RBSIZE];  // Storage for the message
    unsigned char   c;                  // Work character
    
    word            extimer = 0;        // Detects end of message (10 sec)
    
    byte            i = 0;              // Pointer into RAM buffer
    byte            n;                  // Generic counter
    
    if (function == RECORD)
    {

        extimer = YACKSECS(DEFTIMEOUT); // 5 Second until message end
        while(extimer--)    // Continue until we waited 10 seconds
        {
            if (yackctrlkey(FALSE)) return;
            
            if ((c = yackiambic(ON))) // Check for a character from the key
            {
                rambuffer[i++] = c; // Add that character to our buffer
                extimer = YACKSECS(DEFTIMEOUT); // Reset End of message timer
            }
            
            if (i>=RBSIZE) // End of buffer reached?
            {
                yackerror();
                i = 0;
            }
            
            yackbeat(); // 10 ms heartbeat
        }   
        
        // Extimer has expired. Message has ended
        
        if(i) // Was anything received at all?
        {
            rambuffer[--i] = 0; // Add a \0 end marker over last space
            
            // Replay the message
            for (n=0;n<i;n++)
                yackchar(rambuffer[n]);
            
            // Store it in EEPROM

            if (msgnr == 1)
            {
                eeprom_write_block(rambuffer, eebuffer1, RBSIZE);
            }
            if (msgnr == 2)
            {
                eeprom_write_block(rambuffer, eebuffer2, RBSIZE);
            }
            if (msgnr == 3)
            {
                eeprom_write_block(rambuffer, eebuffer3, RBSIZE);
            }
            //config_dirty = 1;
        }
        else
            yackerror();
    }
    
    
    if (function == PLAY)
    {
        // Retrieve the message from EEPROM
        if (msgnr == 1)
        {
            eeprom_read_block(rambuffer, eebuffer1, RBSIZE);
        }
        if (msgnr == 2)
        {
            eeprom_read_block(rambuffer, eebuffer2, RBSIZE);
        }
        if (msgnr == 3)
        {
            eeprom_read_block(rambuffer, eebuffer3, RBSIZE);
        }
        // Replay the message
        for (n=0;(c=rambuffer[n]);n++) // Read until end of message
            yackchar(c); // play it back 
    }
}




char yackiambic(byte ctrl)
{
    
    static enum FSMSTATE    fsms = IDLE;    // FSM state indicator
    static      word        timer;          // A countdown timer
    static      byte        lastsymbol;     // The last symbol sent
    static      byte        buffer = 0;     // A place to store a sent char
    static      byte        bcntr = 0;      // Number of elements sent
    static      byte        iwgflag = 0;    // Flag: Are we in interword gap?
    static      byte        ultimem = 0;    // Buffer for last keying status
                char        retchar;        // The character to return to caller
    
    // This routine is called every YACKBEAT ms. It starts with idle mode where
    // the morse key is polled. Once a contact close is sensed, the TX key is 
    // closed, the sidetone oscillator is fired up and the FSM progresses
    // to the next state (KEYED). There it waits for the timer to expire, 
    // afterwards progressing to IEG (Inter Element Gap).
    // Once the IEG has completed, processing returns to the IDLE state.
    
    // If the FSM remains in idle state long enough (one dash time), the
    // character is assumed to be complete and a decoding is attempted. If
    // succesful, the ascii code of the character is returned to the caller
    
    // If the FSM remains in idle state for another 4 dot times (7 dot times 
    // altogether), we assume that the word has ended. A space char
    // is transmitted in this case.
    
    if (timer) timer--; // Count down
    
    if (ctrl == OFF) iwgflag = 0; // No space detection
    
    switch (fsms)
    {
            
        case IDLE:
            
            keylatch();
           
            // Handle latching logic for various keyer modes
            switch (yackflags & MODE)
            {
                case IAMBICA:
                case IAMBICB:
                    // When the paddle keys are squeezed, we need to ensure that
                    // dots and dashes are alternating. To do that, whe delete
                    // any latched paddle of the same kind that we just sent.
                    // However, we only do this ONCE
                    
                    volflags &= ~lastsymbol;
                    lastsymbol = 0;
                    break;                    
                 
                case ULTIMATIC:
                    // Ultimatic logic: The last paddle to be active will be repeated indefinitely
                    // In case the keyer is squeezed right out of idle mode, we just send a DAH 
                    if ((volflags & SQUEEZED) == SQUEEZED) // Squeezed?
                    {
                        if (ultimem)
                          volflags &= ~ultimem; // Opposite symbol from last one
                        else
                          volflags &= ~DITLATCH; // Reset the DIT latch
                    }
                    else
                    {
                        ultimem = volflags & SQUEEZED; // Remember the last single key
                    }

                    break;
                            
                case DAHPRIO:            
                    // If both paddles pressed, DAH is given priority
                    if ((volflags & SQUEEZED) == SQUEEZED)
                    {
                        volflags &= ~DITLATCH; // Reset the DIT latch
                    }
                    break;
            }        
            
            
            // The following handles the inter-character gap. When there are
            // three (default) dot lengths of space after an element, the
            // character is complete and can be returned to caller
            if (timer == 0 && bcntr != 0) // Have we idled for 3 dots
                // and is there something to decode?
            {
                buffer = buffer << 1;     // Make space for the termination bit
                buffer |= 1;              // The 1 on the right signals end
                buffer = buffer << (7-bcntr); // Shift to left justify
                retchar = morsechar(buffer); // Attempt decoding
                buffer = bcntr = 0;         // Clear buffer
                timer = (IWGLEN - ICGLEN) * wpmcnt; // If 4 further dots of gap,
                // this might be a Word gap.
                iwgflag = 1;                // Signal we are waiting for IWG                          
                return (retchar);           // and return decoded char
            }
            
            // This handles the Inter-word gap. Already 3 dots have been
            // waited for, if 4 more follow, interpret this as a word end
            if (timer == 0 && iwgflag) // Have we idled for 4+3 = 7 dots?
            {
                iwgflag = 0;   // Clear Interword Gap flag
                return (' ');  // And return a space
            }
            
            // Now evaluate the latch and determine what to send next
            if ( volflags & (DITLATCH | DAHLATCH)) // Anything in the latch?
            {
                iwgflag = 0; // No interword gap if dit or dah
                bcntr++;    // Count that we will send something now
                buffer = buffer << 1; // Make space for the new character
                
                if (volflags & DITLATCH) // Is it a dit?
                {
                    timer   = DITLEN * wpmcnt; // Duration = one dot time
                    lastsymbol = DITLATCH; // Remember what we sent
                }
                else // must be a DAH then..
                {
                    timer   = DAHLEN * wpmcnt; // Duration = one dash time
                    lastsymbol = DAHLATCH; // Remember
                    buffer |= 1; // set LSB to remember dash
                }
                
                key(DOWN); // Switch on the side tone and TX
                volflags &= ~(DITLATCH | DAHLATCH); // Reset both latches
                
                fsms    = KEYED; // Change FSM state
            }
            
            break;
            
        case KEYED:

            if ((yackflags & MODE) == IAMBICB) // If we are in IAMBIC B mode
                keylatch();                      // then latch here already 
            
            if(timer == 0) // Done with sounding our element?
            {
#if defined(USE_OE_MIX)
				clk_disable(CWO);
#endif
                key(UP); // Then cancel the side tone
                timer   = IEGLEN * wpmcnt; // One dot time for the gap
                fsms    = IEG; // Change FSM state
            }
            
            break;
            
            
        case IEG:
            
            keylatch(); // Latch any paddle movements (both A and B)
            
            if(timer == 0) // End of gap reached?
            {
                fsms    = IDLE; // Change FSM state
                // The following timer determines what the IDLE state
                // accepts as character. Anything longer than 2 dots as gap will be
                // accepted for a character end.
                timer   = (ICGLEN - IEGLEN -1) * wpmcnt; 
            }
            break;
            
    }
    
    return '\0'; // Nothing to return if not returned in above routine
    
}
