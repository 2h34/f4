#ifndef CAN_APP_H
#define CAN_APP_H

#include "stdint.h"

typedef enum
{
  STATE_OFF = 0,
  STATE_FLOW,
  STATE_BREATH
} led_state_t;

void can_app_init(void);
void can_app_process(uint32_t current_tick);

#endif // CAN_APP_H