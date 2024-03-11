/*
 * Wire 2300 bajtów 6ms
 * I2C 1900 bajtów	5,5ms
 * po zastosowaniu i2c.write dla bulk: 2,5ms
 *
 *
 */
#include <I2C.h>

//#define CZAS_PETLI
//#define DEBUG

#define CLK_EN 			0x03
#define PLL_A			26
#define PLL_B			34
#define CLK0			0
#define CLK1			1
#define CLK2			2
#define VFO				0
#define BFO				1
#define CWO				2
#define MSx_INT			6
#define CLK0_CFG		0x10
#define MS_0			42
const uint8_t Si5351_address =  0x60;
uint8_t _clk_src;
//uint32_t _xtal = 25025025;
uint32_t c = 1048574;                   // "c" part of Feedback-Multiplier from XTAL to PLL
static unsigned long fvco_B;                  // VCO frequency (600-900 MHz) of PLL

void oe_setup()
{
#if defined(DEBUG)
	Serial.begin(1200);
#endif
	I2c.begin();
	si5351_clk_setup();
	SetFrequency (200000000);             // Set VFO Frequency
	Set_BFO_Frequency(8998500);
	simple_set_frequency(9001500);		// ustawienie CWO
#if defined(CZAS_PETLI)
	pinMode(15, OUTPUT);		// PD7 n21
#endif
}

void oe_loop()
{
	long i = 0;
	long f = 3000000;
	do
	{
#if defined(CZAS_PETLI)
	PORTD ^= (1<<PD7);	// goła pętla 0,4us
	f = f + 1000L;
	//SetFrequency (f);             // VFO
	Set_BFO_Frequency(f);
	delay(2);
#endif
#if defined(DEBUG)
	byte status = read_register(0);
	if (((status >> 6) & 0x01) != 0)	// LOL_A lost lock PLL_A (5); (6) - LOL_B - PLL_B
	{
		Serial.print("LOL_B: ");
		Serial.println(status, BIN);
		delay(1000);
	}
#endif

	}
	while  (f < 30000000UL);
}

