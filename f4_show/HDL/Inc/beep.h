#ifndef __BEEP_H
#define __BEEP_H


#include <stdint.h>

void beep_init(void);
void beep_on(void);
void beep_off(void);
void BEEP_Trigger(uint32_t current_tick);
void BEEP_Process(uint32_t current_tick);

#endif // __BEEP_H