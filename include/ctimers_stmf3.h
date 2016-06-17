#ifndef _CTIMERS_STMF3_H
#define _CTIMERS_STMF3_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	void timer2_init(int time, int use_us);
	void timer2_start(void);
	void timer2_stop(void);
    uint32_t timer2_get_time(void);
	void timer2_callback(void);
	
#ifdef __cplusplus
}
#endif

#endif//_CTIMERS_STMF3_H
