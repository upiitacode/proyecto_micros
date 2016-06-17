#include "stm32f30x.h"                  // Device header
#include "serial_stdio.h"
#include "retarget_stm32f3.h"
#include "adc_stm32f3.h"
#include "ctimers_stmf3.h"
#include "delay.h"
#include <string.h>

/*Led PB13, Button PC13*/

void led_init(void);
    
Serial_t USART2_Serial={USART2_getChar,USART2_sendChar};

char mybf[80];/*Input buffer*/
char wordBuffer[80];
uint16_t adc_data;

int mark_ready = 0;
uint32_t mark_time[4];

void cny_init(void);

int main(){
    SystemCoreClockUpdate();
	led_init();
	USART2_init(9600);
	serial_puts(USART2_Serial,"\nExamen de dispositivos programables\n");
    adc_init_injected();
    cny_init();
    
    mark_ready = 0;
	while(1){
        adc_data = adc_read_data(0);
        serial_printf(USART2_Serial,"angulo = %f\n",((adc_data)*(297.0/4095.0)));        
        if(mark_ready){
            serial_printf(USART2_Serial,"Nueva vola!\n"); 
            for(int i  = 0;  i < 4; i++){
                serial_printf(USART2_Serial,"tiempo punto %d = %f[ms]\n",i+1,(mark_time[i]/(1000.0))); 
            }
            serial_printf(USART2_Serial,"tiempo total = %f[ms]\n",(mark_time[3]/(1000.0)));
            serial_printf(USART2_Serial,"velocidad total = %f[m/s]\n",((1.12)/(mark_time[3]/(1000000.0))));
            mark_ready = 0;
        }
        delay_ms(500);
	}
}

void cny_init(void){
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);
	GPIO_InitTypeDef myGPIO;
	GPIO_StructInit(&myGPIO);
	myGPIO.GPIO_Pin=GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	myGPIO.GPIO_Mode=GPIO_Mode_IN;
	myGPIO.GPIO_PuPd=GPIO_PuPd_DOWN;
	myGPIO.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOC,&myGPIO);
    
    EXTI_InitTypeDef myEXTI;
    EXTI_StructInit(&myEXTI);
    
    myEXTI.EXTI_LineCmd = ENABLE;
    myEXTI.EXTI_Mode = EXTI_Mode_Interrupt;
    myEXTI.EXTI_Trigger = EXTI_Trigger_Rising;
    myEXTI.EXTI_Line = EXTI_Line11;
    EXTI_Init(&myEXTI);
    
    myEXTI.EXTI_Line = EXTI_Line12;
    EXTI_Init(&myEXTI);
    
    myEXTI.EXTI_Line = EXTI_Line13;
    EXTI_Init(&myEXTI);
    
    myEXTI.EXTI_Line = EXTI_Line14;
    EXTI_Init(&myEXTI);
    
    myEXTI.EXTI_Line = EXTI_Line15;
    EXTI_Init(&myEXTI);
    
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_SYSCFGEN,ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource11);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource12);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource13);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource14);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource15);
    
    EXTI_ClearITPendingBit(EXTI_Line11);
    EXTI_ClearITPendingBit(EXTI_Line12);
    EXTI_ClearITPendingBit(EXTI_Line13);
    EXTI_ClearITPendingBit(EXTI_Line14);
    EXTI_ClearITPendingBit(EXTI_Line15);
    
    NVIC_EnableIRQ(EXTI15_10_IRQn);
    timer2_init(80000000,1);
}

int mark_count = 0;
void EXTI15_10_IRQHandler(){
    if(EXTI_GetITStatus(EXTI_Line11) == SET){
        timer2_start();
        mark_count = 1;
    }else if((EXTI_GetITStatus(EXTI_Line12) == SET) && (mark_count == 1)){
        mark_time[0] = timer2_get_time();
        mark_count = 2;
    }else if((EXTI_GetITStatus(EXTI_Line13) == SET) && (mark_count == 2)){
        mark_time[1] = timer2_get_time();
        mark_count = 3;
    }else if((EXTI_GetITStatus(EXTI_Line14) == SET) && (mark_count == 3)){
        mark_time[2] = timer2_get_time();
        mark_count = 4;
    }else if((EXTI_GetITStatus(EXTI_Line15) == SET) && (mark_count == 4)){
        mark_time[3] = timer2_get_time();
        mark_count = 0;
        timer2_stop();
        timer2_stop();
        mark_ready = 1;
    }
    
    EXTI_ClearITPendingBit(EXTI_Line11);
    EXTI_ClearITPendingBit(EXTI_Line12);
    EXTI_ClearITPendingBit(EXTI_Line13);
    EXTI_ClearITPendingBit(EXTI_Line14);
    EXTI_ClearITPendingBit(EXTI_Line15);
}

int timer2_counter = 0;
void timer2_callback(void){
    timer2_counter++;
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
