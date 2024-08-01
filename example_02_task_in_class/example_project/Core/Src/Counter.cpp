#include "Counter.h"

Counter::Counter():
task_counter_hdl(NULL), count(0){
	/*
	 * Create the counter to increment count and print it over serial.
	 *
	 * Note that this time the 4th parameter in the xTaskCreate call (task function input argument) is not NULL,
	 * instead we are passing a pointer to the object this constructor is creating.
	 *
	 * This pointer will be used to launch the non-static counter_task method (see counter_task_trampoline)
	 */
	xTaskCreate(counter_task_trampoline, "Counter Task", 128, this, 1,
			&task_counter_hdl);
}

/*
 * This function is explicitly declared as static as that is a requirement as per xTaskCreate.
 *
 * If we pass xTaskCreate a pointer to counter_task directly, it will fail at compile time since
 * counter_task is a non-static member function which has a implicit 'this' pointer (ie it's called off an object).
 *
 * To get around having to write the task from a static context (inconvenient because no access to non-static
 * members like print_count()), we use this function launch into the non-static counter_task via a pointer to the object passed to xTaskCreate.
 */
void Counter::counter_task_trampoline(void *pvParameters) {
	//recast void pointer to led object pointer
	Counter *this_counter = (Counter*) pvParameters;

	//launch the counter task
	this_counter->counter_task();
}

void Counter::counter_task() {

	while (1) {
		//delay for 1000ms
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		//print the current count and increment it
		print_serial("Count: %d \r\n", count++);
	}
}
