// =====================================================================
//
// CONTROL
//
// =====================================================================
// *********************************************************************
// *********************************************************************
// content:
// (0) Control over serial interface
// (1) Control over one analog input 
// (2) Control over 4 - 6 digital input pins (internal pullups enabled)
// (3) Control over encoder (internal pullups enabled)
// (4) Control with Keypad
// *********************************************************************

//#define _LCDML_CONTROL_cfg      0  // serial

//#define _LCDML_CONTROL_cfg    1  // analog
//#define _LCDML_CONTROL_cfg    2  // digital
//#define _LCDML_CONTROL_cfg    3  // encoder
//#define _LCDML_CONTROL_cfg    4  // keypad
#define _LCDML_CONTROL_cfg    5  // moja klawiatura

// therory:
// "#if" is a preprocessor directive and no error, look here:
// (english) https://en.wikipedia.org/wiki/C_preprocessor
// (german)  https://de.wikipedia.org/wiki/C-Pr%C3%A4prozessor


// *********************************************************************
// CONTROL TASK, DO NOT CHANGE
// *********************************************************************
void LCDML_BACK_setup(LCDML_BACKEND_control)      
// *********************************************************************
{
  // call setup   
  LCDML_CONTROL_setup();      
}
// backend loop
boolean LCDML_BACK_loop(LCDML_BACKEND_control)
{    
  // call loop
  LCDML_CONTROL_loop();

  // go to next backend function and do not block it
  return true;  
}
// backend stop stable
void LCDML_BACK_stable(LCDML_BACKEND_control)
{
}




// *********************************************************************
// *************** (0) CONTROL OVER SERIAL INTERFACE *******************
// *********************************************************************
#if(_LCDML_CONTROL_cfg == 0)
// settings
  # define _LCDML_CONTROL_serial_enter           'e'
  # define _LCDML_CONTROL_serial_up              'w'
  # define _LCDML_CONTROL_serial_down            's'
  # define _LCDML_CONTROL_serial_left            'a'
  # define _LCDML_CONTROL_serial_right           'd'
  # define _LCDML_CONTROL_serial_quit            'q'
// *********************************************************************
// setup
void LCDML_CONTROL_setup()
{
}
// *********************************************************************
// loop
void LCDML_CONTROL_loop()
{
  // check if new serial input is available  
  if (Serial.available()) {
    // read one char from input buffer   
    switch (Serial.read())
    {
      case _LCDML_CONTROL_serial_enter:  
		Serial.println("enter");
		LCDML_BUTTON_enter(); 
		break;
      case _LCDML_CONTROL_serial_up:     
		Serial.println("up");
		LCDML_BUTTON_up();    
		break;
      case _LCDML_CONTROL_serial_down:   
		Serial.println("down");
		LCDML_BUTTON_down();  
		break;
      case _LCDML_CONTROL_serial_left:   LCDML_BUTTON_left();  break;
      case _LCDML_CONTROL_serial_right:  LCDML_BUTTON_right(); break;
      case _LCDML_CONTROL_serial_quit:   LCDML_BUTTON_quit();  break;
      default: break;
    } 
  }
}
// *********************************************************************
// ******************************* END *********************************
// *********************************************************************





// *********************************************************************
// *************** (1) CONTROL OVER ONE ANALOG PIN *********************
// *********************************************************************
#elif(_LCDML_CONTROL_cfg == 1)
// settings
  #define _LCDML_CONTROL_analog_pin              0
  // when you did not use a button set the value to zero
  #define _LCDML_CONTROL_analog_enter_min        850     // Button Enter
  #define _LCDML_CONTROL_analog_enter_max        920  
  #define _LCDML_CONTROL_analog_up_min           520     // Button Up
  #define _LCDML_CONTROL_analog_up_max           590   
  #define _LCDML_CONTROL_analog_down_min         700     // Button Down
  #define _LCDML_CONTROL_analog_down_max         770   
  #define _LCDML_CONTROL_analog_back_min         950     // Button Back
  #define _LCDML_CONTROL_analog_back_max         1020   
  #define _LCDML_CONTROL_analog_left_min         430     // Button Left
  #define _LCDML_CONTROL_analog_left_max         500   
  #define _LCDML_CONTROL_analog_right_min        610     // Button Right
  #define _LCDML_CONTROL_analog_right_max        680
