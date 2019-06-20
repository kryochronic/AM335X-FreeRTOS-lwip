/* 
 * \file   app_utils.c
 *
 * \brief  Miscellaneous Utilities for the Application
*/
/*
    Author: Abhinav Tripathi <mr dot a dot tripathi at gmail dot com>

    THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
    WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
    SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
    OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
    IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
    OF SUCH DAMAGE.

    Copyright (c) 2019 Abhinav Tripathi.
    All Rights Reserved.
*/
#ifndef APP_UTILS_C
#define APP_UTILS_C
/**************************************************************************************************************************/
/*                                                     INCLUDE FILES                                                      */
/**************************************************************************************************************************/

#include "soc_AM335x.h"
#include "beaglebone.h"
#include "gpio_v2.h"

#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#include "aws_bufferpool.h"
#include "app_MultiSinkLogger.h"
#include "string.h"
#include "aws_bufferpool_config.h"

/* For _exit() */
#include <unistd.h>


/**************************************************************************************************************************/
/*                                                     CONFIGURATIONS                                                     */
/**************************************************************************************************************************/


/**************************************************************************************************************************/
/*                                                         MACROS                                                         */
/**************************************************************************************************************************/


/**************************************************************************************************************************/
/*                                                        LITERALS                                                        */
/**************************************************************************************************************************/


/**************************************************************************************************************************/
/*                                                       DATA TYPES                                                       */
/**************************************************************************************************************************/


/**************************************************************************************************************************/
/*                                                  FUNCTION PROTOTYPES                                                   */
/**************************************************************************************************************************/


/**************************************************************************************************************************/
/*                                                    GLOBAL VARIABLES                                                    */
/**************************************************************************************************************************/


/**************************************************************************************************************************/
/*                                                          LUTS                                                          */
/**************************************************************************************************************************/


/**************************************************************************************************************************/
/*                                                          CODE                                                          */
/**************************************************************************************************************************/
void Delay_HMSM(uint32_t Hours, uint32_t Minutes, uint32_t Seconds, uint32_t Millis)
{
    /* Block for 500ms. */
    TickType_t  xTicksToDelay  = 1; /* Lets do a minimum of 1 Tick Period */
    if(Hours)   xTicksToDelay += Hours   * 3600 * 1000 / portTICK_PERIOD_MS;
    if(Minutes) xTicksToDelay += Minutes *   60 * 1000 / portTICK_PERIOD_MS;
    if(Seconds) xTicksToDelay += Seconds * 1000        / portTICK_PERIOD_MS;
    if(Millis)  xTicksToDelay += Millis                / portTICK_PERIOD_MS;
    
    vTaskDelay( xTicksToDelay );
}


/* _exit - Simple implementation for FreeRTOS Debugging! Does not return.
*/
uint32_t ExitCounter = 1;
void	_exit (int __status)
{
  (void)__status;
  while (ExitCounter)
  {
    Delay_HMSM(0,0,1,0);
    NAV_APP_LOG_PRINTF("\n\r\n\rIn Exit.\n\r");
  }
}



void vApplicationMallocFailedHook(void)
{
	/**
	 * vApplicationMallocFailedHook() will only be called if
	 * configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.
	 * It is a hook function that will get called if a call to
	 * pvPortMalloc() fails. pvPortMalloc() is called internally by
	 * the kernel whenever a task, queue, timer or semaphore is created.
	 * It is also called by various parts of the demo application.
	 * If heap_1.c or heap_2.c are used, then the size of the heap
	 * available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	 * FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can
	 * be used to query the size of free heap space that remains
	 * (although it does not provide information on how the remaining heap
	 * might be fragmented).
	 */
	for (;;) {
    Delay_HMSM(0,0,1,0);
    NAV_APP_LOG_PRINTF("\n\r\n\rIn Function:%s.\n\r",__FUNCTION__);
	}
	taskDISABLE_INTERRUPTS();
}

void vApplicationIdleHook(void)
{
	/**
	 * vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK
	 * is set to 1 in FreeRTOSConfig.h.  It will be called on each iteration
	 * of the idle task.  It is essential that code added to this hook
	 * function never attempts to block in any way (for example, call
	 * xQueueReceive() with a block time specified, or call vTaskDelay()).
	 * If the application makes use of the vTaskDelete() API function
	 * (as this demo application does) then it is also important that
	 * vApplicationIdleHook() is permitted to return to its calling
	 * function, because it is the responsibility of the idle task to
	 * clean up memory allocated by the kernel to any task that has
	 * since been deleted.
	 */
}


void vApplicationStackOverflowHook(TaskHandle_t pxTask,
		signed char *pcTaskName)
{
	(void) pcTaskName;
	(void) pxTask;

	/**
	 * Run time stack overflow checking is performed if
	 * configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.
	 * This hook function is called if a stack overflow is
	 * detected.
	 */
	for (;;) {
    Delay_HMSM(0,0,1,0);
    NAV_APP_LOG_PRINTF("\n\r\n\rIn Function:%s.\n\r",__FUNCTION__);
	}
	taskDISABLE_INTERRUPTS();
}

void vApplicationTickHook(void)
{
	/**
	 * This function will be called by each tick interrupt if
	 * configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.
	 * User code can be added here, but the tick hook is called from
	 * an interrupt context, so code must not attempt to block,
	 * and only the interrupt safe FreeRTOS API
	 * functions can be used (those that end in FromISR()).
	 */
}

void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
static portBASE_TYPE xPrinted = pdFALSE;
volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;

    /* Parameters are not used. */
    ( void ) ulLine;
    ( void ) pcFileName;
    NAV_APP_LOG_PRINTF("\n\r%s %s: %02ld.\n\r",__FUNCTION__,pcFileName,ulLine);

    taskENTER_CRITICAL();
    {
        /* You can step out of this function to debug the assertion by using
        the debugger to set ulSetToNonZeroInDebuggerToContinue to a non-zero
        value. */
        while( ulSetToNonZeroInDebuggerToContinue == 0 )
        {
        }
    }
    taskEXIT_CRITICAL();
}


#endif /* #ifndef APP_UTILS_C */