void SetFrequency (unsigned long frequency) { // Frequency in Hz; must be within [7810 Hz to 200 Mhz]
  unsigned long fvco;                  // VCO frequency (600-900 MHz) of PLL
  unsigned long outdivider;            // Output divider in range [4,6,8-900], even numbers preferred
  byte R = 1;                          // Additional Output Divider in range [1,2,4,...128]
  byte a;                              // "a" part of Feedback-Multiplier from XTAL to PLL in range [15,90]
  unsigned long b;                     // "b" part of Feedback-Multiplier from XTAL to PLL
  float f;                             // floating variable, needed in calculation
  unsigned long MS0_P1;                // Si5351a Output Divider register MS0_P1, P2 and P3 are hardcoded below
  unsigned long MSNA_P1;               // Si5351a Feedback Multisynth register MSNA_P1
  unsigned long MSNA_P2;               // Si5351a Feedback Multisynth register MSNA_P2
  unsigned long MSNA_P3;               // Si5351a Feedback Multisynth register MSNA_P3

  outdivider = 900000000 / frequency;  // With 900 MHz beeing the maximum internal PLL-Frequency
  
  while (outdivider > 900){            // If output divider out of range (>900) use additional Output divider
    R = R * 2;
    outdivider = outdivider / 2;
  }
  if (outdivider % 2) outdivider--;    // finds the even divider which delivers the intended Frequency

  fvco = outdivider * R * frequency;   // Calculate the PLL-Frequency (given the even divider)

  switch (R){                          // Convert the Output Divider to the bit-setting required in register 44
    case 1: R = 0; break;              // Bits [6:4] = 000
    case 2: R = 16; break;             // Bits [6:4] = 001
    case 4: R = 32; break;             // Bits [6:4] = 010
    case 8: R = 48; break;             // Bits [6:4] = 011
    case 16: R = 64; break;            // Bits [6:4] = 100
    case 32: R = 80; break;            // Bits [6:4] = 101
    case 64: R = 96; break;            // Bits [6:4] = 110
    case 128: R = 112; break;          // Bits [6:4] = 111
  }

  a = fvco / configuration.si_rezonator;                   // Multiplier to get from Quartz-Oscillator Freq. to PLL-Freq.
  f = fvco - a * configuration.si_rezonator;               // Multiplier = a+b/c
  f = f * c;                           // this is just "int" and "float" mathematics
  f = f / configuration.si_rezonator;
  b = f;

  MS0_P1 = 128 * outdivider - 512;     // Calculation of Output Divider registers MS0_P1 to MS0_P3
                                       // MS0_P2 = 0 and MS0_P3 = 1; these values are hardcoded, see below

  f = 128 * b / c;                     // Calculation of Feedback Multisynth registers MSNA_P1 to MSNA_P3
  MSNA_P1 = 128 * a + f - 512;
  MSNA_P2 = f;
  MSNA_P2 = 128 * b - MSNA_P2 * c; 
  MSNA_P3 = c;

  //TODO poprawić - ustawianie tylko 7 bitu;
  //write_register(16, 128);                      // Disable output during the following register settings
  //TODO czy to jest potrzebne? chyba nikt tego nie stosuje

  uint8_t dane[] = {(MSNA_P3 & 65280) >> 8, MSNA_P3 & 255, (MSNA_P1 & 196608) >> 16, (MSNA_P1 & 65280) >> 8, MSNA_P1 & 255, ((MSNA_P3 & 983040) >> 12) | ((MSNA_P2 & 983040) >> 16), (MSNA_P2 & 65280) >> 8, MSNA_P2 & 255};
  write_block(26, dane, 8);

  dane[0] = 0;
  dane[1] = 1;
  dane[2] = ((MS0_P1 & 196608) >> 16) | R;
  dane[3] = (MS0_P1 & 65280) >> 8;
  dane[4] = MS0_P1 & 255;
  dane[5] = 0;
  dane[6] = 0;
  dane[7] = 0;
  write_block(42, dane, 8);
  if (outdivider == 4)
  {
    write_register(44, 12 | R);                 // Special settings for R = 4 (see datasheet)
    write_register(45, 0);                      // Bits [15:8]  of MS0_P1 must be 0
    write_register(46, 0);                      // Bits [7:0]  of MS0_P1 must be 0
  }
  //TODO czy to potrzebne? - jeśli tak - robić restart tylko przy zmianie pasma - dodatkowy parametr funkcji
  // write_register(177, 32);                      // This resets PLL A
  //TODO włączenie zasilania dla clk (16 rejestr) niepotrzebne?
}
void Set_BFO_Frequency (unsigned long frequency) {
	// BFO pracuje na PLL_B
	// Frequency in Hz; must be within [7810 Hz to 200 Mhz]
  unsigned long fvco;                  // VCO frequency (600-900 MHz) of PLL
  unsigned long outdivider;            // Output divider in range [4,6,8-900], even numbers preferred
  byte R = 1;                          // Additional Output Divider in range [1,2,4,...128]
  byte a;                              // "a" part of Feedback-Multiplier from XTAL to PLL in range [15,90]
  unsigned long b;                     // "b" part of Feedback-Multiplier from XTAL to PLL
  float f;                             // floating variable, needed in calculation
  unsigned long MS1_P1;                // Si5351a Output Divider register MS1_P1, P2 and P3 are hardcoded below
  unsigned long MSNB_P1;               // Si5351a Feedback Multisynth register MSNB_P1
  unsigned long MSNB_P2;               // Si5351a Feedback Multisynth register MSNB_P2
  unsigned long MSNB_P3;               // Si5351a Feedback Multisynth register MSNB_P3

  outdivider = 900000000 / frequency;  // With 900 MHz beeing the maximum internal PLL-Frequency

  while (outdivider > 900){            // If output divider out of range (>900) use additional Output divider
    R = R * 2;
    outdivider = outdivider / 2;
  }
  if (outdivider % 2) outdivider--;    // finds the even divider which delivers the intended Frequency

  fvco = outdivider * R * frequency;   // Calculate the PLL-Frequency (given the even divider)
  fvco_B = fvco;
  switch (R){                          // Convert the Output Divider to the bit-setting required in register 44
    case 1: R = 0; break;              // Bits [6:4] = 000
    case 2: R = 16; break;             // Bits [6:4] = 001
    case 4: R = 32; break;             // Bits [6:4] = 010
    case 8: R = 48; break;             // Bits [6:4] = 011
    case 16: R = 64; break;            // Bits [6:4] = 100
    case 32: R = 80; break;            // Bits [6:4] = 101
    case 64: R = 96; break;            // Bits [6:4] = 110
    case 128: R = 112; break;          // Bits [6:4] = 111
  }

  a = fvco / configuration.si_rezonator;                   // Multiplier to get from Quartz-Oscillator Freq. to PLL-Freq.
  f = fvco - a * configuration.si_rezonator;               // Multiplier = a+b/c
  f = f * c;                           // this is just "int" and "float" mathematics
  f = f / configuration.si_rezonator;
  b = f;

  MS1_P1 = 128 * outdivider - 512;     // Calculation of Output Divider registers MS1_P1 to MS0_P3
                                       // MS0_P2 = 0 and MS0_P3 = 1; these values are hardcoded, see below

  f = 128 * b / c;                     // Calculation of Feedback Multisynth registers MSNB_P1 to MSNB_P3
  MSNB_P1 = 128 * a + f - 512;
  MSNB_P2 = f;
  MSNB_P2 = 128 * b - MSNB_P2 * c;
  MSNB_P3 = c;

  //TODO poprawić - ustawianie tylko 7 bitu;
  //write_register(16, 128);                      // Disable output during the following register settings
  //TODO czy to jest potrzebne? chyba nikt tego nie stosuje

  uint8_t dane[] = {(MSNB_P3 & 65280) >> 8, MSNB_P3 & 255, (MSNB_P1 & 196608) >> 16, (MSNB_P1 & 65280) >> 8, MSNB_P1 & 255, ((MSNB_P3 & 983040) >> 12) | ((MSNB_P2 & 983040) >> 16), (MSNB_P2 & 65280) >> 8, MSNB_P2 & 255};
  write_block(34, dane, 8);

  dane[0] = 0;
  dane[1] = 1;
  dane[2] = ((MS1_P1 & 196608) >> 16) | R;
  dane[3] = (MS1_P1 & 65280) >> 8;
  dane[4] = MS1_P1 & 255;
  dane[5] = 0;
  dane[6] = 0;
  dane[7] = 0;
  write_block(50, dane, 8);
  if (outdivider == 4)
  {
    write_register(44+8, 12 | R);                 // Special settings for R = 4 (see datasheet)
    write_register(45+8, 0);                      // Bits [15:8]  of MS1_P1 must be 0
    write_register(46+8, 0);                      // Bits [7:0]  of MS1_P1 must be 0
  }
  //TODO czy to potrzebne? - jeśli tak - robić restart tylko przy zmianie pasma - dodatkowy parametr funkcji
  write_register(177, 128);                      // This resets PLL B
  //TODO włączenie zasilania dla clk (16 rejestr) niepotrzebne?
}
uint8_t read_register (uint8_t regist)
{
	const uint8_t ile = 1;
	I2c.read(Si5351_address, regist, ile);
	return I2c.receive();
}
void write_register (byte regist, byte value)
{
	I2c.write(Si5351_address, regist, value);
}
void si5351_clk_setup()
{
	//I2c.write(Si5351_address,0x09, 0xFF);		// disable OEB pin ? nie ma takiego
	//write_register(0x0F, 0x00);		// XTAL as input for both PLLs - reset value?
	//write_register(0xB7, 0xC0);		// 10pF	XTAL load capacitance  błąd: pzostałe bity [5:0] muszą być: 010010
	//write_register(0xB7, B11010010);		// 10pF (default)
	write_register(0x03, 0xFC);				// CLK0 i CLK1 (VFO I BFO) enable; CLK2 (CWO) disable
	write_register(16, B01001101);			// CLK0: powered up, integer mode, PLL_A, not inverted, Multisynth, 4mA
	write_register(17, B01101101);			// CLK1: powered up, integer mode, PLL_B, not inverted, Multisynth, 4mA
	write_register(18, B01101101);			// CLK2: powered up, integer mode, PLL_B, not inverted, Multisynth, 4mA
	if (configuration.bfo_on & 0x01)
	{
		if (configuration.mode == 0)
		{
			clk_enable(BFO);
		}
		else
		{
			clk_disable(BFO);
		}
	}
	else
	{
		clk_disable(BFO);
	}
}
/**************************************************************************/
/*!
    @brief  Disable output

    @param[in]	clk
    			Number of the configured channel. Accepts CLK0, CLK1 or CLK2
*/
/**************************************************************************/
void clk_disable(uint8_t clk)
{
	write_register(CLK_EN, read_register(CLK_EN) | (1 << clk));
}