// *********************************************************************
// setup
void LCDML_CONTROL_setup()
{
}
// *********************************************************************
// loop
void LCDML_CONTROL_loop()
{
  // check debounce timer  
  if((millis() - g_LCDML_DISP_press_time) >= _LCDML_DISP_cfg_button_press_time) {
    g_LCDML_DISP_press_time = millis(); // reset debounce timer
    
    uint16_t value = analogRead(_LCDML_CONTROL_analog_pin);  // analogpin for keypad
    
    if (value >= _LCDML_CONTROL_analog_enter_min && value <= _LCDML_CONTROL_analog_enter_max) { LCDML_BUTTON_enter(); }
    if (value >= _LCDML_CONTROL_analog_up_min    && value <= _LCDML_CONTROL_analog_up_max)    { LCDML_BUTTON_up();    }
    if (value >= _LCDML_CONTROL_analog_down_min  && value <= _LCDML_CONTROL_analog_down_max)  { LCDML_BUTTON_down();  }
    if (value >= _LCDML_CONTROL_analog_left_min  && value <= _LCDML_CONTROL_analog_left_max)  { LCDML_BUTTON_left();  }
    if (value >= _LCDML_CONTROL_analog_right_min && value <= _LCDML_CONTROL_analog_right_max) { LCDML_BUTTON_right(); }
    if (value >= _LCDML_CONTROL_analog_back_min  && value <= _LCDML_CONTROL_analog_back_max)  { LCDML_BUTTON_quit();  }
  }
}
// *********************************************************************
// ******************************* END *********************************
// *********************************************************************






// *********************************************************************
// *************** (2) CONTROL OVER DIGITAL PINS ***********************
// *********************************************************************
#elif(_LCDML_CONTROL_cfg == 2)
// settings
  #define _LCDML_CONTROL_digital_low_active      0    // (0 = low active (pullup), 1 = high active (pulldown) button
                                                      // http://playground.arduino.cc/CommonTopics/PullUpDownResistor
  #define _LCDML_CONTROL_digital_enable_quit     0 //1
  #define _LCDML_CONTROL_digital_enable_lr       0 //1
  #define _LCDML_CONTROL_digital_enter           12 //8    
  #define _LCDML_CONTROL_digital_up              3 //9
  #define _LCDML_CONTROL_digital_down            2 //10
  #define _LCDML_CONTROL_digital_quit            11
  #define _LCDML_CONTROL_digital_left            12
  #define _LCDML_CONTROL_digital_right           13
