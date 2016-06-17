#include "stm32f30x.h"                  // Device header
#include "serial_stdio.h"
#include "retarget_stm32f3.h"
#include "adc_stm32f3.h"
#include <string.h>
/*Led PB13, Button PC13*/

void delay_ms(int delay_time);
void led_init(void);

Serial_t USART2_Serial={USART2_getChar,USART2_sendChar};

char mybf[80];/*Input buffer*/
char wordBuffer[80];
uint16_t adc_data;

int main(){
	led_init();
	USART2_init(9600);
	serial_puts(USART2_Serial,"\nSystem ready\n");
    adc_init_injected();
	while(1){
        adc_data = adc_read_data(0);
		serial_printf(USART2_Serial,"adc = %d\n", adc_data);
        delay_ms(1000);
	}
}


void led_init(void){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
	GPIO_InitTypeDef myGPIO;
	GPIO_StructInit(&myGPIO);
	myGPIO.GPIO_Pin=GPIO_Pin_13;
	myGPIO.GPIO_Mode=GPIO_Mode_OUT;
	myGPIO.GPIO_OType=GPIO_OType_PP;
	myGPIO.GPIO_PuPd=GPIO_PuPd_NOPULL;
	myGPIO.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&myGPIO);
	GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
}

void delay_ms(int delay_time){
	for(int i=0; i<delay_time; i++);
}
