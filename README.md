# FreeRTOS Logger for STM32

A simple asynchronous logger for STM32 microcontrollers using FreeRTOS and UART.

## Features

- **Asynchronous logging** — application tasks send messages to a FreeRTOS queue, while a dedicated logger task handles UART transmission.
- **Log levels** — DEBUG, INFO, WARN, and ERROR.
- **Timestamps** — microsecond timestamps using the Cortex-M DWT cycle counter.
- **Formatted output** — `printf`-style formatting with support for variable arguments.
- **Thread-safe message submission** — multiple tasks can submit log messages through a shared queue.

## Usage

Initialize the logger after configuring the HAL and before starting the FreeRTOS scheduler:

```c
Logger_Init();
```

Submit log messages from application tasks:

```c
Logger_Log(LOG_LEVEL_INFO, "MAIN", "System started");
Logger_Log(LOG_LEVEL_DEBUG, "SD", "Read %lu bytes", bytes_read);
Logger_Log(LOG_LEVEL_ERROR, "FS", "Error code: %d", error_code);
```

## Configuration

The following constants in `logger.c` control the logger's resources:

```c
#define LOG_QUEUE_LEN      20
#define LOG_MAX_STR_LEN    128
```

- `LOG_QUEUE_LEN` — maximum number of pending log messages.
- `LOG_MAX_STR_LEN` — maximum message buffer size, including the prefix and line ending.

The logger uses `huart3` for output. Change the UART handle in `logger.c` to match your hardware configuration.

## Requirements

- STM32 microcontroller with a configured UART peripheral.
- STM32 HAL.
- FreeRTOS.
- CMSIS-RTOS2 headers.
- Cortex-M7 DWT cycle counter support for timestamps.

## Notes

- Configure the UART baud rate and other parameters using STM32CubeMX or your preferred development environment.
- The logger task runs independently of application tasks, but UART transmission is blocking within the logger task.
- Messages may be lost if the queue is full because the return value of `xQueueSend()` is not checked.
- The DWT cycle counter is finite and wraps around. The timestamp implementation should be validated for the selected core clock and expected uptime.
- Ensure that the logger task priority and stack size are appropriate for your application.

## License

This project is licensed under the **MIT License**.

Third-party components, including **STM32 HAL, CMSIS, and FreeRTOS**, are distributed under their respective licenses.