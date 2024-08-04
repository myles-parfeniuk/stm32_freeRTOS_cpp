#include "Led.h"

Led::Led(GPIO_TypeDef *gpio_port, uint16_t gpio_num, TaskFunction_t task_fxn):
task_blink_hdl(NULL), gpio_port(gpio_port), gpio_num(gpio_num)
{

	/*
	 * Create the blink task to toggle gpio.
	 *
	 * This time we are passing the task function as an input argument rather than a member method.
	 */
		xTaskCreate(task_fxn, "Blink Task", 2 * 128, this, 1, &task_blink_hdl);

}

void Led::set(bool on)
{
	HAL_GPIO_WritePin(gpio_port, (1U << 8), on ? GPIO_PIN_RESET : GPIO_PIN_SET); //set pin according to led_state
}

