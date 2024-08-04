/*
 *
 */
#include "main_cpp.h"

//cube HAL includes
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "usart.h"
//example specific includes
#include "Led.h"
#include "Counter.h"
#include "print_serial.h"


//GPIO pin info for LED, below is for PC8 (port c, number 8), edit these accordingly
GPIO_TypeDef *GPIO_PORT = GPIOC;
const uint16_t GPIO_NUM = 8;

//create a mutex semaphore to make use of usart peripheral mutually exclusive
SemaphoreHandle_t sem_usart_lock = xSemaphoreCreateMutex();

int main_cpp() {

	static Led led(GPIO_PORT, GPIO_NUM, [](void *arg) {
		uint16_t blink_count = 1;
		static bool led_state = true; //whether the led is on or off
		led.set(true); //initially set LED high

		while (1) {

			//delay for 200ms
			vTaskDelay(200/portTICK_PERIOD_MS);

			//take the semaphore and block indefinitely, task will unblock once the usart lock is free
			xSemaphoreTake(sem_usart_lock, portMAX_DELAY);
			print_serial("Blink Task: Blink Count: %d \r\n", blink_count++);
			//give the the lock back after printing
			xSemaphoreGive(sem_usart_lock);

			//toggle led
			led.set(led_state);
			led_state = !led_state;
		}
	});

	static Counter counter( [](void *arg) {

		while (1) {

			//delay for 1000ms
			vTaskDelay(1000 / portTICK_PERIOD_MS);

			//take the semaphore and block indefinitely, task will unblock once the usart lock is free
			xSemaphoreTake(sem_usart_lock, portMAX_DELAY);
			//print the current count and increment it
			print_serial("Counter Task: Count: %d \r\n", counter.increment_count());
			xSemaphoreGive(sem_usart_lock);

		}
	});

	/*
	 * Start the task scheduler and begin executing tasks.
	 * The program will not proceed to the return statement below this.
	 */
	vTaskStartScheduler();

	return 0;
}





