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

//GPIO pin info for LED, below is for PC8 (port c, number 8), edit these accordingly
GPIO_TypeDef* GPIO_PORT = GPIOC;
const uint16_t GPIO_NUM = 8;

int main_cpp() {

	/*
	 * Create the objects containing tasks, the calls to xTaskCreate are located within their
	 * constructors.
	 *
	 * We declare the objects as static so they're stored .bss segment of flash (see linker files and .map file).
	 *
	 * Why?
	 * If declared as "Led led(GPIO_PORT, GPIO_NUM)" it will live on the main stack in RAM.
	 * When vTaskStartScheduler() is called, many ports of freeRTOS reset and reuse the main
	 * stack, leading to the member variables of led becoming corrupted.
	 * Try it and see what happens.
	 *
	 * Alternatively we could declare the object on the heap:
	 * Led *led = new Led(GPIO_PORT, GPIO_NUM);
	 * But generally it's better to keep large or critical structures and objects statically allocated in
	 * flash for safety reasons, as well as to save RAM.
	 */
	static Led led(GPIO_PORT, GPIO_NUM);
	static Counter counter;


	/*
	 * Start the task scheduler and begin executing tasks.
	 * The program will not proceed to the return statement below this.
	 */
	vTaskStartScheduler();

	return 0;
}
