#ifndef PLATFORM_C
#define PLATFORM_C

#include <stdint.h>

#include "consoleUtils.h"
#include "soc_AM335x.h"
#include "beaglebone.h"
#include "interrupt.h"
#include "dmtimer.h"
#include "error.h"
#include "FreeRTOS.h"
#include "task.h"

#define portSEI()                                                          \
{                                                                          \
    __asm volatile("PUSH {R0};");                                          \
    __asm volatile("MRS R0, CPSR;");     /* Read the status register */    \
    __asm volatile("BIC R0, R0, #0x40;"); /* Clear the I bit */             \
    __asm volatile("MSR CPSR, R0;");     /*Write it back to enable IRQs */ \
    __asm volatile("POP {R0};");                                           \
}

#define portDI()                                                           \
{                                                                          \
    __asm volatile("PUSH {R0};");                                          \
    __asm volatile("MRS R0, CPSR;");     /* Read the status register */    \
    __asm volatile("ORR R0, R0, #0x40;"); /* Set the I bit */               \
    __asm volatile("MSR CPSR, R0;");     /*Write it back to disable IRQs */\
    __asm volatile("POP {R0};");                                           \
}

extern void IntIrqEnableNewIrqs(void);
extern volatile uint32_t ulCriticalNesting, ulPortInterruptNesting;
extern void (*fnRAMVectors[configMAX_IRQ_VECTORS])(void);


void vSetupTickInterrupt()
{
    // basic initialization is done platform configuration
    // RTOS only enables interrupt
    

    ConsoleUtilsPrintf("Enabling timer interrupt!\r\n");
    DMTimerIntEnable(SOC_DMTIMER_2_REGS, DMTIMER_INT_OVF_EN_FLAG);
    /*ulCriticalNesting = 0UL;*/
}



// void yuiyuiyu(void)
// {
//     /* Set interrupt mask before altering scheduler structures.   The tick
// 	handler runs at the lowest priority, so interrupts cannot already be masked,
// 	so there is no need to save and restore the current mask value.  It is
// 	necessary to turn off interrupts in the CPU itself while the ICCPMR is being
// 	updated. */
// 	portCPU_IRQ_DISABLE();
// 	IntPriorityThresholdSet(
//             (uint32_t)(configMAX_API_CALL_INTERRUPT_PRIORITY));
// 	__asm volatile (	"dsb		\n"
// 						"isb		\n" ::: "memory" );
// 	portCPU_IRQ_ENABLE();

// 	/* Increment the RTOS tick. */
// 	if( xTaskIncrementTick() != pdFALSE )
// 	{
// 		ulPortYieldRequired = pdTRUE;
// 	}

// 	/* Ensure all interrupt priorities are active again. */
// 	portCLEAR_INTERRUPT_MASK();
// 	configCLEAR_TICK_INTERRUPT();
// }
void vApplicationFPUSafeIRQHandler(void)
{
    if(ulPortInterruptNesting >= 20)
    __asm volatile (	"bkpt		\n");



    extern void IntIrqEnableNewIrqs(void);
    extern portBASE_TYPE xInsideISR;

    portBASE_TYPE Local_xInsideISR = xInsideISR;

    
    xInsideISR ++;
    uint32_t IrqPrio     = IntCurrIrqPriorityGet();
    IntPriorityThresholdSet(configMAX_API_CALL_INTERRUPT_PRIORITY);
    uint32_t index = IntActiveIrqNumGet();
    IntSystemDisable(index);
    IntIrqEnableNewIrqs();
    portSEI();
    fnRAMVectors[index]();
    portDI();
    xInsideISR = Local_xInsideISR;
    IntPriorityThresholdSet(portUNMASK_VALUE);
}



#endif /* end of include guard: PLATFORM_C */
