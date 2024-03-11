/*
 * written by Witold Buszkiewicz SP3JDZ
 * created 2016-08-22
 * wersja z Atmega32
 *
 */
/*
********************************************************
 Spostrzeżenia itd.
- stabilizator 3,3V grzeje się (parzy) - lepiej, żeby był poziomo na płytce chłodzony - może mały? - raczej duży TO220 lub podobny mniejszy
- stabilizator 5V wystarczy mały 1157
- raster dla przycisków 12,5mm zamiast 15mm (u mnie)
	- zwrócić uwagę na położenie względem pola wyświetlacza (ew. CENTER)
********************************************************
 CHANGELOG
 2017.07.30 - v.1.1.6b
 	 - opóźnienie inicjalizacji wyświetlacza
 2017.06.23 - v.1.1.6a
 	 - pośrednia od 10kHz do 99MHz
 2017.06.15 - v.1.1.6
 	 - tryby pracy startowe: LSB/USB
 	 - S-metr 0,5V na pełną skalę
 2017.06.04 - v.1.1.5
 	 - poprawienie obsługi zmiany trybu na pasmach
 2017.05.21 - v.1.1.4 (na bazie 1.1.3d; wersja 1.1.3.e pominięta ze względu na brak pamięci)
 	 - uruchamianie trzeciego generatora (CWO) dla CW
 2017.02.12 - v.1.1.3d
 	 - poprawienie poszerzenia dla 10MHz
 	 - zwiększenie czasu strobe dla CD4514 do 50ms
 2017.02.10 - v.1.1.3c
 	 - poszerzenie pasm (przestrajanie w całym pasmie)
 2017.02.06 - v.1.1.3b
 	 - pasma LF
 2017.01.29 - v.1.1.3a
 	 - uwagi
 	 	 - 2xR20
 	 - pasmo 1,8 od dołu bez limitu
 	 - podświetlanie opisu klawisza CMD
 	 - poprawienie pracy klucza w trybie "K" bez nadawania
 2016.11.12 - v.1.1.3
 	 - dołączenie klucza Yack
		- zmiana prędkości enkoderem
		- obsługa pamięci przez klawisze
	- korekcja dla Si5351 - dostosowanie do nowych funkcji Si5351
		- podczas ustawiania częstotliwości rezonatora - na wyjściu ONa
	- język angielski w menu
	- wskaźnik nadawania - czerwona kropa
	- BK na sztorcu
	- setup
		- po starcie CWO wyłączony
		- wyłączanie BFO w menu
		- w trybie SDR (>0) wyłączanie BFO

TODO
	- test ILI9341_t3 może będzie mniejsza?
	- test mikromenu https://github.com/abcminiuser/micromenu-v2

	- #define KLAWISZE_MOJE - zrobione
		- klawisze_moje
	- #define YACK_MEMO - zrobione
		- pamięci klucza na ATT, Filtrze i DWN
	- testy odbioru na Graciku
		- stuki przy przestrajaniu - zrobione - wyłączenie resetu PLLi

 2016.11.12 - v.1.1.2
 	 - zmiana obsługi Si5351 na mix z OE1CGS, SQ9NJE
 	 - zmiana obsługi I2C z Wire na bibliotekę I2C (krótsza, szybsza, z bardziej rozbudowaną obsługą błędów)
 	 - uzyskanie dzięki powyższym 4kB pamięci flash (28 490 bajtów po skompilowaniu)
 2016.11.07 - v.1.1.1 produkcja
 	 - setup
 	 	 - odstęp pilotów od środka filtru (IFoffset)
 2016.11.04 - v.1.1.0 odskocznia
	- zamiast Freqency2 ręczne ustawienie timer2 i przerwań dla enkodera - zaoszczędzenie miejsca - nic z tego!!!
		- Si5351_P biblioteka - kolejne podejście
		- skorzystać z biblioteki I2C
	- a może biblioteka OE1CGS?
		- OE1CGS + I2C -> OK patrz wersja 1.1.2
 2016.11.01 - v.1.0.9
	- dopasowanie opisów klawiszy do przycisków
	- opisy klawiszy podczas setupu
		- enter
		- quit
		- up
		- down
		- setup
	- wyłączenie update s-metra i u_zas na czas kluczowania

	- testy
		- poziomy z VFO
		- zgodność częstotliwości z oczekiwaniami
			- czy generuje dla różnych IF (3-5-12-30MHz)?
				- 3MHz nie generuje 145MHz (dałem zakres IF od 4MHz...)
			- co po wyłączeniu SPLITu? (jak jest w złomku?)
			- USB zamienione z LSB? chyba nie
		- wejścia i wyjścia
		- sterowanie pasmami
			- wyjścia CD4514
	- Yack - pamięci itd.
	- ew. deaktywacja pasm powyżej 28MHz dla trybu SDR (configuration.mode > 0)
	- setup
		- odstęp pilotów od środka filtru (IFoffset)
		- zmiana poziomów generatorów:2, 4, 6, 8mA
	- zmiana koloru częstotliwości dla SPLITu A/B
 2016.10.28 - v.1.0.8 
	- setup
		- wartość dla korekcji częstotliwości Si5351
			- zmieniłem wartość _LCDML_DISP_cfg_max_rows z 4 na 5 w pliku LCDMenuLib.h w bibliotece
	- dokończenie S-metra
		- linijka z klocków
	- umiejscowienie pomiaru napięcia
	- dokończenie SWR-metra
 2016.10.17 - v.1.0.7
	- wersja z kluczem Yack - podejście
 2016.10.15 - v.1.0.6
	- kolejne podejście do K3NG
		- zamiana biblioteki Si na mniejszą - miejsce
 2016.10.07 - v.1.0.5
	- klucz CW własny ale kulawy
		- ok bk
			- ok w setupie regulowane opóźnienie
		- ok podsłuch PWM
	- osobne pliki na kody klawiszy
		- dwie wersje: na moją płytkę uniwersalną i na prototyp
 2016.10.02 - v.1.0.4
	- biblioteka LCDMenu do obsługi setupu
		- setup
			- ustawienie IF
			- włączanie/wyłączanie pasm
			- ustawienie trybu: pośrednia/mnożnik
 2016.09.12 - v.1.0.3
	- dołożenie obsługi klucza: K3NG
	- zmiana biblioteki do Si5351 na mniej pamięciożerną
 2016.09.22 - v v.1.0.3.a
	- testy Si5351 trzy generatory
	- bez klucza K3NG
*/
//#define ENGLISH
//#define KLAWISZE_MOJE
//#define YACK_MEMO
//#define DEBUG
//#define DEBEK
//#define CZAS_PETLI	// uruchamia impulsy na PD7 - stan zmienia się za każdym obiegiem głównej pętli
//#define USE_Si5351Arduino_LIB
//#define USE_Si5351_P_LIB
#if defined(DEBUG)
#define WERSJA "v 1.1.6b debug"
#else
#if defined(ENGLISH)
#define WERSJA "v 1.1.6b eng"
#else
#define WERSJA "v 1.1.6b"
#endif
#endif
#if defined(KLAWISZE_MOJE)
#include "klawisze_moje.h"
#else
#include "klawisze.h"
#endif
#define USE_OE_MIX
#include "yack.h"
#include "SPI.h"
//#include <Wire.h>
#if defined(USE_OE_MIX)
#include <I2C.h>
#endif

#include <PDQ_GFX.h>
#include "PDQ_ILI9341_config.h"			// PDQ: ILI9341 pins and other setup for this sketch
#include <PDQ_ILI9341.h>
#include <Bounce2.h>
#include <FrequencyTimer2.h>
//#include <TimerOne_32.h> nie działa delay()...
#include <LCDMenuLib.h>
#include <EEPROM.h>
// LCDMenuLib config
#define _LCDML_DISP_cfg_button_press_time          200    // button press time in ms
#define _LCDML_DISP_cfg_scrollbar                  0      // enable a scrollbar
#define _LCDML_DISP_cfg_cursor                     0x2D // minus; 0x7E - tylda   // cursor Symbol 
#define setText2G tft.setTextSize(2); tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);

// ********************************************************************* 
// LCDML TYPE SELECT
// *********************************************************************
  // settings for lcd 
  #define _LCDML_DISP_cols             20
  #define _LCDML_DISP_rows             7
  // przeliczniki dla tft
  unsigned char mnoznik = 16;	// dla czcionki x2
  unsigned char mnoznik2 = 12;	// j.w.
  unsigned char dy = 60;
