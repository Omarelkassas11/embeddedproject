#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
 
 void setupUART(){
 
    /* Set Baudrate  */
       UBRR0L =(uint8_t) (103 & 0xFF);
    UBRR0H =(uint8_t) (103>>8);
 
 
    UCSR0B = (1<<TXEN0); /* Enable  transmitter                 */
 };
  void setupADC(){
     ADMUX |=(1<<REFS0);           //Clear MUX0-MUX3,Important in Multichannel conv
     // ATmega328p ADC Configuration Code 
      ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0)|(1<<ADEN);// Select ADC Prescalar to 128,
                                                    // 11.0598MHz/128 = 85KHz
   
};
 
 void uartPrint(unsigned char data ){
  
          while (!( UCSR0A & (1<<UDRE0))); /* Wait for empty transmit buffer*/
     
            UDR0 =data;  
  };
 void uartPrintHex8(uint8_t value){
  
  uint8_t upper = (value & 0xF0)>>4;
  uint8_t lower = (value & 0x0F);

  upper += upper >9?'A'-10:'0';
  lower += lower>9?'A'-10:'0';

  uartPrint(upper);
  uartPrint(lower);
  };

int main()
{
  setupUART();
  setupADC();
DDRB |= _BV(DDB5);
  uint8_t  ADC_8bit_High_Byte = 0; // unsigned int 8 bit variable to get the ADC High Byte
  uint8_t  ADC_8bit_Low_Byte  = 0; // unsigned int 8 bit variable to get the ADC Low Byte
    while(1) /* Loop the messsage continously */
    { 
      ADCSRA |= (1<<ADEN)  | (1<<ADSC);                // Enable ADC and Start the conversion
  
    while( !(ADCSRA & (1<<ADIF)) );                  // Wait for conversion to finish
                    
    ADC_8bit_Low_Byte   = ADCL;   // Read the 8 bit ADCL first,then ADCH
    ADC_8bit_High_Byte  = ADCH; 
 /* print the data*/
       uartPrintHex8(ADC_8bit_Low_Byte);
       uartPrintHex8(ADC_8bit_High_Byte);
        uartPrint('\n');
      if(ADC==0)
        PORTB|=_BV(PORTB5);
       else
        PORTB&=~_BV(PORTB5);
        ADCSRA |= (1<<ADIF);   // Clear ADIF,ADIF is cleared by writing a 1 to ADSCRA (According to ATmega328p datasheet)
      _delay_ms(500);
      }
}//end of main
