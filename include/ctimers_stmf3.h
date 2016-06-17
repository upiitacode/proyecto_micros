#ifndef _CTIMERS_STMF3_H
#define _CTIMERS_STMF3_H

#ifdef __cplusplus
extern "C" {
#endif

	void timer2_init(int time, int use_us);
	void timer2_start();
	void timer2_stop();
	void timer2_callback(void);
	
#ifdef __cplusplus
}
#endif

#endif//_CTIMERS_STMF3_H