// *********************************************************************
// LCDML MENU/DISP
// *********************************************************************
  // create menu
  // menu element count - last element id
  // this value must be the same as the last menu element
  #define _LCDML_DISP_cnt    7
  
  // LCDML_root        => layer 0 
  // LCDML_root_X      => layer 1 
  // LCDML_root_X_X    => layer 2 
  // LCDML_root_X_X_X  => layer 3 
  // LCDML_root_... 	 => layer ... 
  
  // LCDMenuLib_add(id, group, prev_layer_element, new_element_num, lang_char_array, callback_function)
  LCDML_DISP_init(_LCDML_DISP_cnt);
#if defined(ENGLISH)
#define MENU_1_1	"IF change"
#define MENU_1_2	"Bands"
#define MENU_1_3	"Mode"
#define MENU_1_4	"BK delay"
#define MENU_1_5	"Si5351 crystal"
#define MENU_1_6	"BFO offset"
#define MENU_1_7	"BFO On/Off"
#define TXT_CHANGE_IF_1		"change IF by encoder"
#define TXT_CHANGE_IF_2		"you may change step"
#define TXT_CHANGE_IF_3		"step:"
#define TXT_PASMA	"choose bands [act.][def]:"
#define TXT_TRYB_1	"choose mode:  [ ]"
#define TXT_TRYB_2	"for =0 - TRX with IF"
#define TXT_TRYB_3	"for >0 - SDR or homodyne"
#define TXT_TRYB_4	"     (multipl for VFO)"
#define TXT_BK		"BK delay:   [   ] ms"
#define TXT_CRYSTAL	"Si crystal freq:[        ] "
#define TXT_BFO_OFFSET	"BFO offset:     [    ]"
#define TXT_BFO_ON_OFF	"BFO is on:    [ ]"
#else
#define MENU_1_1	"Zmiana IF"
#define MENU_1_2	"Pasma"
#define MENU_1_3	"Tryb pracy"
#define MENU_1_4	"Czas op. BK"
#define MENU_1_5	"Rezonator Si5351"
#define MENU_1_6	"Odstep pilotow"
#define MENU_1_7	"wl/wyl BFO"
#define TXT_CHANGE_IF_1		"zmiana pcz enkoderem"
#define TXT_CHANGE_IF_2		"mozna zmieniac krok"
#define TXT_CHANGE_IF_3		"krok:"
#define TXT_PASMA	"wybierz pasma [akt.][def]:"
#define TXT_TRYB_1	"wybierz tryb: [ ]"
#define TXT_TRYB_2	"dla =0 - TRX z posrednia"
#define TXT_TRYB_3	"dla >0 - SDR lub homodyna"
#define TXT_TRYB_4	"     (mnoznik dla VFO)"
#define TXT_BK		"op. dla BK: [   ] ms"
#define TXT_CRYSTAL	"rezonator w Si: [        ] "
#define TXT_BFO_OFFSET	"odstep pilotow: [    ]"
#define TXT_BFO_ON_OFF	"BFO wlaczone: [ ]"
#endif
  LCDML_DISP_add      (0  , _LCDML_G1  , LCDML_root        , 1  , "Setup"        		, LCDML_FUNC);  
  LCDML_DISP_add      (1  , _LCDML_G1  , LCDML_root_1      , 1  , MENU_1_1	 	     	, LCDML_FUNC_change_IF);
  LCDML_DISP_add      (2  , _LCDML_G1  , LCDML_root_1      , 2  , MENU_1_2         	 	, LCDML_FUNC_pasma);
  LCDML_DISP_add      (3  , _LCDML_G1  , LCDML_root_1      , 3  , MENU_1_3		       	, LCDML_FUNC_tryb);
  LCDML_DISP_add      (4  , _LCDML_G1  , LCDML_root_1      , 4  , MENU_1_4	        	, LCDML_FUNC_czas_bk);
  LCDML_DISP_add      (5  , _LCDML_G1  , LCDML_root_1      , 5  , MENU_1_5			 	, LCDML_FUNC_korekcja_si);
  LCDML_DISP_add      (6  , _LCDML_G1  , LCDML_root_1      , 6  , MENU_1_6		 		, LCDML_FUNC_odstep_pilotow);
  LCDML_DISP_add      (7  , _LCDML_G1  , LCDML_root_1      , 7  , MENU_1_7		 		, LCDML_FUNC_bfo_on_off);
  LCDML_DISP_createMenu(_LCDML_DISP_cnt);
// ********************************************************************* 
// LCDML BACKEND (core of the menu, do not change here anything yet)
// ********************************************************************* 
  // define backend function  
  #define _LCDML_BACK_cnt    1  // last backend function id
  
  LCDML_BACK_init(_LCDML_BACK_cnt);
  LCDML_BACK_new_timebased_dynamic (0  , ( 20UL )         , _LCDML_start  , LCDML_BACKEND_control);
  LCDML_BACK_new_timebased_dynamic (1  , ( 1000UL )       , _LCDML_stop   , LCDML_BACKEND_menu);
  LCDML_BACK_create();


#define TFT_DC 24			// atmega32 24; nano 9
#define TFT_CS 4			// atmega32 4; nano 10
PDQ_ILI9341 tft = PDQ_ILI9341();
#define ENC_A	10
#define ENC_B	11
#define STEP_MAX	10000
#define STEP_MAX_KONF	1000000UL
#define STEP_MIN_KONF	1UL
#define STEP_MIN	10
#define VFO			0
#define BFO			1
#define CWO			2
//#define SI5351_PLL_A 62677980000ULL //87688420000ULL
#define SI5351_PLL_A 0ULL //SI5351_PLL_FIXED //87688420000ULL
#define SI5351_PLL_B 81272196000ULL //79127904000ULL

#define eeprom_magic_number 23
#define eeprom_start_address 512
//byte eeprom_magic_number EEMEM = 23;

#define SZTORC_PIN	A4		// PA4
#define PTT_PIN		A5		// PA5
#define NO_PIN		15		// PD7
#define ATT_PIN		14		// PD6 nóżka 20 (DIP40)
#define SSB_CW_PIN	A6		// PD5 było 13
#define FILTR_CW_PIN	1	// PB1 nóżka 2  (DIP40)
#define PAD_PIN		0		// PA0 nóżka 40 (DIP40) numeracja portów analogowych: 0...7 zamiast A0...A7; funkcja pomiar() do odczytu wartości !!!
#define UZAS_PIN	3		// PA3 nóżka 37 (DIP40) numeracja portów analogowych: 0...7 zamiast A0...A7; funkcja pomiar() do odczytu wartości !!!
#define BPF_A_PIN	18		// PC2 n24
#define BPF_B_PIN	19		// PC3 n25
#define BPF_C_PIN	20		// PC4 n26
#define BPF_D_PIN	21		// PC5 n27
#define STROBE_PIN	22		// PC6 n28
#define TONE_CW_PIN	13		// PD5
#define KLUCZOWANIE_CW	23	// PC7 n29
#define KRESKI_PIN	8		// PD0
#define KROPKI_PIN	9		// PD1

#if defined(USE_Si5351_P_LIB)
	#include <si5351_P.h>
	si5351 si;
#endif
#if defined(USE_Si5351Arduino_LIB)
	#include <si5351.h>
	Si5351 si5351;
#endif
const byte ile_pasm = 12;
struct config_t {
	//long if_frequency = 10694950;                   	// częstotliwość środkowa filtru kwarcowego u mnie
	long if_frequency = 9000000L;                   	// typowa częstotliwość środkowa filtru kwarcowego
	uint8_t status_pasma[ile_pasm] = {1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};		// 0 - wyłączone; 1 - włączone; 2 - włączone i domyślne (włącza się po załączeniu zasilania)
	int mode = 0;
	//unsigned int dit_time_ms = 80;
	unsigned int bk_time = 500;
	long si_rezonator = 25000000L;	// częstotliwość kwarcu w Si5351
	int IFoffset = 1600;			// odstęp BFO od częstotliwości środkowej filtru kwarcowego
	byte bfo_on = 1;
} configuration ;
//unsigned int last_dit_time_ms = configuration.dit_time_ms;
long frequency = 7015000;
long rit_frequency = 0;                        			//domyślna wartość poprawki
const long rit_range = 2000;                          	//zakres pracy RIT +/- podana wartość, domyślnie 2000Hz
const long rit_step = 20;                             	//krok działania RIT-a
const unsigned long update_time = 100;     	//interwał odświeżania s-metra w msec
const int CWoffset = 750;  								//w Hz
// const int mode = 0;                                   	//tryby pracy: 0-pośrednia, 1-generator, 2-lub wyżej, mnożnik razy 2 lub więcej - teraz jest w konfiguracji
unsigned long step = 100;
byte active_vfo = 0;                             		//0 - vfo A, 1 - vfo B
unsigned long VOX_time = 0;					// zmienna do odmierzania czasu od ostatniej kreski lub kropki na potrzeby VOX CW
// zmienne do pasm
typedef struct {
  long lower_limit;
  long upper_limit;
  long mid;
  char nazwa[5];
} band_t;

