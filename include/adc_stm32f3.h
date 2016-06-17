#ifndef _ADC_STM32F3_H
#define _ADC_STM32F3_H

#ifdef __cplusplus
extern "C" {
#endif

void adc_init_injected(void);
uint16_t adc_read_data(int channel);

#ifdef __cplusplus
}
#endif

#endif /* _ADC_STM32F3_H */
