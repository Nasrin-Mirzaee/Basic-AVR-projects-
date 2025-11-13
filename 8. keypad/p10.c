#include <mega32.h>
#include <delay.h>
#include <stdio.h>
#include <alcd.h>


unsigned char key_raw, key_index;
char str[16];

// Keypad character mapping
const char key_map[16] = {
    '1','2','3','A',
    '4','5','6','B',
    '7','8','9','C',
    '*','0','#','D'
};


void main(void)
{
// Declare your local variables here

// Input/Output Ports initialization
// Port A initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTA=0x00;
DDRA=0x00;

// Port B initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTB=0x00;
DDRB=0xff;

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
// Clock value: Timer1 Stopped
// Mode: Normal top=FFFFh
// OC1A output: Discon.
// OC1B output: Discon.
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
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
// Clock value: Timer2 Stopped
// Mode: Normal top=FFh
// OC2 output: Disconnected
ASSR=0x00;
TCCR2=0x00;
TCNT2=0x00;
OCR2=0x00;

// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
// INT2: Off
MCUCR=0x00;
MCUCSR=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=0x00;

// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
ACSR=0x80;
SFIOR=0x00;
lcd_init(16);


             
while(1)
      { 
        // Scan keypad
        do {
            DDRA = 0x0F;     // Lower nibble as output
            PORTA = 0xF0;    // Pull-up on upper nibble
            delay_us(100);
            key_raw = PINA;

            DDRA = 0xF0;     // Upper nibble as output
            PORTA = 0x0F;    // Pull-up on lower nibble
            delay_us(100);
            key_raw |= PINA;

        } while (key_raw == 0xFF);  // Wait until a key is pressed

        // Map raw value to key index
        switch (key_raw)
        {
            case 0b11101110: key_index = 0; break;
            case 0b11011110: key_index = 1; break;
            case 0b10111110: key_index = 2; break;
            case 0b01111110: key_index = 3; break;
            case 0b11101101: key_index = 4; break;
            case 0b11011101: key_index = 5; break;
            case 0b10111101: key_index = 6; break;
            case 0b01111101: key_index = 7; break;
            case 0b11101011: key_index = 8; break;
            case 0b11011011: key_index = 9; break;
            case 0b10111011: key_index = 10; break;
            case 0b01111011: key_index = 11; break;
            case 0b11100111: key_index = 12; break;
            case 0b11010111: key_index = 13; break;
            case 0b10110111: key_index = 14; break;
            case 0b01110111: key_index = 15; break;
            default: key_index = 255; break; // Invalid
        }

        if (key_index < 16)
        {
            lcd_clear();
            lcd_gotoxy(0, 0);
            sprintf(str, "Key: %c", key_map[key_index]);
            lcd_puts(str);
        }

        // Wait until key is released
        while (key_raw != 0xFF)
        {
            DDRA = 0x0F;
            PORTA = 0xF0;
            delay_us(100);
            key_raw = PINA;

            DDRA = 0xF0;
            PORTA = 0x0F;
            delay_us(100);
            key_raw |= PINA;
        }

        delay_ms(200);  // Simple debounce
    }
}