typedef struct {
  byte band;
  long freq[ile_pasm];
  byte mode[ile_pasm];
} vfo_t;

band_t bands[ile_pasm] = {
  {8000, 2500000, 1850000,     	"160m"},
  {2500000, 4500000, 3722000,     	" 80m"},
  {4500000, 9000000, 7150000,     	" 40m"},
  {9000000, 12000000, 10140000,  	" 30m"},
  {12000000, 16000000, 14200000,  	" 20m"},
  {16000000, 19000000, 18130000,  	" 17m"},
  {19000000, 23000000, 21200000,  	" 15m"},
  {23000000, 27000000, 24960000,  	" 12m"},
  {27000000, 40000000, 28500000,  	" 10m"},
  {40000000, 60000000, 50300000,  	"  6m"},
  {60000000, 120000000, 70100000,  	"  4m"},
  {120000000, 200000000, 144200000, "  2m"}
};
byte modes[ile_pasm]={2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3};
vfo_t vfo[2];
//byte tryb_pracy = 0;							// tryby pracy: 0 - CW, 1 - CW VOX, 2 - LSB, 3 - USB
byte rit_state = 0;                             //stan RIT-a: 0-rit off, 1-rit on
byte prev_LF = 53;

//byte debounce_time = 100;
char bufor[] = "              ";
Bounce krok = Bounce();
Bounce rit = Bounce();
Bounce split = Bounce();
Bounce tryb = Bounce();
Bounce sztorc = Bounce();
Bounce ptt = Bounce();
boolean key_mode = false;						// wskaźnik przejścia na pracę kluczem telegraficznym
boolean ptt_on = false;                        //stan przycisku PTT
boolean last_ptt_state = false;					// poprzedni stan PTT
boolean sztorc_on = false;						// stan wejścia sztorc
boolean last_sztorc_state = false;				// poprzedni stan sztorca
boolean split_on = false;						// wskaźnik przejścia na tryb pracy w splicie
boolean tun_on = false;							// wskaźnik strojenia
boolean att_on = false;							// wskaźnik włączenia tłumika
boolean filtr_CW_on = false;
//boolean tone_cw_on = false;
boolean konf_state = false;
boolean jest_opis_smetra = true;				// czy jest opis smetra
byte config_dirty = 0;
unsigned long s_metr_update_time = 0;          //zmienna pomocnicza do przechowywania czasu następnego uruchomienia s-metra
unsigned long swr_update_time = 0;
unsigned long uzas_update_time = 0;
unsigned long wpm_update_time = 0;
// Yack:
byte    wpm;            // Real wpm
byte	last_wpm;		// ostatnia wartość wpm
byte    volflags = 0;     // Temporary working flags (volatile)
word    wpmcnt;         // Speed
//inicjalizujemy enkoder
int8_t enc_delta;							// -128 ... 127
// funkcja sprawdzająca stan wejść od enkodera
void encode_read()
{
	static int8_t last;
	int8_t nowy;
	int8_t diff;
	nowy = 0;
	if (digitalRead(ENC_A) == LOW)
	nowy = 3;
	if (digitalRead(ENC_B) == LOW)
	nowy ^= 1;								// convert gray to binary
	diff = last - nowy;						// difference last - nowy
	if( diff & 1 )
	{							// bit 0 = value (1)
		last = nowy;							// store nowy as next last
		enc_delta += (diff & 2) - 1;		// bit 1 = direction (+/-)
	}
}
int8_t encode_read4( void )					// read four step encoders; funkcja dla enkodera kwadraturowego
{
	int8_t val;
	noInterrupts();
	val = enc_delta;
	enc_delta &= 3;
	interrupts();
	return val >> 2;
}
void linijka(int y, char zielone, char czerwone, char zielone_max, char czerwone_max)
{
	char kostka = 219;
	String linijka = "";
	tft.setCursor(60, y);
	for (int i = 0; i<zielone; i++)
	{
		linijka.concat(kostka);
	}
	for (int i = 0; i<zielone_max-zielone; i++)
	{
		linijka.concat(' ');
	}
	tft.setTextSize(2);
	tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
	tft.print(linijka);		//zielona plus czarne do S9
	linijka = "";
	for (int i = 0; i<czerwone; i++)
	{
		linijka.concat(kostka);
	}
	for (int i = 0; i<czerwone_max-czerwone; i++)
	{
		linijka.concat(' ');
	}
	tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
	tft.print(linijka);		//czerwone plus czarne reszta	
}

int read_settings_from_eeprom() 
{
  // returns 0 if eeprom had valid settings, returns 1 if eeprom needs initialized
    if (EEPROM.read(eeprom_start_address) == eeprom_magic_number)
	//if (eeprom_read_byte(&eeprom_magic_number))
	{

		byte* p = (byte*)(void*)&configuration;
		unsigned int i;
		int ee = eeprom_start_address + 1; // starting point of configuration struct
		for (i = 0; i < sizeof(configuration); i++)
		{
			*p++ = EEPROM.read(ee++);  
		}
     return 0;
    } 
	else 
	{
		return 1;
    }
	return 1;
}
void write_settings_to_eeprom(int initialize_eeprom) 
{   
	if (initialize_eeprom) 
	{
		EEPROM.write(eeprom_start_address, eeprom_magic_number);
	}
	const byte* p = (const byte*)(const void*)&configuration;
	unsigned int i;
	int ee = eeprom_start_address + 1;  // starting point of configuration struct
	for (i = 0; i < sizeof(configuration); i++)
	{
		EEPROM.write(ee++, *p++);  
	}
	config_dirty = 0;
}

void check_eeprom_for_initialization()
{
	// read settings from eeprom and initialize eeprom if it has never been written to
	if (read_settings_from_eeprom()) 
	{
		write_settings_to_eeprom(1);
	}
}

void check_for_dirty_configuration()
{
	if (config_dirty) 
	{
		write_settings_to_eeprom(0);
	}
}

