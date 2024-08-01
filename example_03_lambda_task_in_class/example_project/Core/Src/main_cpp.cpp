/*
 *
 */
#include "main_cpp.h"

//cube HAL includes
#include "FreeRTOS.h"
#include "task.h"
//example specific includes
#include "Led.h"
#include "Counter.h"
#include "print_serial.h"

//GPIO pin info for LED, below is for PC8 (port c, number 8), edit these accordingly
GPIO_TypeDef *GPIO_PORT = GPIOC;
const uint16_t GPIO_NUM = 8;

int main_cpp() {

	/*
	 * Create the objects containing tasks, the calls to xTaskCreate are located within their
	 * constructors.
	 *
	 * This time we pass the task functions as lambda functions.
	 * As a design choice it allows us to keep all our task functions in one file for code readability
	 * while simultaneously still having the ability to abstract functions and variables within a class.
	 */
	static Led led(GPIO_PORT, GPIO_NUM, [](void *arg) {
		static bool led_state = true; //whether the led is on or off
		led.set(true); //initially set LED high

		while (1) {
			//delay for 200ms, portTICK_PERIOD_MS is the duration of 1 tick period in ms
			vTaskDelay(200 / portTICK_PERIOD_MS);

			//toggle led
			led.set(led_state);
			led_state = !led_state;
		}
	});

	static Counter counter( [](void *arg) {

		while (1) {
			//delay for 1000ms
			vTaskDelay(1000 / portTICK_PERIOD_MS);
			//print the current count and increment it
			print_serial("Count: %d \r\n", counter.increment_count());
		}
	});

	/*
	 * Start the task scheduler and begin executing tasks.
	 * The program will not proceed to the return statement below this.
	 */
	vTaskStartScheduler();

	return 0;
}
