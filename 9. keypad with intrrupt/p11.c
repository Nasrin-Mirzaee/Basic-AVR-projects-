#include <delay.h>
#include <mega32.h>
#include <stdio.h>


// Alphanumeric LCD Module functions
#asm
   .equ __lcd_port=0x18 ;PORTB
#endasm
#include <lcd.h>

unsigned char raw_key_code;     // Raw key code from keypad
unsigned char row_mask;         // Mask used to scan keypad rows
unsigned char row_index;        // Row index during scanning
unsigned char lcd_buffer[16];   // Buffer to hold LCD text
unsigned char key_number = 0;   // Final key number (1 to 16)


interrupt [EXT_INT0] void ext_int0_isr(void)
{
    // Set PORTA upper nibble (PA4–PA7) to high
    PORTA = 0xF0;

    // Start with row mask: 11111110 (only PA0 low)
    row_mask = 0b11111110;

    // Scan each row (PA0 to PA3)
    for (row_index = 0; row_index < 4; row_index++)
    {
        PORTA = row_mask;       // Activate one row
        delay_ms(10);           // Small delay for stabilization

        // Check if any column (PA4–PA7) is low (key press detected)
        if ((PINA & 0xF0) != 0xF0)
            break;

        // Shift mask to activate next row
        row_mask <<= 1;
        row_mask |= 1;
    }

    // Combine row and column bits to get full key code
    raw_key_code = (PINA & 0xF0) | (row_mask & 0x0F);

    // Map raw key code to key number (1 to 16)
    switch (raw_key_code)
    {
        case 0b11101110: key_number = 1; break;
        case 0b11011110: key_number = 2; break;
        case 0b10111110: key_number = 3; break;
        case 0b01111110: key_number = 4; break;
        case 0b11101101: key_number = 5; break;
        case 0b11011101: key_number = 6; break;
        case 0b10111101: key_number = 7; break;
        case 0b01111101: key_number = 8; break;
        case 0b11101011: key_number = 9; break;
        case 0b11011011: key_number = 10; break;
        case 0b10111011: key_number = 11; break;
        case 0b01111011: key_number = 12; break;
        case 0b11100111: key_number = 13; break;
        case 0b11010111: key_number = 14; break;
        case 0b10110111: key_number = 15; break;
        case 0b01110111: key_number = 16; break;
    }

    // Reset PORTA to default state
    PORTA = 0xF0;
}



// Declare your global variables here

void main(void)
{
// Declare your local variables here

// Input/Output Ports initialization
// Port A initialization
// Func7=In Func6=In Func5=In Func4=In Func3=Out Func2=Out Func1=Out Func0=Out 
// State7=P State6=P State5=P State4=P State3=0 State2=0 State1=0 State0=0 
PORTA=0xF0;
DDRA=0x0F;

// Port B initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTB=0x00;
DDRB=0x00;

// Port C initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTC=0x00;
DDRC=0x00;

// Port D initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTD=0x00;
DDRD=0x00;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
// Mode: Normal top=FFh
// OC0 output: Disconnected
TCCR0=0x00;
TCNT0=0x00;
OCR0=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: Timer 1 Stopped
// Mode: Normal top=FFFFh
// OC1A output: Discon.
// OC1B output: Discon.
// Noise Canceler: Off
// Input Capture on Falling Edge
TCCR1A=0x00;
TCCR1B=0x00;
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: Timer 2 Stopped
// Mode: Normal top=FFh
// OC2 output: Disconnected
ASSR=0x00;
TCCR2=0x00;
TCNT2=0x00;
OCR2=0x00;

// External Interrupt(s) initialization
// INT0: On
// INT0 Mode: Falling Edge
// INT1: Off
// INT2: Off
GICR|=0x40;
MCUCR=0x02;
MCUCSR=0x00;
GIFR=0x40;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=0x00;

// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
// Analog Comparator Output: Off
ACSR=0x80;
SFIOR=0x00;

// LCD module initialization
lcd_init(16);

// Global enable interrupts
#asm("sei")
                              

while (1)
      {
       
        // Display the last detected key number on LCD
        sprintf(lcd_buffer, "Key = %2.2d", key_number);
        lcd_gotoxy(0, 0);
        lcd_puts(lcd_buffer);


      }
}
