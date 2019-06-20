

#include "soc_AM335x.h"
#include "beaglebone.h"
#include "gpio_v2.h"

#include "FreeRTOS.h"
#include "portmacro.h"
#include "event_groups.h"
#include "task.h"
#include "TaskLED2.h"
#include "app_MultiSinkLogger.h"
#include "app_InitOSEvents.h"

extern void GPIO1PinMuxSetup(unsigned int pinNo);
extern uint32_t app_MultiSinkLogHookConsole(void * msg);

portTASK_FUNCTION(vLED_blink_XX,pvParameters)
{
	AppLEDBlinkyTaskParams_DSType *pParams = (AppLEDBlinkyTaskParams_DSType *)pvParameters;
	GPIO1PinMuxSetup(pParams->PinNo);
	/* Setting the GPIO pin as an output pin. */
	GPIODirModeSet(SOC_GPIO_1_REGS, pParams->PinNo, GPIO_DIR_OUTPUT);
	for(;;)
	{
		NAV_APP_LOG_PRINTF("Toggling Pin 0x%08X\n",pParams->PinNo);
		GPIOPinWrite(SOC_GPIO_1_REGS, pParams->PinNo, GPIO_PIN_HIGH);
		vTaskDelay( pParams->DelayTicksOn );

		GPIOPinWrite(SOC_GPIO_1_REGS, pParams->PinNo, GPIO_PIN_LOW);
		vTaskDelay( pParams->DelayTicksOff );
	}
}



portTASK_FUNCTION(vLED_blink_evBits,pvParameters)
{
	AppLEDBlinkyTaskParams_DSType *pParams = (AppLEDBlinkyTaskParams_DSType *)pvParameters;
	GPIO1PinMuxSetup(pParams->PinNo);
	uint32_t PreScaleCounter = pParams->PreScaleCounter;
	/* Setting the GPIO pin as an output pin. */
	GPIODirModeSet(SOC_GPIO_1_REGS, pParams->PinNo, GPIO_DIR_OUTPUT);
	for(;;)
	{
		NAV_APP_LOG_PRINTF("Toggling Pin 0x%08X\n",pParams->PinNo);
		EventBits_t ActiveFlags;
        ActiveFlags = xEventGroupWaitBits(pParams->EventGroup,pParams->WaitBits,pdTRUE,pdTRUE,portMAX_DELAY);
        /* Although we have set the Wait Period for indefinite, the following check is just for sanity */
        if((ActiveFlags & pParams->WaitBits) && (0 == PreScaleCounter))
        {   
			uint32_t rv = GPIOPinRead(SOC_GPIO_1_REGS,24);
 			uint32_t write_val = rv ^= GPIO_PIN_HIGH; 
 			GPIOPinWrite(SOC_GPIO_1_REGS, pParams->PinNo, write_val);
			PreScaleCounter = pParams->PreScaleCounter;
            NAV_APP_LOG_PRINTF("Actuate LED ...");
        }
		if(PreScaleCounter)
		{
			PreScaleCounter --;
		}
	}
}

void vLED_blink_evBits_ActivateHook(uint32_t SetReset)
{

	// extern EventGroupHandle_t xApplicationEventGroup_001;
	EventBits_t        ev = 23ul;
	BaseType_t xHigherPriorityTaskWoken, xResult;

    /* xHigherPriorityTaskWoken must be initialised to pdFALSE. */
    xHigherPriorityTaskWoken = pdFALSE;

    /* Set / Reset in xEventGroup. */
    if (pdTRUE == SetReset)
    {
        xResult = xEventGroupSetBitsFromISR(
                                    xApplicationEventGroup_001,   /* The event group being updated. */
                                    ev, /* The bits being set. */
                                    &xHigherPriorityTaskWoken );
    }
    else
    {
        xResult = xEventGroupClearBitsFromISR(
                                    xApplicationEventGroup_001,   /* The event group being updated. */
                                    ev /* The bits being set. */);
        xHigherPriorityTaskWoken = pdFALSE;

    }
}

#if 0

 
void vLED_blink_24 ( void *pvParameters )
{
	GPIO1PinMuxSetup(24);
	GPIODirModeSet(SOC_GPIO_1_REGS, 24, GPIO_DIR_OUTPUT);
	for(;;)
	{
		GPIOPinWrite(SOC_GPIO_1_REGS, 24, GPIO_PIN_HIGH);
		vTaskDelay( 1500 );
		
		GPIOPinWrite(SOC_GPIO_1_REGS, 24, GPIO_PIN_LOW);
		vTaskDelay( 1500 );
	}
}
#endif