/**************************************************************************/
/*!
    @brief  Enable output

    @param[in]	clk
    			Number of the configured channel. Accepts CLK0, CLK1 or CLK2
*/
/**************************************************************************/
void clk_enable(uint8_t clk)
{
	write_register(CLK_EN, read_register(CLK_EN) & ~(1 << clk));
}
/**************************************************************************/
/*!
    @brief  Writes a continuous block of registers

    @param[in]	address
    			Address of the first register to write.
    @param[in]	value
    			Pointer to the data block to transfer.
    @param[in]	len
    			Number of registers to write
*/
/**************************************************************************/
void write_block(uint8_t address, uint8_t* value, uint8_t len)
{
	char result = I2c.write(Si5351_address, address, value, len);
#if defined(DEBUG)
	if (result != 0)
	{
		Serial.print("write_block:blad_trans: ");
		Serial.println(result, HEX);
	}
#endif
}

void simple_set_frequency(uint32_t freq)
{
	// ustawienie generatora CWO
	uint8_t clk = CWO;
	uint64_t vco;
	uint32_t a, b, c, p1, p2;
	uint8_t config[8];
	vco = fvco_B;
#if defined(DEBUG)
	Serial.print("fvco_B: ");
	Serial.println(fvco_B);
#endif
	vco = vco * 0x00100000;
	c = 0x000FFFFE;
	b = vco / freq;
	a = (b - (b & 0x000FFFFF)) >> 20;
	b = b & 0x000FFFFF;
#if defined(DEBUG)
	Serial.print("c: ");
	Serial.println(c);
	Serial.print("b: ");
	Serial.println(b);
	Serial.print("a: ");
	Serial.println(a);
#endif
	if(b == 0)
	{
		write_register(CLK0_CFG + clk, read_register(CLK0_CFG + clk) | (1<<MSx_INT));		// MS_0 a nie CLK0_CFG?
		c = 1;
		p1 = 128*a - 512;
		p2 = 0;
#if defined(DEBUG)
		Serial.println("integer mode");
#endif
	}
	else
	{
		write_register(CLK0_CFG + clk, read_register(CLK0_CFG + clk) & ~(1<<MSx_INT));		// MS_0 a nie CLK0_CFG?
#if defined(DEBUG)
		Serial.println(read_register(CLK0_CFG + clk), BIN);
#endif
		p1 = 128*a + (128 * b)/c - 512;
#if defined(DEBUG)
		Serial.print("p1: ");
		Serial.println(p1);
#endif
		p2 = 128*b - c*((128 * b)/c);
#if defined(DEBUG)
		Serial.print("p2: ");
		Serial.println(p2);
#endif
	}
	config[0] = (c & 0x0000FF00) >> 8;
	config[1] = (c & 0x000000FF);
	config[2] = (p1 & 0x00030000) >> 16;
	config[3] = (p1 & 0x0000FF00) >> 8;
	config[4] = (p1 & 0x000000FF);
	config[5] = ((p2 & 0x000F0000) >> 16) | (c & 0x000F0000) >> 12;
	config[6] = (p2 & 0x0000FF00) >> 8;
	config[7] = (p2 & 0x000000FF);

	write_block(MS_0 + clk*8, config, 8);
}
