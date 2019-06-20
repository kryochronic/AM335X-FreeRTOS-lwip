/* 
 * \file   hal_bspInit.c
 *
 * \brief  Overall Board Init Module for AM335X(CA8 + INTC) Port of FreeRTOS
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

    Copyright (c) 2018 Abhinav Tripathi.
    All Rights Reserved.
*/
#ifndef HAL_BSPINIT_C
#define HAL_BSPINIT_C
/**************************************************************************************************************************/
/*                                                     INCLUDE FILES                                                      */
/**************************************************************************************************************************/
#include <hw_types.h>
#include "soc_AM335x.h"
#include <hw_cm_per.h>
#include <hw_cm_wkup.h>
#include "beaglebone.h"

#include "gpio_v2.h"
#include "hal_bspInit.h"
#include "consoleUtils.h"
#include "interrupt.h"

#include "dmtimer.h"
#include "error.h"


#include "FreeRTOS.h"
#include "task.h"

/**************************************************************************************************************************/
/*                                                     CONFIGURATIONS                                                     */
/**************************************************************************************************************************/


/**************************************************************************************************************************/
/*                                                         MACROS                                                         */
/**************************************************************************************************************************/
#define TIMER_INITIAL_COUNT             (0xffffa261) // 1ms approximate
#define TIMER_RLD_COUNT                 (0xffffa261) // 1ms approximate

/**************************************************************************************************************************/
/*                                                        LITERALS                                                        */
/**************************************************************************************************************************/


/**************************************************************************************************************************/
/*                                                       DATA TYPES                                                       */
/**************************************************************************************************************************/


/**************************************************************************************************************************/
/*                                                  FUNCTION PROTOTYPES                                                   */
/**************************************************************************************************************************/

static void DMTimerAintcConfigure(void);
static void DMTimerSetUp(void);
static void DMTimerIsr(void);
static void GPIOModuleClkConfig(uint32_t x);

/**************************************************************************************************************************/
/*                                                    GLOBAL VARIABLES                                                    */
/**************************************************************************************************************************/

extern volatile uint32_t ulPortYieldRequired;
volatile unsigned int cntValue = 0;
static volatile unsigned int flagIsr = 0;

/**************************************************************************************************************************/
/*                                                          LUTS                                                          */
/**************************************************************************************************************************/


/**************************************************************************************************************************/
/*                                                          CODE                                                          */
/**************************************************************************************************************************/

void configure_platform(void)
{

    /* Initiate MMU and ... Invoke Cache */
    InitMem(); 
    
     /* Initializing the ARM Interrupt Controller. */
    IntAINTCInit();
    
    /* Enable Branch Prediction Shit */
    CP15BranchPredictionEnable();
    
    /* Initialize the UART console */
    ConsoleUtilsInit();

    /* Select the console type based on compile time check */
    ConsoleUtilsSetType(CONSOLE_UART);

    /* This function will enable clocks for the DMTimer2 instance */
    DMTimer2ModuleClkConfig();
    
    /* Register DMTimer2 interrupts on to AINTC */
    DMTimerAintcConfigure();

    /* Perform the necessary configurations for DMTimer */
    DMTimerSetUp();

    

}

void halBspInit(void)   
{
    configure_platform();
    /* Enabling functional clocks for GPIO1 instance. */
    GPIOModuleClkConfig(1);

	/* Enabling the GPIO module. */
	GPIOModuleEnable(SOC_GPIO_1_REGS);

	/* Resetting the GPIO module. */
	GPIOModuleReset(SOC_GPIO_1_REGS);

}


/*
** Do the necessary DMTimer configurations on to AINTC.
*/
static void DMTimerAintcConfigure(void)
{
    /* Registering DMTimerIsr */
    IntRegister(SYS_INT_TINT2, DMTimerIsr);

    /* Set the priority */
    IntPrioritySet(SYS_INT_TINT2,(configMAX_IRQ_PRIORITIES -1), AINTC_HOSTINT_ROUTE_IRQ); /* Lowest Priority */

    /* Enable the system interrupt */
    IntSystemEnable(SYS_INT_TINT2);
}


/*
** Setup the timer for one-shot and compare mode.
** Setup the timer 2 to generate the tick interrupts at the required frequency.
 */
static void DMTimerSetUp(void)
{
    /* Load the counter with the initial count value */
    DMTimerCounterSet(SOC_DMTIMER_2_REGS, TIMER_INITIAL_COUNT);

    /* Load the load register with the reload count value */
    DMTimerReloadSet(SOC_DMTIMER_2_REGS, TIMER_RLD_COUNT);

    /* Configure the DMTimer for Auto-reload and compare mode */
    DMTimerModeConfigure(SOC_DMTIMER_2_REGS, DMTIMER_AUTORLD_NOCMP_ENABLE);
}


/*
** DMTimer interrupt service routine.
*/
extern void FreeRTOS_Tick_Handler( void );
extern void vLED_blink_evBits_ActivateHook(uint32_t SetReset);
static void DMTimerIsr(void)
{
    /* Disable the DMTimer interrupts */
    DMTimerIntDisable(SOC_DMTIMER_2_REGS, DMTIMER_INT_OVF_EN_FLAG);

    /* Clear the status of the interrupt flags */
    DMTimerIntStatusClear(SOC_DMTIMER_2_REGS, DMTIMER_INT_OVF_IT_FLAG);

    FreeRTOS_Tick_Handler();
    /* Application Hook */
    vLED_blink_evBits_ActivateHook(pdTRUE);
    /* Enable the DMTimer interrupts */

    DMTimerIntEnable(SOC_DMTIMER_2_REGS, DMTIMER_INT_OVF_EN_FLAG);
    IntSystemEnable(SYS_INT_TINT2);
}




static void GPIOModuleClkConfig(uint32_t x)
{
    switch(x)
    {
        case 0:
            GPIO0ModuleClkConfig();
        break;
        case 1:
            GPIO1ModuleClkConfig();
        break;
    }
}




void hal_init(void)
{
    

    halBspInit();
}

#endif /* #ifndef HAL_BSPINIT_C */