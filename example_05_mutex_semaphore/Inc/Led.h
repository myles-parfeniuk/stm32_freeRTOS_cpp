#pragma once

//cube HAL includes
#include "FreeRTOS.h"
#include "task.h"
#include "gpio.h"

class Led {
public:
	Led(GPIO_TypeDef *gpio_port, uint16_t gpio_num, TaskFunction_t task_fxn);
	void set(bool on);

private:
	TaskHandle_t task_blink_hdl;
	GPIO_TypeDef* gpio_port;
	uint16_t gpio_num;

};
