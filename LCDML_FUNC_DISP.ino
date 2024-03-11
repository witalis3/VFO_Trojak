/* ===================================================================== *
 *                                                                       *
 * DISPLAY SYSTEM                                                        *
 *                                                                       *
 * ===================================================================== *
 * every "disp menu function" needs three functions 
 * - void LCDML_DISP_setup(func_name)    
 * - void LCDML_DISP_loop(func_name)     
 * - void LCDML_DISP_loop_end(func_name)
 *
 * EXAMPLE CODE:
    void LCDML_DISP_setup(..menu_func_name..) 
    {
      // setup
      // is called only if it is started

      // starts a trigger event for the loop function every 100 millisecounds
      LCDML_DISP_triggerMenu(100);  
    }
    
    void LCDML_DISP_loop(..menu_func_name..)
    { 
      // loop
      // is called when it is triggert
      // - with LCDML_DISP_triggerMenu( millisecounds ) 
      // - with every button status change

      // check if any button is presed (enter, up, down, left, right)
      if(LCDML_BUTTON_checkAny()) {         
        LCDML_DISP_funcend();
      } 
    }
    
    void LCDML_DISP_loop_end(..menu_func_name..)
    {
      // loop end
      // this functions is ever called when a DISP function is quit
      // you can here reset some global vars or do nothing  
    } 
 * ===================================================================== *
 */

