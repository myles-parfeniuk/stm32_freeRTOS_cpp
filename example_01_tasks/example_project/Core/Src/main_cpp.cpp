/*
 *
 */
#include "main_cpp.h"

//standard librares
#include <stdio.h>
#include <stdarg.h>
#include <cstring>
#include <new>
//cube HAL includes
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "gpio.h"

//GPIO pin info for LED, below is for PC8 (port c, number 8), edit these accordingly
static GPIO_TypeDef* GPIO_PORT = GPIOC;
static const uint16_t GPIO_NUM = 8;

/*
 * Handles:
 *
 * In this example they are not used but present to demonstrate how they would be passed to xTaskCreate.
 * Later examples will show how to use them to pause, resume, delete, and perform other operations with tasks.
 */
TaskHandle_t task_led_hdl;
TaskHandle_t task_counter_hdl;

/*
 * Task Function Prototypes:
 *
 * Note pvParameters is required in the function prototype.
 * It is used to pass a parameter at task launch. In this case we are not using it,
 * so we will pass NULL as the pvParameters argument of xTaskCreate.
 */
void task_led(void *pvParameters);
void task_counter(void *pvParameters);

//function for setting external led on gpio
void set_led(bool on);
//printf style function for printing strings over serial
void print_serial(const char *format, ...);

int main_cpp() {
	/*
	 * Create two tasks, one blinks an LED via GPIO toggling, the other prints a count over serial.
	 *
	 * Parameters:
	 * 1st  = pointer to task fxn
	 * 2nd  = task name
	 * 3rd  = task stack size in words (stm32 is 32 bit, so, 128words * 32bits / 8bits = 512 bytes, or 0.5kb)
	 * 4th  = task fxn input argument, not used here therefore NULL
	 * 5th = task priority, in this case the priorities are equal so the tasks will execute in a "round robin fashion"
	 * 6th = pointer to task handle, note this could be passed as NULL if the handles were not used like in this example
	 *
	 */
	xTaskCreate(task_led, "Task LED", 128, NULL, 1, &task_led_hdl);
	xTaskCreate(task_counter, "Task Counter", 128, NULL, 1, &task_counter_hdl);

	/*
	 * Start the task scheduler and begin executing tasks.
	 * The program will not proceed to the return statement below this.
	 */
	vTaskStartScheduler();

	return 0;
}

/*
 * task_led:
 *
 * Delays for 200ms and toggles an LED.
 */
void task_led(void *pvParameters) {

	static bool led_state = true; //whether the led is on or off
	set_led(true); //initially set LED high

	while (1) {
		//delay for 200ms, portTICK_PERIOD_MS is the duration of 1 tick period in ms
		vTaskDelay(200 / portTICK_PERIOD_MS);

		//toggle led
		set_led(led_state);
		led_state = !led_state;
	}
}

/*
 * task_counter:
 *
 * Delays for 1000ms and prints a count over serial while simotaneously incrementing it.
 */
void task_counter(void *pvParameters) {
	uint16_t count = 0;

	while (1) {
		//delay for 1000ms
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		//print the current count and increment it
		print_serial("Count: %d \r\n", count++);
	}
}

void set_led(bool on) {
	HAL_GPIO_WritePin(GPIO_PORT, (1U << GPIO_NUM), on ? GPIO_PIN_RESET : GPIO_PIN_SET); //set pin according to led_state
}

void print_serial(const char *format, ...) {
	va_list args;
	va_list args_copy;
	int buffer_sz;
	char *buffer = nullptr;

	//variable argument list parsing
	va_start(args, format);

	va_copy(args_copy, args);
	buffer_sz = vsnprintf(nullptr, 0, format, args_copy);
	va_end(args_copy);

	if (buffer_sz >= 0) {
		//allocate memory for string, don't throw exception, return nullptr on failure
		buffer = new (std::nothrow) char[buffer_sz + 1]; //+1 for null terminator

		if (buffer != nullptr) {
			vsnprintf(buffer, buffer_sz + 1, format, args);
			HAL_UART_Transmit(&huart2, (const uint8_t*) (buffer), buffer_sz, 1);

			//free memory from string buffer
			delete[] buffer;
		}

	}

	va_end(args);
}