// *********************************************************************
// setup
void LCDML_CONTROL_setup()
{
  // init buttons
  pinMode(_LCDML_CONTROL_digital_enter      , INPUT_PULLUP);
  pinMode(_LCDML_CONTROL_digital_up         , INPUT_PULLUP);
  pinMode(_LCDML_CONTROL_digital_down       , INPUT_PULLUP);  
  # if(_LCDML_CONTROL_digital_enable_quit == 1)
    pinMode(_LCDML_CONTROL_digital_quit     , INPUT_PULLUP);
  # endif
  # if(_LCDML_CONTROL_digital_enable_lr == 1)
    pinMode(_LCDML_CONTROL_digital_left     , INPUT_PULLUP);
    pinMode(_LCDML_CONTROL_digital_right    , INPUT_PULLUP);
  # endif
}
// *********************************************************************
// loop
void LCDML_CONTROL_loop()
{
  #if(_LCDML_CONTROL_digital_low_active == 1)
  #  define _LCDML_CONTROL_digital_a !
  #else
  #  define _LCDML_CONTROL_digital_a
  #endif
  
  uint8_t but_stat = 0x00;
  
  bitWrite(but_stat, 0, _LCDML_CONTROL_digital_a(digitalRead(_LCDML_CONTROL_digital_enter)));
  bitWrite(but_stat, 1, _LCDML_CONTROL_digital_a(digitalRead(_LCDML_CONTROL_digital_up)));
  bitWrite(but_stat, 2, _LCDML_CONTROL_digital_a(digitalRead(_LCDML_CONTROL_digital_down)));
  #if(_LCDML_CONTROL_digital_enable_quit == 1)
  bitWrite(but_stat, 3, _LCDML_CONTROL_digital_a(digitalRead(_LCDML_CONTROL_digital_quit)));
  #endif
  #if(_LCDML_CONTROL_digital_enable_lr == 1)
  bitWrite(but_stat, 4, _LCDML_CONTROL_digital_a(digitalRead(_LCDML_CONTROL_digital_left)));
  bitWrite(but_stat, 5, _LCDML_CONTROL_digital_a(digitalRead(_LCDML_CONTROL_digital_right)));
  #endif  
  
  if (but_stat > 0) {
    if((millis() - g_LCDML_DISP_press_time) >= _LCDML_DISP_cfg_button_press_time) {
      g_LCDML_DISP_press_time = millis(); // reset press time
    
      if (bitRead(but_stat, 0)) { LCDML_BUTTON_enter(); }
      if (bitRead(but_stat, 1)) { LCDML_BUTTON_up();    }
      if (bitRead(but_stat, 2)) { LCDML_BUTTON_down();  }
      if (bitRead(but_stat, 3)) { LCDML_BUTTON_quit();  }
      if (bitRead(but_stat, 4)) { LCDML_BUTTON_left();  }
      if (bitRead(but_stat, 5)) { LCDML_BUTTON_right(); }        
    }    
  }
}
// *********************************************************************
// ******************************* END *********************************
// *********************************************************************






// *********************************************************************
// *************** (3) CONTROL WITH ENCODER ****************************
// *********************************************************************
#elif(_LCDML_CONTROL_cfg == 3)
// settings
  #define _LCDML_CONTROL_encoder_pin_a           10 // pin encoder b
  #define _LCDML_CONTROL_encoder_pin_b           11 // pin encoder a
  #define _LCDML_CONTROL_encoder_pin_button      12 // pin taster
  #define _LCDML_CONTROL_encoder_high_active     0  // (0 = low active (pullup), 1 = high active (pulldown)) button
                                                      // // http://playground.arduino.cc/CommonTopics/PullUpDownResistor
// global defines
  uint8_t  g_LCDML_CONTROL_encoder_t_prev = 0;
  uint8_t  g_LCDML_CONTROL_encoder_a_prev = 0;

// *********************************************************************
// setup
void LCDML_CONTROL_setup()
{
  // set encoder update intervall time 
  LCDML_BACK_dynamic_setLoopTime(LCDML_BACKEND_control, 5UL);  // 5ms 

  // init pins  
  pinMode(_LCDML_CONTROL_encoder_pin_a      , INPUT_PULLUP);
  pinMode(_LCDML_CONTROL_encoder_pin_b      , INPUT_PULLUP);
  pinMode(_LCDML_CONTROL_encoder_pin_button , INPUT_PULLUP); 
}
// *********************************************************************
// loop
void LCDML_CONTROL_loop()
{    
  // read encoder status
  unsigned char a = digitalRead(_LCDML_CONTROL_encoder_pin_a);
  unsigned char b = digitalRead(_LCDML_CONTROL_encoder_pin_b);
  unsigned char t = digitalRead(_LCDML_CONTROL_encoder_pin_button);
  
  // change button status if high and low active are switched
  if (_LCDML_CONTROL_encoder_high_active == 1) {
    t != t;
  }
  
  // check encoder status and set control menu
  if (!a && g_LCDML_CONTROL_encoder_a_prev) {
    g_LCDML_CONTROL_encoder_t_prev = 1;
    
    if (!b) { LCDML_BUTTON_up();   }
    else    { LCDML_BUTTON_down(); }            
  } 
  else {
    // check button press time for enter
    if((millis() - g_LCDML_DISP_press_time) >= _LCDML_DISP_cfg_button_press_time) {
      g_LCDML_DISP_press_time = millis(); // reset button press time
      
      // press button once
      if (!t && g_LCDML_CONTROL_encoder_t_prev == 0) {          
          LCDML_BUTTON_enter();          
      } 
      else {
        g_LCDML_CONTROL_encoder_t_prev = 0;
      }
    }      
  }
  g_LCDML_CONTROL_encoder_a_prev = a;  // set new encoder status 
  
}
// *********************************************************************
// ******************************* END *********************************
// *********************************************************************