// *********************************************************************
// Zmiana częstotliwości pośredniej
// *********************************************************************
long f;		// tymczasowa wartość pośredniej
void show_IF(){
	sprintf(bufor, "%3lu.%03lu.%02lu", (f/1000000)%1000, (f/1000)%1000, (f%1000)/10);
	tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
	tft.setTextSize(5);
	tft.setCursor(10, 10);
	tft.print(bufor);		
}
void LCDML_DISP_setup(LCDML_FUNC_change_IF)
{
	setText2G
	tft.setCursor(0, 140);
	tft.println(TXT_CHANGE_IF_1);
	tft.print(TXT_CHANGE_IF_2);
	f = configuration.if_frequency;
	show_IF();
	show_step();
	tft.setCursor(2, 75);
	tft.print(TXT_CHANGE_IF_3);
	LCDML_DISP_triggerMenu(10);
}
void LCDML_DISP_loop(LCDML_FUNC_change_IF)
{
	krok.update();
	if (krok.read() == LOW)
	{
		set_step();
	}
	int enc = encode_read4();
	//step = 1000000UL;
	f = constrain(f + enc*step, 10000UL, 100000000UL);
	show_IF();
	if(LCDML_BUTTON_checkAny()) 
	{	// check if any button is presed (enter, up, down, left, right)
		// LCDML_DISP_funcend calls the loop_end function
    //if (LCDML_BUTTON_checkUp())    { HELP_FUNC_set_dat(+1); LCDML_BUTTON_resetUp(); }
    //if (LCDML_BUTTON_checkDown())  { HELP_FUNC_set_dat(-1);  LCDML_BUTTON_resetDown(); }
		if (LCDML_BUTTON_checkEnter()) 
		{ 
			configuration.if_frequency = f;	// ustawienie nowej pośredniej
			config_dirty = 1;	
			LCDML_BUTTON_resetEnter();
			LCDML_DISP_funcend();
		}
	}
}
void LCDML_DISP_loop_end(LCDML_FUNC_change_IF)
{
	step = 100;
}
// *********************************************************************
// wybór pasm
// *********************************************************************
char aktywne_pasmo = 0;
void show_pasma()
{
	setText2G
	tft.setCursor(0, 0);
	tft.println(TXT_PASMA);
	tft.setCursor(0, 23);	
	for (int i = 0; i < ile_pasm; i++)
	{
		tft.print(bands[i].nazwa);
		sprintf(bufor, " [%c][%c] %s", (configuration.status_pasma[i] > 0)?'x':' ', (configuration.status_pasma[i] == 2)? 'x' : ' ', (i == aktywne_pasmo)? "<-" : "  " );
		tft.println(bufor);
	}
}
void LCDML_DISP_setup(LCDML_FUNC_pasma)
{
	LCDML_DISP_triggerMenu(100);
	show_pasma();
}
void LCDML_DISP_loop(LCDML_FUNC_pasma)
{
	if(LCDML_BUTTON_checkAny()) 
	{
		if (LCDML_BUTTON_checkDown())    
		{ 
			aktywne_pasmo++;
			aktywne_pasmo = (aktywne_pasmo > ile_pasm - 1) ? ile_pasm - 1 : aktywne_pasmo;
			LCDML_BUTTON_resetDown(); 
		}
		if (LCDML_BUTTON_checkUp())  
		{ 
			aktywne_pasmo--;
			aktywne_pasmo = (aktywne_pasmo < 0)? 0 : aktywne_pasmo;
			LCDML_BUTTON_resetUp(); 
		}
		if (LCDML_BUTTON_checkEnter()) 
		{ 
			// zmiana statusu
			switch (configuration.status_pasma[aktywne_pasmo])
			{
				case 0:
					configuration.status_pasma[aktywne_pasmo] = 1;
					break;
				case 1:
					configuration.status_pasma[aktywne_pasmo] = 2;
					break;
				case 2:
					configuration.status_pasma[aktywne_pasmo] = 0;
					break;
				default:
					configuration.status_pasma[aktywne_pasmo] = 0;
			}
			config_dirty = 1;
			LCDML_BUTTON_resetEnter();
		}
		show_pasma();
	}	
}
void LCDML_DISP_loop_end(LCDML_FUNC_pasma)
{
}
// *********************************************************************
// wybór trybu pracy
// *********************************************************************
void show_tryby()
{
	setText2G
	tft.setCursor(0, 0);
	tft.print(TXT_TRYB_1);
	tft.setCursor(180, 0);
	tft.print(configuration.mode);
	tft.setCursor(0, 23);		
	tft.println(TXT_TRYB_2);
	tft.println(TXT_TRYB_3);
	tft.println(TXT_TRYB_4);
}
void LCDML_DISP_setup(LCDML_FUNC_tryb)
{
	LCDML_DISP_triggerMenu(100);  
	show_tryby();
}
void LCDML_DISP_loop(LCDML_FUNC_tryb)
{
	if(LCDML_BUTTON_checkAny()) 
	{
		if (LCDML_BUTTON_checkUp())    
		{ 
			configuration.mode++;
			configuration.mode = (configuration.mode > 4) ? 4 : configuration.mode;
			LCDML_BUTTON_resetUp(); 
		}
		if (LCDML_BUTTON_checkDown())  
		{ 
			configuration.mode--;
			configuration.mode = (configuration.mode < 0)? 0 : configuration.mode;
			LCDML_BUTTON_resetDown(); 
		}
		if (LCDML_BUTTON_checkEnter()) 
		{
			config_dirty = 1;
			LCDML_BUTTON_resetEnter();
			LCDML_DISP_funcend();
		}
		show_tryby();
	}	
}
void LCDML_DISP_loop_end(LCDML_FUNC_tryb)
{
	si5351_clk_setup();
}
// *********************************************************************
// ustawienie czasu dla BK
// *********************************************************************
void show_czas_bk()
{
	setText2G
	tft.setCursor(0, 0);
	tft.print(TXT_BK);
	tft.setCursor(156, 0);
	tft.print(configuration.bk_time);
}
void LCDML_DISP_setup(LCDML_FUNC_czas_bk)
{
	show_czas_bk();
	LCDML_DISP_triggerMenu(10);  
}
void LCDML_DISP_loop(LCDML_FUNC_czas_bk)
{
	int enc = encode_read4();
	if (enc)
	{
		configuration.bk_time = configuration.bk_time + enc*10;
		configuration.bk_time = (configuration.bk_time > 999) ? 999 : configuration.bk_time;
		show_czas_bk();
	}
	if(LCDML_BUTTON_checkAny()) 
	{
		if (LCDML_BUTTON_checkEnter()) 
		{
			config_dirty = 1;
			LCDML_BUTTON_resetEnter();
			LCDML_DISP_funcend();
		}
	}	
}
void LCDML_DISP_loop_end(LCDML_FUNC_czas_bk)
{
}
// *********************************************************************
// ustawienie częstotliwości generatora kwarcowego Si5351
// *********************************************************************

