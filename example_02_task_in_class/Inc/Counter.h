#pragma once
//cube HAL includes
#include "FreeRTOS.h"
#include "task.h"
//example specific includes
#include "print_serial.h"

class Counter {
public:
	Counter();
private:
	TaskHandle_t task_counter_hdl;
	uint16_t count;
	static void counter_task_trampoline(void *pvParameters);
	void counter_task();
};
