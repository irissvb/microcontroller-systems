/*
4-Bit 16x2 LCD
*/

//#define F_CPU 16000000UL   // Not necessary in simulator
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Your global variables here
int current_led = 0; 			// 0: Yellow, 1: Green, 2: Red
uint8_t states[3] = {0, 0, 0};	// 0: OFF, 1: ON
char* colors[] = {"Yellow:  ", "Green:   ", "Red:     "};

// Defination of pot and pin variables
#define LCD_Port         PORTD	// Define LCD Port (PORTA, PORTB, PORTC, PORTD)
#define LCD_DPin         DDRD	// Define 4-Bit Pins (PD4-PD7 at PORT D)
#define LCD_DATA0_PIN    PD4    // < pin for 4bit data bit 0  
#define LCD_DATA1_PIN    PD5    // < pin for 4bit data bit 1  
#define LCD_DATA2_PIN    PD6    // < pin for 4bit data bit 2  
#define LCD_DATA3_PIN    PD7    // < pin for 4bit data bit 3  
#define RSPIN 			 PD3	// RS Pin
#define ENPIN            PD2 	// E Pin
int runtime;			 //Timer for LCD

// LCD initialization function with the minimum commands list
void LCD_Init (void)
{
  // Data and control pins as outputs
  LCD_DPin |= (1<<LCD_DATA0_PIN)|(1<<LCD_DATA1_PIN)|(1<<LCD_DATA2_PIN)|(1<<LCD_DATA3_PIN)|(1<<RSPIN)|(1<<ENPIN);		//Control LCD Pins (D4-D7)
  _delay_ms(16);		//Wait more when 15 ms after start
  
  LCD_Action(0x02);	    // Returns the cursor to the home position (Address 0). Returns display to its original state if it was shifted. 
  LCD_Action(0x28);       // Data sent or received in 4 bit lengths (DB7-DB4)
  LCD_Action(0x0C);       // Display on, cursor off
  LCD_Action(0x06);       // Increment cursor (shift cursor to right)
  LCD_Action(0x01);       // Clean LCD
  _delay_ms(5);
}

// Function to send commands and data to the LCD
void LCD_Action(unsigned char cmnd)
{
  LCD_Port = (LCD_Port & 0x0F) | (cmnd & 0xF0);
  LCD_Port &= ~ (1<<RSPIN);
  LCD_Port |= (1<<ENPIN);
  _delay_us(1);
  LCD_Port &= ~ (1<<ENPIN);
  _delay_us(200);
  LCD_Port = (LCD_Port & 0x0F) | (cmnd << 4);
  LCD_Port |= (1<<ENPIN);
  _delay_us(1);
  LCD_Port &= ~ (1<<ENPIN);
  _delay_ms(2);
}

// Clear whole LCD
void LCD_Clear()
{
  LCD_Action (0x01);	//Clear LCD
  _delay_ms(2);			//Wait to clean LCD
  LCD_Action (0x80);	//Move to Position Line 1, Position 1
}

// Prints string of chars 
void LCD_Print (char *str)
{
  int i;
  for(i=0; str[i]!=0; i++)
  {
    LCD_Port = (LCD_Port & 0x0F) | (str[i] & 0xF0);
    LCD_Port |= (1<<RSPIN);
    LCD_Port|= (1<<ENPIN);
    _delay_us(1);
    LCD_Port &= ~ (1<<ENPIN);
    _delay_us(200);
    LCD_Port = (LCD_Port & 0x0F) | (str[i] << 4);
    LCD_Port |= (1<<ENPIN);
    _delay_us(1);
    LCD_Port &= ~ (1<<ENPIN);
    _delay_ms(2);
  }
}

// Prints string of chars to the specific location
// row - the row 0 or 1;
// pos - the column from 0 till 16
void LCD_Printpos (char row, char pos, char *str)
{
  if (row == 0 && pos<16)
    LCD_Action((pos & 0x0F)|0x80);
  else if (row == 1 && pos<16)
    LCD_Action((pos & 0x0F)|0xC0);
  LCD_Print(str);
}

void update_leds() {
  if(states[0]) PORTB |= (1 << PB5); else PORTB &= ~(1 << PB5);
  if(states[1]) PORTB |= (1 << PB4); else PORTB &= ~(1 << PB4);
  if(states[2]) PORTB |= (1 << PB3); else PORTB &= ~(1 << PB3);
}

void refresh_text() {
  LCD_Printpos(0, 0, "VilniusTech Menu");
  LCD_Printpos(1, 0, "LED ");
  LCD_Print(colors[current_led]);
  
  if(states[current_led]) {
    LCD_Print("ON ");
  } else {
    LCD_Print("OFF");
  }
}

void setup() {
  // Buttons M1 (PB2), M2 (PB1), M3 (PB0)
  DDRB &= ~((1 << PB2) | (1 << PB1) | (1 << PB0));
  PORTB |= (1 << PB2) | (1 << PB1) | (1 << PB0);
  
  // LEDs Yelloww (PB5), Green (PB4), Red (PB3)
  DDRB |= (1 << PB5) | (1 << PB4) | (1 << PB3);
}
 
int main()
{
  LCD_Init();
  
  setup();
  
  refresh_text();
  
  while(1) {
    // M1 button: select previous LED
    if (!(PINB & (1 << PB2))) {
      _delay_ms(50);
      current_led = (current_led + 2) % 3;
      refresh_text();
      while (!(PINB & (1 << PB2)));
      _delay_ms(50);
    }
    
    // M2 button: select next LED
    if (!(PINB & (1 << PB1))) {
      _delay_ms(50);
      current_led = (current_led + 1) % 3;
      refresh_text();
      while (!(PINB & (1 << PB1)));
      _delay_ms(50);
    }
    
    // M3 button: change status of current LED
    if (!(PINB & (1 << PB0))) {
      _delay_ms(50);
      states[current_led] = !states[current_led];
      update_leds();
      refresh_text();
      while (!(PINB & (1 << PB1)));
      _delay_ms(50);
    }
  }
  return 0;
}