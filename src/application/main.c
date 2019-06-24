#include "stdio.h"
#include "soc_AM335x.h"
#include "beaglebone.h"
#include "gpio_v2.h"
#include "consoleUtils.h"
#include "hal_bspInit.h"

#include "FreeRTOS.h"
#include "task.h"
#include "TaskLED2.h"
#include "app_InitOSEvents.h"
#include "app_MultiSinkLogger.h"
#include "aws_bufferpool.h"
#include "string.h"
#include "enet_wrapper.h"

/* FW Specific Includes here  */
// #include "framework_config.h"
// #include "gor_hal_system_init.h"
// #include "task_manager_api.h"
extern void Entry(void);

/*****************************************************************************
**                INTERNAL MACRO DEFINITIONS

*****************************************************************************/
enet_task_params_s_t enet_task_params_s;

AppMultiSinkLogTaskParam_DSType LogTaskConsoleParams = 
{
	.pInterface = &ConsoleUtilsPrintf,
	// .EntryProgram = &Entry,
};
AppLEDBlinkyTaskParams_DSType Pin23 = {.PinNo = 23ul,.DelayTicksOn =  50ul,.DelayTicksOff = 1450ul, .WaitBits = (1ul << 23ul)};
AppLEDBlinkyTaskParams_DSType Pin24 = {.PinNo = 24ul,.DelayTicksOn = 250ul,.DelayTicksOff = 1250ul, .WaitBits = (1ul << 24ul), .PreScaleCounter = 1000};



void init(void)
{
	halBspInit();
}

uint32_t app_MultiSinkLogHookConsole(void * msg)
{
    return NAV_APP_LOG_PRINTF(msg);
}


int main()
{
	init();
	ConsoleUtilsPrintf("\r\nPlatform initialized.\r\n");
	app_InitOSEvents();
	Pin23.EventGroup = xApplicationEventGroup_001;
	Pin24.EventGroup = xApplicationEventGroup_001;
	LogTaskConsoleParams.p_enet_task_params_s = &enet_task_params_s;
	ConsoleUtilsPrintf("App Events initialized.\r\n");
	BUFFERPOOL_Init();
	ConsoleUtilsPrintf("BufferPool initialized.\r\n");
	xTaskCreate( &vLED_blink_XX, "LED23", pdAPP_TASK_STACK_SIZE_1KW_UL, &Pin23, PRIORITY_TASK_LED_23, NULL );
	ConsoleUtilsPrintf("Blinky:LED%d initialized.\r\n",Pin23.PinNo);
	xTaskCreate( &vLED_blink_XX, "LED24", pdAPP_TASK_STACK_SIZE_1KW_UL, &Pin24, PRIORITY_TASK_LED_24, NULL );
	ConsoleUtilsPrintf("Blinky:LED%d initialized.\r\n",Pin24.PinNo);
	LogTaskConsoleParams.InQ        =  app_InitOSEventsList_DS.pLoggerSinkUartQ;
	InitSerialisedLogPrintf(app_InitOSEventsList_DS.pLoggerSinkUartQ);
	LogTaskConsoleParams.pInterface = &ConsoleUtilsPrintf;
	/* Lets keep the priority of logger high, for debugging for the moment */
	xTaskCreate( &vMultiSinkLoggerConsole_main, "LOG-CON", pdAPP_TASK_STACK_SIZE_1KW_UL, &LogTaskConsoleParams, PRIORITY_LOGGER, NULL );
	NAV_APP_LOG_PRINTF("%s Init ... Enjoy\n","LOG-CON");
	xTaskCreate( &lwip_main, "lwip", pdAPP_TASK_STACK_SIZE_1KW_UL, (void *)&enet_task_params_s, PRIORITY_LWIP, NULL );
	NAV_APP_LOG_PRINTF("%s Init ... Enjoy\n","lwip_main");
#if 1
	/* We want to now start the sceduler */
	vTaskStartScheduler();
#else
	/* We start the scheduler with the framework */
	framework_init();
#endif
	while(pdTRUE);
		
	return 0;
} 


