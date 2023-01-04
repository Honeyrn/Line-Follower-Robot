#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>


#define IR1 (PINB&(1<<4))
#define IR2 (PINB&(1<<5))
#define IR3 (PINB&(1<<6))
#define RS 0
#define RW 1
#define EN 2


void lcdCommand(unsigned char cmnd)
{
   PORTC = cmnd; //Data PORT
   PORTD &= ~(1<<RS);	//RS = 0 for command (command port)
   PORTD &= ~(1<<RW);
   PORTD |= (1<<EN);
   
   _delay_ms(10);
   
   PORTD &= ~(1<<EN);
   
   _delay_ms(100);
}
void lcdData(unsigned char data1)
{
   PORTC = data1; //Data PORT
   PORTD |= (1<<RS);	//RS = 1 for command (command port)
   PORTD &= ~(1<<RW);
   PORTD |= (1<<EN);
 
   _delay_us(1);
   
   PORTD &= ~(1<<EN);
   
   _delay_ms(100);
}
void lcd_init()
{
   DDRD = 0xFF;
   DDRC = 0xFF;
   _delay_ms(100);
   lcdCommand(0x38);
   lcdCommand(0x0E);
   lcdCommand(0x06);
   lcdCommand(0x01);
}
void lcdPrint(char str[100])
{
   unsigned char i=0;
   while(str!=0)
   {
      lcdData(str[i]);
      i++;
   }
}


void stop()
{
   PORTA = 0x00;
   PORTB |= (1<<PB3);
   _delay_ms(100);
}

void front()
{
   PORTA = 0x05;
   OCR0 = 229;
   _delay_ms(200);
   //PORTB |= (1<<PB3);
   //_delay_ms(100);
}
void right()
{
   PORTA = 0x06;
   PORTB |= (1<<PB3);
   _delay_ms(100);

}
void left()
{
   PORTA = 0x09;
   PORTB |= (1<<PB3);
   _delay_ms(100);
}

void PWM_init()
{
    //OCR0 = 32;			//87.5% DC - 47rmp
    OCR0 = 0;
    TCCR0 = 0x74;		// Phase Correct PWM, N=256, inverted, 
    //DDRB |= (1<<3);
}

int main()
 { 
   
   //For DC Motor
    DDRA |= ((1<<0)|(1<<1)|(1<<2)|(1<<3));    // A0 to A3 as o/p port
    DDRB |= (1<<PB3);			//PB3 as ENA and ENB, PB3 as output
    
    //For Infrared Sensors
    DDRB |= (1<<7);			//LED 
    //PORTB=0x7f;
    PORTB = 0x0F;
    
    PWM_init();
    
    //For LCD
  
    lcd_init();
    lcdCommand(0x81);
    //lcdPrint("Hey there");
    
    
   /* while (1){
      //if(((PINB & (1<<4)) && ((PINB & (1<<5)) == 0) && ((PINB & (1<<6)) == 0)))	//100 - 4 - Left
      //if((((PINB & (1<<4))==0) && (PINB & (1<<5)) && ((PINB & (1<<6)) == 0)))		//010 - 2 - Front
      //if((((PINB & (1<<4))==0) && ((PINB & (1<<5)) == 0) && (PINB & (1<<6))))		//001 - 1 - Right
      {
	 PORTB |= (1<<7);
	 right();
	 
      }
      else
	 PORTB &= ~ (1<<7);
	 
   };
   */
   
   
    while(1)
    {       
      if((((PINB & (1<<4))==0) && ((PINB & (1<<5)) == 0) && (PINB & (1<<6))))		//001 - 1 - Right
      {
	 left();
      }
      else if((((PINB & (1<<4))==0) && (PINB & (1<<5)) && ((PINB & (1<<6)) == 0)))		//010 - 2 - Front
      {
	 PORTB &= ~ (1<<7);
	 front();
      }
      else if(((PINB & (1<<4)) && ((PINB & (1<<5)) == 0) && ((PINB & (1<<6)) == 0)))	//100 - 4 - Left
      {
	 right();
      }
    }
    while(1);
   return 0;
 }


