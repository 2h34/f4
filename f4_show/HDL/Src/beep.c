#include "beep.h"
#include "main.h"

#define BEEP_DURATION_TICKS  50U

static uint8_t beep_active = 0;
static uint32_t beep_start_tick = 0;

void BEEP_ON(void)
{
    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
}

void BEEP_OFF(void)
{
    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
}


void BEEP_Trigger(uint32_t current_tick)
{
    BEEP_ON();

    beep_start_tick = current_tick;
    beep_active = 1;
}

void BEEP_Process(uint32_t current_tick)
{
    if (beep_active && (current_tick - beep_start_tick >= BEEP_DURATION_TICKS))
    {
        BEEP_OFF();
        beep_active = 0;
    }
}