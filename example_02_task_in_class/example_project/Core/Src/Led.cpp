#include "Led.h"

Led::Led(GPIO_TypeDef *gpio_port, uint16_t gpio_num):
task_blink_hdl(NULL), gpio_port(gpio_port), gpio_num(gpio_num)
{

	/*
	 * Create the blink task to toggle gpio.
	 *
	 * Note that this time the 4th parameter in the xTaskCreate call (task function input argument) is not NULL,
	 * instead we are passing a pointer to the object this constructor is creating.
	 *
	 * This pointer will be used to launch the non-static blink_task method (see blink_task_trampoline)
	 */
		xTaskCreate(blink_task_trampoline, "Blink Task", 128, this, 1, &task_blink_hdl);

}

void Led::set(bool on)
{
	HAL_GPIO_WritePin(gpio_port, (1U << 8), on ? GPIO_PIN_RESET : GPIO_PIN_SET); //set pin according to led_state
}

/*
 * This function is explicitly declared as static as that is a requirement as per xTaskCreate.
 *
 * If we pass xTaskCreate a pointer to blink_task directly, it will fail at compile time since
 * blink_task is a non-static member function which has a implicit 'this' pointer (ie it's called off an object).
 *
 * To get around having to write the task from a static context (ie no access to non-static members like set()),
 * we use this function launch into the non-static blink_task via a pointer to the object passed to xTaskCreate.
 */
void Led::blink_task_trampoline(void *pvParameters)
{
	//recast void pointer to led object pointer
	Led *this_led = (Led *)pvParameters;

	//launch the blink task
	this_led->blink_task();
}


void Led::blink_task()
{
	static bool led_state = true; //whether the led is on or off
	set(true); //initially set LED high

	while (1) {
		//delay for 200ms, portTICK_PERIOD_MS is the duration of 1 tick period in ms
		vTaskDelay(200 / portTICK_PERIOD_MS);

		//toggle led
		set(led_state);
		led_state = !led_state;
	}
}
