#include <FreeRTOS.h>
#include <event_groups.h>

typedef struct AppLEDBlinkyTaskParams_DSType_
{
	uint32_t PinNo;
	uint32_t DelayTicksOn;
	uint32_t DelayTicksOff;
	EventGroupHandle_t EventGroup;
	EventBits_t WaitBits;
	uint32_t PreScaleCounter;
} AppLEDBlinkyTaskParams_DSType;

void vLED_blink_XX ( void *pvParameters );
void vLED_blink_evBits ( void *pvParameters );

void vLED_blink_evBits_ActivateHook(uint32_t SetReset);