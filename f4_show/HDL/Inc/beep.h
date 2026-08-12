#ifndef __BEEP_H
#define __BEEP_H


#include <stdint.h>




void BEEP_Trigger(uint32_t current_tick,uint16_t count);
void BEEP_Process(uint32_t current_tick);

#endif // __BEEP_H