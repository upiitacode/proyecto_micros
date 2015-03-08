#include "stm32f30x.h"                  // Device header
/*Led PB13, Button PC13*/
#include "retarget_stm32f3.h"
#include <stdio.h>
void delay_ms(int delay_time);


int main(){
	UART2_init();
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
	GPIO_InitTypeDef myGPIO;
	GPIO_StructInit(&myGPIO);
	myGPIO.GPIO_Pin=GPIO_Pin_13;
	myGPIO.GPIO_Mode=GPIO_Mode_OUT;
	myGPIO.GPIO_OType=GPIO_OType_PP;
	myGPIO.GPIO_PuPd=GPIO_PuPd_NOPULL;
	myGPIO.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&myGPIO);
	while(1){
		GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
		delay_ms(0xFFFFF);
		GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
		delay_ms(0xFFFFF);
		printf("Hello world\n");
	}
}

void delay_ms(int delay_time){
	for(int i=0; i<delay_time; i++);
}