void show_korekcja_si()
{
	setText2G
	tft.setCursor(0, 0);
	tft.print(TXT_CRYSTAL);
	tft.setCursor(204, 0);
	tft.print(configuration.si_rezonator);
}
void LCDML_DISP_setup(LCDML_FUNC_korekcja_si)
{
	show_korekcja_si();
	show_step();
	tft.setCursor(2, 75);
	tft.print(TXT_CHANGE_IF_3);
	LCDML_DISP_triggerMenu(10);
	// ustawienie na wyjściu VFO generatora kwarcowego - do odczytu i ustawienia w setupie

	write_register(16, B00000001);			// CLK0: powered up, integer mode, PLL_A, not inverted, Multisynth, 4mA
	write_register(187, read_register(187) | (1<<6));
	/*
	write_register(17, B01100001);			// CLK1: powered up, integer mode, PLL_B, not inverted, Multisynth, 4mA
	write_register(18, B01100001);			// CLK2: powered up, integer mode, PLL_B, not inverted, Multisynth, 4mA
	clk_enable(VFO);
	clk_enable(BFO);
	clk_enable(CWO);
	*/
}
void LCDML_DISP_loop(LCDML_FUNC_korekcja_si)
{
	krok.update();
	if (krok.read() == LOW)
	{
		set_step();
	}
	int enc = encode_read4();
	if (enc)
	{
		configuration.si_rezonator = configuration.si_rezonator + enc*step;
		//configuration.si_rezonator = (configuration.si_rezonator > 9999999L) ? 9999999L : configuration.si_rezonator;
		configuration.si_rezonator = constrain(configuration.si_rezonator, 24900000L, 27100000L);
		show_korekcja_si();
	}
	if(LCDML_BUTTON_checkAny())
	{
		if (LCDML_BUTTON_checkEnter())
		{
			config_dirty = 1;
			LCDML_BUTTON_resetEnter();
			LCDML_DISP_funcend();
			#if defined(USE_Si5351Arduino_LIB)
				si5351.set_correction(configuration.si_correction);
			#endif
		}
	}
}
void LCDML_DISP_loop_end(LCDML_FUNC_korekcja_si)
{
	// krok w zakresie
	step = 100;
	// powrór do normalnych utawień - VFO z PLL_A
	write_register(16, B00001101);			// CLK0: powered up, integer mode, PLL_A, not inverted, Multisynth, 4mA
	write_register(187, read_register(187) & ~(1<<6));
}
// *********************************************************************
// ustawienie odstępu pilotów od częstotliwości środkowej filtru - wartość IFoffset
// *********************************************************************

void show_odstep()
{
	setText2G
	tft.setCursor(0, 0);
	tft.print(TXT_BFO_OFFSET);
	tft.setCursor(204, 0);
	tft.print(configuration.IFoffset);
}
void LCDML_DISP_setup(LCDML_FUNC_odstep_pilotow)
{
	show_odstep();
	LCDML_DISP_triggerMenu(10);
}
void LCDML_DISP_loop(LCDML_FUNC_odstep_pilotow)
{
	unsigned char krok = 10;
	int enc = encode_read4();
	if (enc)
	{
		configuration.IFoffset = constrain(configuration.IFoffset + enc*krok, 0, 2500);
		show_odstep();
	}
	if(LCDML_BUTTON_checkAny()) 
	{
		if (LCDML_BUTTON_checkEnter()) 
		{
			config_dirty = 1;
			LCDML_BUTTON_resetEnter();
			LCDML_DISP_funcend();
		}
	}	
}
void LCDML_DISP_loop_end(LCDML_FUNC_odstep_pilotow)
{
}
// *********************************************************************
// włączanie i wyłączanie BFO
// *********************************************************************

void show_bfo_on_off()
{
	setText2G
	tft.setCursor(0, 0);
	tft.print(TXT_BFO_ON_OFF);
	tft.setCursor(180, 0);
	tft.print(configuration.bfo_on & 0x01?'x':' ');
}
void LCDML_DISP_setup(LCDML_FUNC_bfo_on_off)
{
	show_bfo_on_off();
	LCDML_DISP_triggerMenu(10);
}
void LCDML_DISP_loop(LCDML_FUNC_bfo_on_off)
{
	if(LCDML_BUTTON_checkAny())
	{
		if (LCDML_BUTTON_checkEnter())
		{
#if defined(DEBUG)
			Serial.print("conf.bfo_on przed: ");
			Serial.println(configuration.bfo_on, BIN);
#endif
			//configuration.bfo_on = configuration.bfo_on & 0x01;
			configuration.bfo_on ^= (1<<0);
#if defined(DEBUG)
			Serial.print("conf.bfo_on po: ");
			Serial.println(configuration.bfo_on, BIN);
#endif
			config_dirty = 1;
			LCDML_BUTTON_resetEnter();
			show_bfo_on_off();
			si5351_clk_setup();
		}
	}
}
void LCDML_DISP_loop_end(LCDML_FUNC_bfo_on_off)
{
}
