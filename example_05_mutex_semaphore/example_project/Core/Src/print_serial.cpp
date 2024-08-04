#include "print_serial.h"

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
		//allocate memory for string
		buffer = new char[buffer_sz + 1]; //+1 for null terminator

		if (buffer != nullptr) {
			vsnprintf(buffer, buffer_sz + 1, format, args);
			HAL_UART_Transmit(&huart2, (const uint8_t*) (buffer), buffer_sz, 10UL);

			//free memory from string buffer
			delete[] buffer;
		}

	}

	va_end(args);
}
