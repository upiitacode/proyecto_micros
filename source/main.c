#include "stm32f30x.h"                  // Device header

#include <stdio.h>
#include <string.h>
/*Led PB13, Button PC13*/

void delay_ms(int delay_time);
void led_init(void);

void USART2_init(void);
void USART2_sendChar(char ch);
char USART2_getChar(void);

void s_puts(const char * pString);
int s_gets(char * pBuffer, int bufferSize);
char mybf[80];
char myOutBuffer[80];
char wordBuffer[80];
int main(){
	int lineCounter=1;
	led_init();
	USART2_init();
	s_puts("\nSystem ready\n");
	while(1){
		snprintf(myOutBuffer,80,"%d$ ",lineCounter);
		s_puts(myOutBuffer);
		s_gets(mybf,80);
		snprintf(myOutBuffer,80,"%s\n",mybf);
		s_puts(myOutBuffer);
		if(sscanf(mybf,"%s",wordBuffer) > 0){
			snprintf(myOutBuffer,80,"word: %s\n",wordBuffer);
			s_puts(myOutBuffer);
			snprintf(myOutBuffer,80,"characters: %d\n",strlen(wordBuffer));
			s_puts(myOutBuffer);
		}
		lineCounter++;
	}
	return 0;
}

int s_gets(char * pBuffer, int bufferSize){
	char newChar; 
	int i ;
	for(i = 0 ; i < (bufferSize - 1); i++){
		newChar = USART2_getChar();
		if( (newChar != '\r') ){
			pBuffer[i]=newChar;
		}else{
			pBuffer[i]='\0';
			return i;
		}
	}
	pBuffer[bufferSize-1]='\0';
	return bufferSize-1;
}

void s_puts(const char * pString){
	char newChar; 
	int i ;
	for(i = 0 ; i < 81; i++){
		newChar = pString[i];
		if( newChar != '\0' ){
			USART2_sendChar(newChar);
		}else{
			break;
		}
		
	}
}

void USART2_sendChar(char ch){
	while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));
	
	USART_SendData(USART2,ch);
}

char USART2_getChar(void){
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)) USART_ClearFlag(USART2,USART_FLAG_ORE); 
	
	while(!USART_GetFlagStatus(USART2,USART_FLAG_RXNE));
	return USART_ReceiveData(USART2);
}

void USART2_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/*Enable peripheral clock for GPIOA*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	/*Enable peripheral clock for UART2*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	/*GPIOA Configuration PA2 as TX PA3 as RX*/
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/*Connect USART2 pins to AF2*/
	//TX=PA2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_7);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_7);
	
	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_Init(USART2,&USART_InitStructure);
	USART_Cmd(USART2,ENABLE);
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