unsigned char getKey()
{
	unsigned char kod_klawisza = 0;
	krok.update();
	if (krok.read() == LOW)
	{
		kod_klawisza |= (1<<3);
#if defined(DEBUG)
		Serial.print("kl po krok: ");
		Serial.println(kod_klawisza, BIN);
#endif
	}
	rit.update();
	if (rit.read() == LOW)
	{
		kod_klawisza |= (1<<2);
#if defined(DEBUG)
		Serial.print("kl po rit: ");
		Serial.println(kod_klawisza, BIN);
#endif
	}
	split.update();
	if (split.read() == LOW)
	{
		kod_klawisza |= (1<<1);
#if defined(DEBUG)
		Serial.print("kl po split: ");
		Serial.println(kod_klawisza, BIN);
#endif
	}
	tryb.update();
	if (tryb.read() == LOW)
	{
		kod_klawisza |= 1;		
#if defined(DEBUG)
		Serial.print("kl po mode: ");
		Serial.println(kod_klawisza, BIN);
#endif
	}
	return kod_klawisza & 0x0f;
}
void show_uzas()
{
	if(millis() >= uzas_update_time)
	{                              //sprawdzam czy już jest czas na wykonanie funkcji
		//PORTD ^= (1<<PD7);
		unsigned char uzas_view;
		int uzas_value = pomiar(UZAS_PIN);                          //odczyt wartości napięcia zasilania
		uzas_view = map(uzas_value, 0, 1023, 0, 150);				//przeskalowanie
		sprintf(bufor, "%2u.%1uV", uzas_view/10, uzas_view%10);
		tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
		tft.setTextSize(2);
		//tft.setCursor(259, 157);
		tft.setCursor(259, 164);
		//tft.print("U:");
		tft.print(bufor);
		//tft.print("V");
		uzas_update_time = millis() + update_time;       //ustawiam czas kolejnego wykonania tej funkcji
	}
}
void show_swr(){
	if(millis() >= swr_update_time)		//sprawdzam czy już jest czas na wykonanie funkcji
	{                              
		unsigned char swr_value;
		char zielone, czerwone;
		// linijka wskaźnika mocy
		// fala padająca
		int swr_pad_value = pomiar(PAD_PIN);                                 //odczyt wartości fali padającej
		unsigned char padajaca = map(swr_pad_value, 0, 992, 0, 16);
#if defined(DEBUG)
		Serial.print("pad: ");
		Serial.println(swr_pad_value);
		Serial.println(padajaca, HEX);
#endif
		tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
		tft.setTextSize(2);
		tft.setCursor(60, 124);
		tft.print("0      25     100");
		linijka(140, padajaca, 0, 16, 0);
		// fala odbita <-> SWR
		int swr_odb_value = pomiar(ODB_PIN);						//odczyt wartości fali odbitej
		if (swr_pad_value == 0)		// brak mocy
		{
			zielone = 0;
			czerwone = 0;
		}
		else
		{
			if (swr_odb_value <= swr_pad_value)
			{
				swr_value = map(swr_odb_value, 0, swr_pad_value, 0, 16);				//przeskalowanie względem fali padającej
			}
			else
			{
				swr_value = 16;
			}
			if (swr_value > 8)
			{
				zielone = 8;
				czerwone = swr_value - 8;
			}
			else
			{
				zielone = swr_value;
				czerwone = 0;
			}
		}
#if defined(DEBUG)
		Serial.print("odb: ");
		Serial.println(swr_odb_value);
		Serial.println(swr_value, HEX);
#endif
		tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
		tft.setCursor(60, 164);
		tft.print("1   2  3  5 9 oo");
		linijka(180, zielone, czerwone, 8, 8);
		jest_opis_smetra = false;
		swr_update_time = millis() + update_time;       //ustawiam czas kolejnego wykonania tej funkcji
	}
}
void show_opis_smetra()
{
	tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
	tft.setTextSize(2);
	// czyszczenie opisu i linijki SWR-metra
	tft.setCursor(60, 124);
	tft.print("                 ");
	tft.setCursor(60, 140);
	tft.print("                ");
	// opis S-metra
	tft.setCursor(60, 164);
	tft.print("1 3 5 7 9       ");
	tft.setTextSize(1);
	tft.setCursor(168, 170);
	tft.print("  20  40  60+");
}
void show_smeter(){
	if(millis() >= s_metr_update_time)
	{                              //sprawdzam czy już jest czas na wykonanie funkcji
		//PORTD ^= (1<<PD7);
		unsigned int smeter_val = pomiar(SMETR_PIN);
		char zielone = 0;  
		char czerwone = 0;
		//Serial.println(smeter_val);
		if(smeter_val < 2) // < S1
		{
			zielone = 0;
			czerwone = 0;
		}
		else if(smeter_val >= 2 && smeter_val < 7) // S1
		{   
			zielone = 1;
			czerwone = 0;
		}
		else if(smeter_val >= 7 && smeter_val < 12) // S2
		{   
			zielone = 2;
			czerwone = 0;
		}
		else if(smeter_val >= 12 && smeter_val < 17) // S3
		{   
			zielone = 3;
			czerwone = 0;
		}
		else if(smeter_val >= 17 && smeter_val < 22) // S4
		{   
			zielone = 4;
			czerwone = 0;
		}
		else if(smeter_val >= 22 && smeter_val < 26) // S5
		{  
			zielone = 5;
			czerwone = 0;
		}
		else if(smeter_val >=26  && smeter_val < 31) // S6
		{  
			zielone = 6;
			czerwone = 0;
		}
		else if(smeter_val >= 31 && smeter_val < 36) // S7
		{  
			zielone = 7;
			czerwone = 0;
		}
		else if(smeter_val >= 36 && smeter_val < 41) // S8
		{   
			zielone = 8;
			czerwone = 0;
		}
		else if(smeter_val >= 41 && smeter_val < 49) // S9
		{  
			zielone = 9;
			czerwone = 0;
		}
		else if(smeter_val >= 49 && smeter_val < 57) // +10dB
		{
			zielone = 9;
			czerwone = 1;
		}
		else if(smeter_val >= 57 && smeter_val < 65) // +20dB
		{  
			zielone = 9;
			czerwone = 2;
		}
		else if(smeter_val >= 65 && smeter_val < 73) // +30dB
		{  
			zielone = 9;
			czerwone = 3;
		}
		else if(smeter_val >= 73 && smeter_val < 81) // +40dB
		{  
			zielone = 9;
			czerwone = 4;
		}
		else if(smeter_val >= 81 && smeter_val < 89) // +50dB
		{  
			zielone = 9;
			czerwone = 5;
		}
		else if(smeter_val >= 89 && smeter_val < 97) // +60dB
		{  
			zielone = 9;
			czerwone = 6;
		}
		else if(smeter_val >= 97) // >65dB
		{  
			zielone = 9;
			czerwone = 7;
		}
		//Serial.print("S-meter: ");
		//Serial.println(smeter_val);
		if (!jest_opis_smetra)
		{
			show_opis_smetra();
			jest_opis_smetra = true;
		}
		linijka(180, zielone, czerwone, 9, 7);
		s_metr_update_time = millis() + update_time;       //ustawiam czas kolejnego wykonania tej funkcji
	}
}
void show_filtr_cw()
{
	tft.setTextSize(2);
	tft.setCursor(51, 215);
	if (filtr_CW_on)
	{
		tft.setTextColor(ILI9341_MAGENTA, ILI9341_BLACK);
	}
	else
	{
		tft.setTextColor(ILI9341_DARKGREY, ILI9341_BLACK);
	}
	tft.print("FCW");
}
void set_filtr_cw()
{
	if (filtr_CW_on)
	{
		digitalWrite(FILTR_CW_PIN, LOW);
		filtr_CW_on = false;
	}
	else
	{
		digitalWrite(FILTR_CW_PIN, HIGH);
		filtr_CW_on = true;
	}
	show_filtr_cw();
}
void set_mode()
{
	switch(vfo[active_vfo].mode[vfo[active_vfo].band])
	{
		case 0:		// było CW
			vfo[active_vfo].mode[vfo[active_vfo].band] = 2; // jest LSB
			digitalWrite(SSB_CW_PIN, LOW);
#if defined(USE_OE_MIX)
			Set_BFO_Frequency(configuration.if_frequency - configuration.IFoffset);
#endif
#if defined(USE_Si5351Arduino_LIB)
			si5351.set_freq((configuration.if_frequency - configuration.IFoffset)*SI5351_FREQ_MULT, SI5351_PLL_B, BFO);		// częstotliwość BFO dla LSB (wstęga się odwraca)
#endif
			break;
		case 2:		// było LSB
			vfo[active_vfo].mode[vfo[active_vfo].band] = 3;	// jest USB
			digitalWrite(SSB_CW_PIN, LOW);
#if defined(USE_OE_MIX)
			Set_BFO_Frequency(configuration.if_frequency + configuration.IFoffset);
#endif
#if defined(USE_Si5351Arduino_LIB)
			si5351.set_freq((configuration.if_frequency + configuration.IFoffset)*SI5351_FREQ_MULT, SI5351_PLL_B, BFO);		// częstotliwość BFO dla USB (wstęga się odwraca)
#endif
			break;
		case 3:		// było USB
			vfo[active_vfo].mode[vfo[active_vfo].band] = 0; // jest CW
			digitalWrite(SSB_CW_PIN, HIGH);
#if defined(USE_OE_MIX)
			Set_BFO_Frequency(configuration.if_frequency - configuration.IFoffset);
#endif
#if defined(USE_Si5351Arduino_LIB)
			si5351.set_freq((configuration.if_frequency - configuration.IFoffset)*SI5351_FREQ_MULT, SI5351_PLL_B, BFO); 		// częstotliwość BFO dla CW
#endif
			break;
	}
	//vfo[active_vfo].mode[vfo[active_vfo].band] = tryb_pracy;
	set_frequency(0);
	show_mode();
}
void show_mode()
{
	tft.setTextSize(2);
	tft.setCursor(5, 215);
	tft.setTextColor(ILI9341_MAGENTA, ILI9341_BLACK);
	switch(vfo[active_vfo].mode[vfo[active_vfo].band])
	{
		case 0:
			tft.print("   ");
			tft.setCursor(11, 215);
			tft.print("CW");
			break;
		case 2:
			tft.print("LSB");
			break;
		case 3:
			tft.print("USB");
			break;
	}
}
void set_att()
{
	if (att_on)
	{
		att_on = false;
		digitalWrite(ATT_PIN, LOW);
	}
	else
	{
		att_on = true;
		digitalWrite(ATT_PIN, HIGH);
	}
	show_att();
}
void show_att()
{
	if (att_on)
	{
		tft.setTextColor(ILI9341_MAGENTA, ILI9341_BLACK);
	}
	else
	{
		tft.setTextColor(ILI9341_DARKGREY, ILI9341_BLACK);
	}
	tft.setTextSize(2);
	tft.setCursor(143, 215);
	tft.print("ATT");
}
void set_rit()
{
	if (split_on)
	{
		if (active_vfo == 0)
		{
			vfo[1].band = vfo[0].band;
			vfo[1].freq[vfo[active_vfo].band] = frequency;
			show_frequency(1);
		}
		else
		{
			vfo[0].band = vfo[1].band;
			vfo[0].freq[vfo[active_vfo].band] = frequency;
			show_frequency(0);
		}
	}
	else
	{
		switch(rit_state)
		{                     //przełącznik trybu pracy z RIT
		case 1:                               //jesli tryb jest 1 (enkoder pracuje jako rit wartość RIT dodaję do częstotliwości)
			rit_state = 0;                       //ustaw tryb 0
			break;
		case 2:                                //jeśli tryb jest 2 (nieużywany) (enkoder pracuje jako enkoder wartość RIT dodaję do częstotliwości)
			rit_state = 0;                        //ustaw tryb 0
			break;
		case 0:                                //jeśli tryb jest 0 (enkoder pracuje jako enkoder wartość RIT wyłączony)
			rit_state = 1;                        //ustaw tryb 1
			break; 
		}
		show_rit();
		set_frequency(0);
		show_frequency(0);
   }
}
void show_rit()
{
	tft.setTextSize(2);
	if (rit_state == 0)
	{
		tft.drawRoundRect(246, 71, 72, 24, 3, ILI9341_BLACK);	// brak ramki dla odchylenia częstotliwości RIT
		tft.setCursor(252, 75);
		tft.print("     ");
		tft.setCursor(8, 122);
		tft.setTextColor(ILI9341_DARKGREY, ILI9341_BLACK);
	}
	else
	{
		tft.setCursor(8, 122);
		tft.setTextColor(ILI9341_MAGENTA, ILI9341_BLACK);
		tft.drawRoundRect(246, 71, 72, 24, 3, ILI9341_WHITE);	// ramka dla odchylenia częstotliwości RIT
	}
	tft.print("RIT");
}
void set_split()
{
	if (split_on)
	{
		split_on = false;
		if (active_vfo == 1) // przejście na VFO_B po wyłączeniu splitu
		{
			active_vfo = 0;
			change_vfo();
		}
		//digitalWrite(VFO_A_port, HIGH); // led zielony VFO A wyłączony
		//digitalWrite(VFO_B_port, HIGH); // led czerwony VFO B wyłączony
	}
	else
	{
		split_on = true;
		rit_state = 0;		// wyłączenie RITa podczas splitu
		if (active_vfo == 1) // na wszelki wypadek przełączenie na VFO_A
		{
			active_vfo = 0;
			change_vfo();
		}
		set_frequency(0);
		//digitalWrite(VFO_A_port, LOW); // led zielony VFO A włączony
		//digitalWrite(VFO_B_port, HIGH); // led czerwony VFO B wyłączony
	}
	show_split();
}
void show_split()
{
	tft.setTextSize(2);
	if (split_on)
	{
		show_rit();
		show_AB();
		tft.setCursor(8, 122);
		tft.print("A=B");
		tft.setTextColor(ILI9341_MAGENTA, ILI9341_BLACK);
		tft.setCursor(8, 169);
		tft.print("SPL");
	}
	else
	{
		tft.setTextColor(ILI9341_DARKGREY, ILI9341_BLACK);
		tft.setCursor(8, 169);
		tft.print("SPL");
		tft.setTextColor(ILI9341_MAGENTA, ILI9341_BLACK);
		tft.setCursor(8, 75);
		tft.print("STP");
		tft.setCursor(8, 122);
		tft.setTextColor(ILI9341_DARKGREY, ILI9341_BLACK);
		tft.print("RIT");
	}
}
void show_AB()
{
	tft.setTextSize(2);
	switch(active_vfo)
	{
		case 0:                                   //jeśli A to:
			tft.setTextColor(ILI9341_MAGENTA, ILI9341_BLACK);
			tft.setCursor(8, 75);
			tft.print("A");
			tft.setTextColor(ILI9341_DARKGREY, ILI9341_BLACK);
			tft.setCursor(20, 75);
			tft.print("/B");
			// digitalWrite(VFO_A_port, LOW); // led zielony VFO A włączony
			// digitalWrite(VFO_B_port, HIGH); // led czerwony VFO B wyłączony
		break;
		case 1:
			tft.setTextColor(ILI9341_DARKGREY, ILI9341_BLACK);
			tft.setCursor(8, 75);
			tft.print("A/");
			tft.setTextColor(ILI9341_MAGENTA, ILI9341_BLACK);
			tft.setCursor(32, 75);
			tft.print("B");
			// digitalWrite(VFO_A_port, HIGH); // led zielony VFO A wyłączony
			// digitalWrite(VFO_B_port, LOW); // led czerwony VFO B włączony
		break;
	}
}
//funkcja do obsługi zmiany VFO
void change_vfo(){
	show_AB();
	set_frequency(0);                            //ustawiam częstotliwość dla syntezera
	show_frequency(active_vfo);                            //pokazuję częstotliwość na LCD
	//delay(200);                                  //małe opóźnienie
}
void set_step()
{
	if (split_on)
	{
		// przełączanie VFO A/ VFO B
		if (active_vfo == 0)
		{
			active_vfo = 1;
		}
		else
		{
			active_vfo = 0;
		}
	   change_vfo();
	}
	else
	{
		if (step == ((konf_state) ? (STEP_MAX_KONF) : (STEP_MAX)))
		{
			step = ((konf_state) ? (STEP_MIN_KONF) : (STEP_MIN));
		}
		else
		{
			step = step * 10L;
		} 
		show_step();
	}
}
void show_step(){
  tft.setCursor(62, 75);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  if (step==1L){tft.print("  1");}
  if (step==10L){tft.print(" 10");}
  if (step==100L){tft.print("100");}
  if (step==1000L){tft.print(" 1k");}
  if (step==10000L){tft.print("10k");} 
  if (step==100000L){tft.print("0.1");} 
  if (step==1000000L){tft.print(" 1M");} 
}
//funkcja ustawiająca częstotliwość DDS-a
void set_frequency(int plus_or_minus)
{
	int correction = 0;                                                                //lokalna zmienna pomocnicza
	long frequency_to_dds = 0L;                     //zmienna pomocnicza przechowuje częstotliwość którą wysyłam do DDS-a
	frequency = vfo[active_vfo].freq[vfo[active_vfo].band];
	//tryb_pracy = vfo[active_vfo].mode[vfo[active_vfo].band];		// tryb_pracy <-> mode (CW, LSB, USB)
	if(rit_state == 2 || rit_state == 0)
	{                                            //jeśli nie obsługuję RIT-a to manipuluje częstotliwością
		frequency = frequency + plus_or_minus * step;                                          //częstotliwość = częstotliwość + krok
	}
	if(rit_state == 1)
	{                                                              //jesli obsługuję rita
		rit_frequency = rit_frequency + plus_or_minus * rit_step;                      //częstotliwość poprawki zwiększam o krok poprawki
		rit_frequency = constrain(rit_frequency, -rit_range, rit_range);                 //limitujemy poprawkę RIT do wartości z konfiguracji
	}
	if(rit_state != 0 && ptt_on == false)
	{                                           //jeśli jesteśmy w trybie włączonego RIT-a
		correction = rit_frequency;                                                    //lokalna zmienna pomocnicza przyjmuje wartość RIT by można to było dodać do czestotliwości
	}
	frequency = constrain(frequency, bands[vfo[active_vfo].band].lower_limit, bands[vfo[active_vfo].band].upper_limit);       //limitowanie zmiennej częstotliwości tej na wyświetlaczu
	if(configuration.mode == 0) //tryb pracy syntezy 0 - pośrednia
	{
		//a tutaj obliczam częstotliwość wynikową dla pracy w trybie pośredniej + ew.poprawka z RIT
		switch(vfo[active_vfo].mode[vfo[active_vfo].band])
		{
			case 0: // CW
				frequency_to_dds = frequency + configuration.if_frequency - configuration.IFoffset + CWoffset + correction;
				break;
			case 1: // CW VOX - tak jak CW
				frequency_to_dds = frequency + configuration.if_frequency - configuration.IFoffset + CWoffset + correction;
				break;
			case 2: // LSB
				frequency_to_dds = frequency + configuration.if_frequency - configuration.IFoffset + correction;
				break;
			case 3: // USB
				frequency_to_dds = frequency + configuration.if_frequency + configuration.IFoffset + correction;
				break;
		}
	}
	else
	{                                                                           //tryby pracy 1 - mnożnik * 1 generator lub 2 i więcej mnożnik
		frequency_to_dds = (frequency + correction) * configuration.mode;                            //mnożymy częstotliwość przez tryb pracy no i pamiętamy o poprawce
	}
#if defined(USE_OE_MIX)
	SetFrequency(frequency_to_dds);
#endif
	#if defined(USE_Si5351Arduino_LIB)
		si5351.set_freq(frequency_to_dds*SI5351_FREQ_MULT, SI5351_PLL_A, VFO);
	#endif
	#if defined(USE_Si5351_P_LIB)
		si.set_frequency(CLK0, frequency_to_dds*100ULL);
	#endif
	vfo[active_vfo].freq[vfo[active_vfo].band] = frequency;
	#if defined(DEBUG)
		Serial.print("frequency_to_dds");
		Serial.println(frequency_to_dds);                                                //debugowanie
	#endif
	// przełączanie pasm LF
	if ((vfo[active_vfo].band & B1111) == 0)
	{
#if defined(DEBUG)
	Serial.println("pasmo 0");
#endif
		if (frequency > 1000000 && frequency <= 2002000)
		{
#if defined(DEBUG)
	Serial.println("pasmo 00");
#endif
			if (prev_LF != 0)
			{
#if defined(DEBUG)
	Serial.println("wl pasma 00");
#endif
				steruj_4514(B00000000);
				prev_LF = 0;
			}
		}
		else if (frequency >= 400000 && frequency <= 1000000)
		{
#if defined(DEBUG)
	Serial.println("pasmo 01");
#endif
			if (prev_LF != 1)
			{
#if defined(DEBUG)
	Serial.println("wl pasma 01");
#endif
				steruj_4514(B00001100);
				prev_LF = 1;
			}
		}
		else if (frequency >= 100000 && frequency < 400000)
		{
#if defined(DEBUG)
	Serial.println("pasmo 02");
#endif
			if (prev_LF != 2)
			{
#if defined(DEBUG)
	Serial.println("wl pasma 02");
#endif
				steruj_4514(B00001101);
				prev_LF = 2;
			}
		}
		else if (frequency >= 55000 && frequency < 100000)
		{
#if defined(DEBUG)
	Serial.println("pasmo 03");
#endif
			if (prev_LF != 3)
			{
#if defined(DEBUG)
	Serial.println("wl pasma 03");
#endif
				steruj_4514(B00001110);
				prev_LF = 3;
			}
		}
		else if (frequency < 55000)
		{
#if defined(DEBUG)
	Serial.println("pasmo 04");
#endif
			if (prev_LF != 4)
			{
#if defined(DEBUG)
	Serial.println("wl pasma 04");
#endif
				steruj_4514(B00001111);
				prev_LF = 4;
			}
		}
	}
}
//funkcja do obsługi wyświetlania zmiany częstotliwości
void show_frequency(byte ktore_vfo){
	long f;
	if(rit_state != 1)
	{
		//sprintf(bufor, "%3lu.%03lu.%02lu", (frequency/1000000)%1000, (frequency/1000)%1000, (frequency%1000)/10);
		f = vfo[ktore_vfo].freq[vfo[ktore_vfo].band];
		sprintf(bufor, "%3lu.%03lu.%02lu", (f/1000000)%1000, (f/1000)%1000, (f%1000)/10);
		if (ktore_vfo == 0)
		{
			tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
			tft.setTextSize(5);
			tft.setCursor(10, 10);
		}
		else
		{
			tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
			tft.setTextSize(2);
			tft.setCursor(114, 75);
		}
		tft.print(bufor);		
	}
	if(rit_state == 1)
	{  
		//jeśli RIT jest włączony i enkoder pracuje jako RIT wyświetlamy zmiany częstotliwości RIT-a
		sprintf(bufor,"%4lu",abs(rit_frequency));     //przygotowujemy bufor z zawartością aktualnej wartości RIT
		tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
		tft.setTextSize(2);
		tft.setCursor(264, 75);
		tft.print(bufor);		
		tft.setCursor(252, 75);
		if(rit_frequency < 0)
		{
			tft.print("-");
		}else if(rit_frequency > 0)
		{
			tft.print("+");
		}                                        
		else
		{
			tft.setCursor(252, 75);
			tft.print("    0");
		}
	}
}
void show_down_up()
{
	tft.setTextColor(ILI9341_MAGENTA, ILI9341_BLACK);
	tft.drawRoundRect(184, 211, 45, 24, 3, ILI9341_WHITE);		// DWN
	tft.setCursor(189, 215);
	tft.print("DWN");
	tft.drawRoundRect(230, 211, 42, 24, 3, ILI9341_WHITE);		// UP
	tft.setCursor(241, 215);
	tft.print("UP");
}
void show_enter()
{
	tft.drawRoundRect(273, 211, 45, 24, 3, ILI9341_WHITE);		// ENTER
	tft.setTextColor(ILI9341_MAGENTA, ILI9341_BLACK);
	tft.setCursor(278, 215);
	tft.print("ENT");
}
void show_setup()
{
	tft.drawRoundRect(92, 211, 45, 24, 3, ILI9341_WHITE);		// SETUP
	tft.setCursor(97, 215);
	tft.print("SET");
}
void show_esc()
{
	tft.drawRoundRect(46, 211, 45, 24, 3, ILI9341_WHITE);		// Escape
	tft.setCursor(51, 215);
	tft.print("ESC");
}
void show_template(){
	tft.drawRoundRect(0, 0, 320, 60, 5, ILI9341_WHITE);	// ramka dla częstotliwości
	tft.drawRoundRect(0, 71, 48, 24, 3, ILI9341_WHITE);		// STP/A/B
	tft.drawRoundRect(54, 71, 48, 24, 3, ILI9341_WHITE);		// ramka dla wartości kroku
	tft.drawRoundRect(108, 71, 132, 24, 3, ILI9341_WHITE);	// ramka dla wartości VFO B(1)
	tft.drawRoundRect(0, 118, 48, 24, 3, ILI9341_WHITE);		// RIT/A=B
	tft.drawRoundRect(0, 165, 48, 24, 3, ILI9341_WHITE);		// ramka dla split (SPL)
	tft.drawRoundRect(0, 211, 45, 24, 3, ILI9341_WHITE);		// mode
	tft.drawRoundRect(46, 211, 45, 24, 3, ILI9341_WHITE);		// FCW filtr CW
	show_down_up();
	show_setup();
	tft.drawRoundRect(138, 211, 45, 24, 3, ILI9341_WHITE);		// ATT
	tft.drawRoundRect(273, 211, 45, 24, 3, ILI9341_WHITE);		// CMD
	tft.setTextColor(ILI9341_DARKGREY, ILI9341_BLACK);
	tft.setCursor(278, 215);
	tft.print("CMD");
	show_opis_smetra();
	show_split();
	show_rit();
	show_mode();
	show_att();
	show_filtr_cw();
	show_step();
}
void steruj_4514(byte pasmo)
{
	digitalWrite(STROBE_PIN, LOW);
	digitalWrite(BPF_A_PIN, pasmo & B1);
	digitalWrite(BPF_B_PIN, pasmo & B10);
	digitalWrite(BPF_C_PIN, pasmo & B100);
	digitalWrite(BPF_D_PIN, pasmo & B1000);
	digitalWrite(STROBE_PIN, HIGH);
	delay(50);
	digitalWrite(STROBE_PIN, LOW);
}
void switch_bands()
{
	prev_LF = 53;
	set_frequency(0);
	show_mode();
	switch(vfo[active_vfo].mode[vfo[active_vfo].band])
	{
		case 2:		// LSB
			digitalWrite(SSB_CW_PIN, LOW);
#if defined(USE_OE_MIX)
			Set_BFO_Frequency(configuration.if_frequency - configuration.IFoffset);
#endif
#if defined(USE_Si5351Arduino_LIB)
			si5351.set_freq((configuration.if_frequency - configuration.IFoffset)*SI5351_FREQ_MULT, SI5351_PLL_B, BFO);		// częstotliwość BFO dla LSB (wstęga się odwraca)
#endif
			break;
		case 3:		// USB
			digitalWrite(SSB_CW_PIN, LOW);
#if defined(USE_OE_MIX)
			Set_BFO_Frequency(configuration.if_frequency + configuration.IFoffset);
#endif
#if defined(USE_Si5351Arduino_LIB)
			si5351.set_freq((configuration.if_frequency + configuration.IFoffset)*SI5351_FREQ_MULT, SI5351_PLL_B, BFO);		// częstotliwość BFO dla USB (wstęga się odwraca)
#endif
			break;
		case 0:		// CW
			digitalWrite(SSB_CW_PIN, HIGH);
#if defined(USE_OE_MIX)
			Set_BFO_Frequency(configuration.if_frequency - configuration.IFoffset);
#endif
#if defined(USE_Si5351Arduino_LIB)
			si5351.set_freq((configuration.if_frequency - configuration.IFoffset)*SI5351_FREQ_MULT, SI5351_PLL_B, BFO); 		// częstotliwość BFO dla CW
#endif
			break;
	}
	unsigned char pasmo = vfo[active_vfo].band & B1111;
#if defined(DEBUG)
	Serial.print("pasmo: ");
	Serial.println(pasmo, BIN);
#endif
	if (pasmo != 0)
	{
		steruj_4514(pasmo);
	}
}
uint16_t pomiar(uint8_t kanal)
{
	ADMUX = (ADMUX & 0xF8) | kanal;
	// start
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC));
	return ADCW;
}
void show_NO()
{
	if (ptt_on)
	{
		tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
	}
	else
	{
		tft.setTextColor(ILI9341_BLACK, ILI9341_BLACK);
	}
	tft.setTextSize(5);
	tft.setCursor(280, 100);
	char kostka = 219;
	tft.print(kostka);

}
void setup() {
	check_eeprom_for_initialization();
	#if not defined(DEBUG) 
		yackinit();
	#endif
    //while(!Serial);                    // wait until serial ready
	#if defined(DEBUG)
		Serial.begin(1200);                // start serial  
		Serial.println("setup poczatek");
	#endif
	// ustawienie ADC
	// wybór napięcia odniesienia - VCC
	ADMUX |= (1<<REFS0);
	// włączenie modułu ADC oraz ustawienie preskalera na 64
	ADCSRA |= (1<<ADEN) | (1<<ADPS1) | (1<<ADPS2);
	
	// timer do przerwań enkodera
	FrequencyTimer2::setPeriod(250);
	FrequencyTimer2::setOnOverflow(encode_read);
	
#if defined(USE_OE_MIX)
	I2c.begin();
	si5351_clk_setup();
#endif
	#if defined(USE_Si5351_P_LIB)
		si.begin();
		si.set_xtal(25000000, -8);											// XTAL = 27.000 MHz with -9ppm correction
		si.pll_integer_config(PLL_A, 30);									// PLL_A = 30 * XTAL
		si.clk_config(CLK0, (SRC_PLL << CLKx_SRC) | (IDRV_8 << CLKx_IDRV)); // CLK0 powerd up, fractional mode, MultiSynth0, PLL A, not inverted, 8mA drive
		si.set_phase(CLK0, 0);												// no phase offset for CLK0
		si.clk_enable(CLK0);												// enable CLK0 output
		//si.clk_config(CLK2, (SRC_PLL << CLKx_SRC) | (IDRV_8 << CLKx_IDRV)); // CLK0 powerd up, fractional mode, MultiSynth0, PLL A, not inverted, 8mA drive
		//si.set_phase(CLK2, 0);												// no phase offset for CLK0
	#endif
	#if defined(USE_Si5351Arduino_LIB)
		si5351.set_correction(configuration.si_correction);
		// initialize the Si5351
		//si5351.init(SI5351_CRYSTAL_LOAD_8PF, 27000000); //If you're using a 27Mhz crystal, put in 27000000 instead of 0
		si5351.init(SI5351_CRYSTAL_LOAD_10PF, 0); //If you're using a 27Mhz crystal, put in 27000000 instead of 0
		// 0 is the default crystal frequency of 25Mhz.
		//si5351.set_pll(SI5351_PLL_A, SI5351_PLLA);	// 
		si5351.drive_strength(VFO, SI5351_DRIVE_4MA); 
		si5351.drive_strength(BFO, SI5351_DRIVE_4MA); 
		si5351.drive_strength(CWO, SI5351_DRIVE_4MA); 
		
		si5351.set_pll(SI5351_PLL_B, SI5351_PLLB);	//
		//si5351.set_pll(87682272000ULL, SI5351_PLLA);
		//si5351.set_pll(70578480000ULL, SI5351_PLLB);
		si5351.set_ms_source(VFO, SI5351_PLLA);
		si5351.set_ms_source(BFO, SI5351_PLLB);
		si5351.set_ms_source(CWO, SI5351_PLLB);
		
		// VFO
		si5351.set_freq(15670000000ULL, SI5351_PLL_A, VFO);
		//si5351.set_freq(1420800000ULL, SI5351_PLL_A, VFO);
		
		// BFO
		//si5351.set_freq(1069292000ULL, SI5351_PLL_B, BFO);	// niepotrzebne!!
		//si5351.output_enable(BFO, 1);
		//si5351.output_enable(CWO, 1);

		// CWO: generator dla CW TX
		si5351.set_freq((configuration.if_frequency - configuration.IFoffset + CWoffset)*SI5351_FREQ_MULT, SI5351_PLL_B, CWO);
		si5351.output_enable(CWO, 0);
		//si5351.set_freq(1069371000ULL, SI5351_PLL_A, CWO);
		//si5351.set_freq(351500000ULL, 0ULL, CWO);
	#endif
	delay(1000);
	tft.begin();
	tft.setRotation(3);
	tft.fillScreen(ILI9341_BLACK);
	tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
	tft.setTextSize(2);
	tft.setCursor(20, 46);
	tft.print("Trojak ");
	tft.println(WERSJA);
	byte interval = 10;
	krok.attach(KROK_PIN, INPUT_PULLUP);
	krok.interval(interval);
	rit.attach(RIT_PIN, INPUT_PULLUP);
	rit.interval(interval);
	split.attach(SPLIT_PIN, INPUT_PULLUP);
	split.interval(interval);
	tryb.attach(TRYB_PIN, INPUT_PULLUP);
	tryb.interval(interval);
	sztorc.attach(SZTORC_PIN, INPUT_PULLUP);
	ptt.attach(PTT_PIN, INPUT_PULLUP);
	
	pinMode(NO_PIN, OUTPUT);
	digitalWrite(NO_PIN, LOW);
	
	pinMode(ATT_PIN, OUTPUT);
	digitalWrite(ATT_PIN, LOW);

	pinMode(SSB_CW_PIN, OUTPUT);
	digitalWrite(SSB_CW_PIN, HIGH);
#if defined(USE_OE_MIX)
	Set_BFO_Frequency(configuration.if_frequency - configuration.IFoffset);
	simple_set_frequency(configuration.if_frequency - configuration.IFoffset + CWoffset);	// częstotliwość dla CWO
#endif
#if defined(USE_Si5351Arduino_LIB)
	si5351.set_freq((configuration.if_frequency - configuration.IFoffset)*SI5351_FREQ_MULT, SI5351_PLL_B, BFO);
#endif
	
	pinMode(FILTR_CW_PIN, OUTPUT);
	digitalWrite(FILTR_CW_PIN, LOW);	// przy starcie filtr CW wyłączony

	// S-metr na A2
	pinMode(A2, INPUT);
	
	analogWrite(TONE_CW_PIN, 0);	// PWM około 500Hz
	
	pinMode(KLUCZOWANIE_CW, OUTPUT);
	digitalWrite(KLUCZOWANIE_CW, LOW);
	
	pinMode(BPF_A_PIN, OUTPUT);
	pinMode(BPF_B_PIN, OUTPUT);
	pinMode(BPF_C_PIN, OUTPUT);
	pinMode(BPF_D_PIN, OUTPUT);
	pinMode(STROBE_PIN, OUTPUT);
	
	delay(1000); 
	tft.fillScreen(ILI9341_BLACK);
	pinMode(ENC_A, INPUT_PULLUP);
	pinMode(ENC_B, INPUT_PULLUP);
	char idStartVFO = -1;
	//byte idPaktywne = -1;
	for(byte i=0; i < ile_pasm; i++)
	{
		//Serial.print(bands[i].nazwa);
		//Serial.print(bands[i].mid);
		//Serial.println(configuration.status_pasma[i]);
		if (configuration.status_pasma[i] > 0)
		{
			if (idStartVFO == -1)
			{			
				idStartVFO = i;
			}
			if (configuration.status_pasma[i] == 2) idStartVFO = i;
			vfo[0].freq[i] = bands[i].mid;
			vfo[1].freq[i] = bands[i].mid;
			vfo[0].mode[i] = modes[i];
			vfo[1].mode[i] = modes[i];
		}
	}
	//Serial.print("idStartVFO: ");
	//Serial.println(idStartVFO);
	vfo[0].band = (idStartVFO >= 0) ? idStartVFO : 1;  //początkowe ustawienia pasma po starcie systemu
	vfo[1].band = vfo[0].band;
	set_frequency(0);                       	//odpalamy syntezer i ustawiamy częstotliwość startową
	show_frequency(0);                       	//pokazujemy częstotliwość na lcd
	show_frequency(1);
	frequency = vfo[active_vfo].freq[vfo[active_vfo].band];
	long frequency_to_dds;
	if (configuration.mode == 0)
	{
		frequency_to_dds = frequency + configuration.if_frequency - configuration.IFoffset + CWoffset;
	}
	else
	{
		frequency_to_dds = frequency*configuration.mode;
	}
#if defined(USE_OE_MIX)
	SetFrequency(frequency_to_dds);
#endif
	#if defined(USE_Si5351_P_LIB)
		si.set_frequency(CLK0, frequency_to_dds*100ULL);
	#endif
	#if defined(USE_Si5351Arduino_LIB)
		si5351.set_freq(frequency_to_dds*SI5351_FREQ_MULT, SI5351_PLL_B, VFO);
	#endif
	show_template();                        //pokazujemy domyślne stałe elementy LCD
	show_step();	// ??
	switch_bands();
    // LCD Begin	
	LCDML_DISP_groupEnable(_LCDML_G1); // enable group 1
    LCDML_setup(_LCDML_BACK_cnt);
		
	// Read the Status Register
	#if defined(DEBUG) && defined(USE_Si5351Arduino_LIB)
		si5351.update_status();
		Serial.print("SYS_INIT: ");
		Serial.print(si5351.dev_status.SYS_INIT);
		Serial.print("  LOL_A: ");
		Serial.print(si5351.dev_status.LOL_A);
		Serial.print("  LOL_B: ");
		Serial.print(si5351.dev_status.LOL_B);
		Serial.print("  LOS: ");
		Serial.print(si5351.dev_status.LOS);
		Serial.print("  REVID: ");
		Serial.println(si5351.dev_status.REVID);
		Serial.println("setup koniec");
	#endif
#if defined(DEBEK)
		tft.fillScreen(ILI9341_BLACK);
		tft.setCursor(0,0);
		tft.setTextSize(1);
		tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
#endif

}
void loop() 
{
	#if defined(CZAS_PETLI)
		PORTD ^= (1<<PD7);
	#endif
	#if not defined(DEBUG)
		if (!konf_state)
		{
			yack_loop();
		}
	#endif
	unsigned char kod_klawisza;
	if (konf_state)
	{
	}
	else
	{
		ptt.update();
		if (ptt.read() == LOW)
		{
			ptt_on = true;
		}
		else
		{
			if (millis() - VOX_time > configuration.bk_time)	// wyłączenie ptt  - przejście na odbiór po upływie ...
			{
				ptt_on = false;
				key_mode = false;
			}
		}
		if(last_ptt_state != ptt_on)	//sprawdzam czy zmienił się stan PTT jeśli tak to robię update na LCD
		{            
			if (ptt_on == true)
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
			}
			else
			{
				digitalWrite(NO_PIN, LOW);
				if (split_on)
				{
					active_vfo = 0;
					change_vfo();
				}
			}
			show_NO();
			set_frequency(0);
			last_ptt_state = ptt_on;               //uaktualniam poprzedni stan PTT
		}
		sztorc.update();
		if (sztorc.read() == LOW)
		{
			sztorc_on = true;
    		VOX_time = millis();
		}
		else
		{
			sztorc_on = false;
		}
		if(last_sztorc_state != sztorc_on)
		{            //sprawdzam czy zmienił się stan sztorc
			if (sztorc_on)
			{
				#if defined(USE_Si5351_P_LIB)
					si.clk_enable(CLK2);
				#endif
				#if defined(USE_Si5351Arduino_LIB)
					si5351.output_enable(CWO, 1);
				#endif
			    key(DOWN);
			}
			else
			{
				#if defined(USE_Si5351_P_LIB)
					si.clk_disable(CLK2);
				#endif
				#if defined(USE_Si5351Arduino_LIB)
					si5351.output_enable(CWO, 0);
				#endif
#if defined(USE_OE_MIX)
				clk_disable(CWO);
#endif
				key(UP);
				/*
				analogWrite(TONE_CW_PIN, 0);
				digitalWrite(KLUCZOWANIE_CW, LOW);
				*/
			}
			last_sztorc_state = sztorc_on;
		}
		kod_klawisza = getKey();
		if (kod_klawisza)
		{
			delay(100);
			if (kod_klawisza == getKey())
			//if(true)
			{
				//kod_klawisza = getKey();	//TODO co to jest???
				#if defined(DEBUG)
					Serial.print("kod klawisza: ");
					Serial.println(kod_klawisza);
				#endif
				switch(kod_klawisza)
				{
					case MODE_KEY:		// MODE
						set_mode();
						break;
					case SPLIT_KEY:		// SPL
						set_split();
						break;
					case RIT_KEY:		// RIT//A/B
						set_rit();
						break;
					case UP_KEY:	// UP
						do
						{
							if (vfo[active_vfo].band == ile_pasm - 1)
							{
								vfo[active_vfo].band = 0; 
							}
							else
							{
								vfo[active_vfo].band++; 
							}						
						} while (configuration.status_pasma[vfo[active_vfo].band] == 0);
						rit_state = 0;		// wyłączenie RITa przy zmianie pasma
						switch_bands();
						show_frequency(active_vfo);
						break;
					case DOWN_KEY:		// DOWN
#if defined(YACK_MEMO)
						yackmessage(PLAY, 3);
#else
						do
						{
							if (vfo[active_vfo].band == 0)
							{
								vfo[active_vfo].band = ile_pasm - 1; 
							}
							else
							{
								vfo[active_vfo].band--;
							}
						} while (configuration.status_pasma[vfo[active_vfo].band] == 0);
						rit_state = 0;		// wyłączenie RITa przy zmianie pasma
						#if defined(USE_Si5351Arduino_LIB)
							si5351.set_freq(1000000000ULL, SI5351_PLL_A, VFO);	// proteza, żeby generator zawsze startował
						#endif
						switch_bands();
						show_frequency(active_vfo);
#endif
						break;
					case KROK_KEY:			// STP/A/B
						set_step();
						break;
					case ATT_KEY:		// ATT
#if defined(YACK_MEMO)
						yackmessage(PLAY, 2);
#else
						set_att();
#endif
						break;
					case SETUP_KEY:		// SETUP
						konf_state = true;
						LCDML.Button_enter();
						LCDML_DISP_update_menu();
						//Serial.println("konf_state=true");
						break;
					case FILTR_CW_KEY:		// Filtr CW
#if defined(YACK_MEMO)
						yackmessage(PLAY, 1);
#else
						set_filtr_cw();
#endif
						break;
					case M1_KEY:
						break;
					case M2_KEY:
						yackmessage(PLAY, 1);
						break;
					case M3_KEY:
						yackmessage(PLAY, 2);
						break;
					case M4_KEY:
						yackmessage(PLAY, 3);
						break;
					default:
						break;
				}
			}
		}
		//obsługa enkodera
		int enc = encode_read4();
		if (enc)
		{
			#if defined(DEBUG)
				Serial.print("enc=");
				Serial.println(enc);
			#endif
			if (key_mode)
			{
				//configuration.dit_time_ms = configuration.dit_time_ms - enc;
				if (enc > 0 && millis() > wpm_update_time)
				{
					wpm++;
					wpm_update_time = millis() + 200;
				}
				else
				{
					wpm--;
					wpm_update_time = millis() + 200;
				}
				wpmcnt=(1200/YACKBEAT)/wpm; // Calculate beats
				key_mode = false;
			}
			else
			{
				if (ptt_on)
				{
					//set_moc(enc);
				}
				else
				{
					set_frequency(enc);
					show_frequency(active_vfo);
				}
			}
		}
		if (!konf_state)
		{
			if (ptt_on)
			{
				if (!key_mode || sztorc_on)
				{
					show_swr();
				}
			}
			else
			{
#if not defined(DEBEK)
				if (!key_mode)
				{
					show_smeter();
				}
#endif
				/*
				if (last_dit_time_ms != configuration.dit_time_ms)
				{
					config_dirty = true;
					last_dit_time_ms = configuration.dit_time_ms;
				}
				*/
				if (last_wpm != wpm)
				{
				    volflags |= DIRTYFLAG; // Set the dirty flag
				    last_wpm = wpm;
				}
			}
			if (!key_mode)
			{
#if not defined(DEBEK)
				show_uzas();
#endif
			}
		}
	}
    check_for_dirty_configuration();	
	//LCDMenu
    // this function must called here, do not delete it
    LCDML_run(_LCDML_priority);
}	// end loop()
// *********************************************************************
// check some errors - do not change here anything
// *********************************************************************
# if(_LCDML_DISP_rows > _LCDML_DISP_cfg_max_rows)
# error change value of _LCDML_DISP_cfg_max_rows in LCDMenuLib.h
# endif
# if(_LCDML_DISP_cols > _LCDML_DISP_cfg_max_string_length)
# error change value of _LCDML_DISP_cfg_max_string_length in LCDMenuLib.h
# endif
