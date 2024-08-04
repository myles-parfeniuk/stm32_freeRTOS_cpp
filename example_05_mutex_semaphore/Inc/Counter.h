#pragma once
//cube HAL includes
#include "FreeRTOS.h"
#include "task.h"

class Counter {
public:
	Counter(TaskFunction_t task_fxn);
	uint16_t increment_count();
private:
	TaskHandle_t task_counter_hdl;
	uint16_t count;
};
