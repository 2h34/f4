#include "can_app.h"
#include "can.h"
#include"beep.h"
#include "led.h"

static CAN_RxHeaderTypeDef can_rx_header;
static uint8_t can_rx_data[8];

static volatile uint8_t beep_request = 0U;
static volatile uint8_t beep_count = 0U;
static uint8_t beep_workmode = 0U;

static volatile uint8_t flow_request = 0U;
static volatile uint8_t flow_target_state = 0U;
volatile led_state_t current_state = STATE_OFF;


void can_app_init(void)
{
    CAN_FilterTypeDef filter = {0};

    filter.FilterBank = 0;
    filter.FilterMode = CAN_FILTERMODE_IDLIST;
    filter.FilterScale = CAN_FILTERSCALE_32BIT;
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter.FilterActivation = ENABLE;
    filter.SlaveStartFilterBank = 14;
    
    uint32_t filter_id1,filter_id2;

    filter_id1 = (0x01020101U << 3) | 0x04U;
    filter_id2 = (0x01020201U << 3) | 0x04U;

    filter.FilterIdHigh = (filter_id1 >> 16) & 0xFFFFU;
    filter.FilterIdLow  = filter_id1 & 0xFFFFU;
    filter.FilterMaskIdHigh = (filter_id2 >> 16) & 0xFFFFU;
    filter.FilterMaskIdLow = filter_id2 & 0xFFFFU;

    if (HAL_CAN_ConfigFilter(&hcan1, &filter) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_CAN_Start(&hcan1) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
    {
        Error_Handler();
    }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan->Instance != CAN1)
    {
        return;
    }
    if (HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&can_rx_header,
                             can_rx_data) != HAL_OK)
    {
        return;
    }
    if ((can_rx_header.IDE == CAN_ID_EXT) &&
    (can_rx_header.RTR == CAN_RTR_DATA) &&
    (can_rx_header.DLC == 1U))
    {
        if (can_rx_header.ExtId == 0x01020101U)
        {
            beep_count = can_rx_data[0];
            beep_request = 1;
        }
    }
    if ((can_rx_header.IDE == CAN_ID_EXT) &&
    (can_rx_header.RTR == CAN_RTR_DATA) &&
    (can_rx_header.DLC == 1U))
    {
        if (can_rx_header.ExtId == 0x01020201U)
        {
            if (can_rx_data[0] == 1)
            {
                /* code */
                current_state = STATE_FLOW;
                flow_request = 1;
                flow_target_state = 1;
            }
            if (can_rx_data[0] == 0)
            {
                /* code */
                current_state = STATE_OFF;
                flow_request = 1;
                flow_target_state=0;
            }
        }
            
    }
}


void can_app_process(uint32_t current_tick)
{
    if (beep_request == 1U)
    {
        /* code */
        BEEP_Trigger(current_tick,beep_count);
        beep_request = 0U;
        beep_workmode = 1;
    }
    if (beep_workmode == 1U && (!beep_is_work()))
    {
        /* code */
        CAN_TxHeaderTypeDef tx_header ={0};
        uint8_t can_tx_data[8];
        uint32_t tx_mailbox;

        can_tx_data[0]='O';can_tx_data[1]='K';
        tx_header.DLC=2U;tx_header.ExtId= 0x02010101U;
        tx_header.IDE = CAN_ID_EXT;
        tx_header.RTR = CAN_RTR_DATA;
        tx_header.TransmitGlobalTime = DISABLE;

         if (HAL_CAN_AddTxMessage(&hcan1,
                             &tx_header,
                             can_tx_data,
                             &tx_mailbox) == HAL_OK)
        {
            beep_workmode = 0U;
        }
    }
    
    if (flow_request == 1U)
    {
        /* code */
        if (flow_target_state == 0U)
        {
            LED_OFF(1);
            LED_OFF(2);
        }
        else if (flow_target_state == 1U)
        {
            LED_FlowEnter(current_tick);
        }
        flow_request = 0;
        CAN_TxHeaderTypeDef tx_header ={0};
        uint8_t can_tx_data[8];
        uint32_t tx_mailbox;

        can_tx_data[0]='O';can_tx_data[1]='K';can_tx_data[2]=(char)flow_target_state;
        tx_header.DLC=3U;tx_header.ExtId= 0x02010201U;
        tx_header.IDE = CAN_ID_EXT;
        tx_header.RTR = CAN_RTR_DATA;
        tx_header.TransmitGlobalTime = DISABLE;

         if(HAL_CAN_AddTxMessage(&hcan1,
                             &tx_header,
                             can_tx_data,
                             &tx_mailbox) != HAL_OK)
        {
            Error_Handler();
        }
    }   
    
}