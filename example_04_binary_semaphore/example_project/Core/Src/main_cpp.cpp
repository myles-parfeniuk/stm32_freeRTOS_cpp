/*
 *
 */
#include "main_cpp.h"

//cube HAL includes
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
//example specific includes
#include "Led.h"
#include "Counter.h"
#include "print_serial.h"

//GPIO pin info for LED, below is for PC8 (port c, number 8), edit these accordingly
GPIO_TypeDef *GPIO_PORT = GPIOC;
const uint16_t GPIO_NUM = 8;

//create a binary semaphore to control the execution of the led task from the counter task
SemaphoreHandle_t sem_bin_blink = xSemaphoreCreateBinary();

int main_cpp() {

	static Led led(GPIO_PORT, GPIO_NUM, [](void *arg) {
		static bool led_state = true; //whether the led is on or off
		led.set(true); //initially set LED high

		while (1) {

			//take the semaphore and block indefinitely, task will unblock when the counter_task calls xSemaphoreGive
			xSemaphoreTake(sem_bin_blink, portMAX_DELAY);

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
			print_serial("Blink Count: %d \r\n", counter.increment_count());

			//signal the led_task to run by giving the blink semaphore
			xSemaphoreGive(sem_bin_blink);
		}
	});

	/*
	 * Start the task scheduler and begin executing tasks.
	 * The program will not proceed to the return statement below this.
	 */
	vTaskStartScheduler();

	return 0;
}