// *********************************************************************
// *************** (4) CONTROL WITH A KEYPAD ***************************
// *********************************************************************
#elif(_LCDML_CONTROL_cfg == 4)
// include
  // more information under http://playground.arduino.cc/Main/KeypadTutorial
  #include <Keypad.h>
// settings
  #define _LCDML_CONTROL_keypad_rows 4 // Four rows
  #define _LCDML_CONTROL_keypad_cols 3 // Three columns
// global vars
  char keys[_LCDML_CONTROL_keypad_rows][_LCDML_CONTROL_keypad_cols] = { 
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'#','0','*'}
  };  
  byte rowPins[_LCDML_CONTROL_keypad_rows] = { 9, 8, 7, 6 };  // Connect keypad COL0, COL1 and COL2 to these Arduino pins.
  byte colPins[_LCDML_CONTROL_keypad_cols] = { 12, 11, 10 };  // Create the Keypad
// objects
  Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, _LCDML_CONTROL_keypad_rows, _LCDML_CONTROL_keypad_cols );
// *********************************************************************
// setup
void LCDML_CONTROL_setup()
{
}
// *********************************************************************
// loop
void LCDML_CONTROL_loop()
{    
  char key = kpd.getKey();
  if(key)  // Check for a valid key.
  {
    switch (key)
    {
      case '#': LCDML_BUTTON_enter(); break;
      case '2': LCDML_BUTTON_up();    break;
      case '8': LCDML_BUTTON_down();  break;
      case '4': LCDML_BUTTON_left();  break;
      case '6': LCDML_BUTTON_right(); break;
      case '*': LCDML_BUTTON_quit();  break;
      default: break;       
    }
  } 
}
// *********************************************************************
// *************** (5) CONTROL WITH A Moja Klawiatura ***************************
// *********************************************************************
#elif(_LCDML_CONTROL_cfg == 5)
// *********************************************************************
// setup
void LCDML_CONTROL_setup()
{
}
// *********************************************************************
// loop
void LCDML_CONTROL_loop()
{    
	if (konf_state)
	{
		char key = getKey();
		if(key)
		{
			switch (key)
			{
				case M1_KEY: LCDML_BUTTON_enter(); break;
				case UP_KEY: LCDML_BUTTON_up();    break;
				case DOWN_KEY: LCDML_BUTTON_down();  break;
				case SETUP_KEY: 
					//LCDML_BUTTON_left();  
					konf_state = false;
					LCDML.goRoot();
					tft.fillScreen(ILI9341_BLACK);
					show_template();
					show_frequency(0);
					show_frequency(1);
				break;
				case ATT_KEY: LCDML_BUTTON_right(); break;
				case FILTR_CW_KEY: LCDML_BUTTON_quit();  break;
				default: break;       
			}
			delay(200);
		}
	} 
}
// *********************************************************************
// ******************************* END *********************************
// *********************************************************************

#else
  #error _LCDML_CONTROL_cfg is not defined or not in range
#endif

