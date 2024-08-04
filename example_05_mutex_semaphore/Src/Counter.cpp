#include "Counter.h"

Counter::Counter(TaskFunction_t task_fxn):
task_counter_hdl(NULL), count(0){

	/*
	 * Create the counter to increment count and print it over serial.
	 *
	 * This time we are passing the task function as an input argument rather than a member method.
	 */
	xTaskCreate(task_fxn, "Counter Task", 2 * 128, this, 1,
			&task_counter_hdl);
}

uint16_t Counter::increment_count()
{
	count++;
	return count - 1;
}
