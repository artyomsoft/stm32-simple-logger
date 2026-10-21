#include "logger.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "cmsis_os2.h"
#include "stm32h7xx_hal.h"

// Logger settings
#define LOG_QUEUE_LEN      20   // Maximum number of messages in the queue
#define LOG_MAX_STR_LEN    128  // Maximum length of a single message

// External UART handle
extern UART_HandleTypeDef huart3;

// Queue item structure
typedef struct {
    char msg[LOG_MAX_STR_LEN];
} log_item_t;

static QueueHandle_t xLogQueue = NULL;

// String representations of log levels for readable output
static const char* level_strings[] = {
    "[DEBUG]", "[INFO ]", "[WARN ]", "[ERROR]"
};

// Get the current time in microseconds using DWT (for Cortex-M7 / STM32H7)
static uint32_t Logger_GetMicroseconds(void) {
    // SystemCoreClock contains the current core frequency (e.g., 400 or 480 MHz)
    return (uint32_t)(((uint64_t)DWT->CYCCNT * 1000000ULL) / SystemCoreClock);
}

// Low-level output function
static void Logger_Output(const char *str) {
    HAL_UART_Transmit(&huart3, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
}

// FreeRTOS task responsible for outputting log messages
static void vLoggerTask(void *pvParameters) {
    log_item_t item;

    for (;;) {
        if (xQueueReceive(xLogQueue, &item, portMAX_DELAY) == pdTRUE) {
            Logger_Output(item.msg);
        }
    }
}

void Logger_Init(void) {
    SystemCoreClockUpdate();

    // 1. Enable tracing in CoreDebug
    if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)) {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    }

    // 2. Unlock DWT registers (REQUIRED for Cortex-M7 / STM32H7)
    DWT->LAR = 0xC5ACCE55;

    // 3. Reset and enable the cycle counter (CYCCNT)
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    // 4. Create a queue to store log messages
    xLogQueue = xQueueCreate(LOG_QUEUE_LEN, sizeof(log_item_t));

    if (xLogQueue != NULL) {
        // 5. Create the logger task with a low priority
        xTaskCreate(vLoggerTask, "LogTask", configMINIMAL_STACK_SIZE + 1024, NULL, osPriorityHigh, NULL);
    }
}

// Main message formatting function
void Logger_Log(log_level_t level, const char *tag, const char *format, ...) {
    if (xLogQueue == NULL) return;

    log_item_t item;
    int offset = 0;

    // Get the current timestamp in microseconds
    uint32_t timestamp = Logger_GetMicroseconds();

    // 1. Format the prefix: timestamp (us), log level, and tag (field width increased to 10 characters)
    offset = snprintf(item.msg, LOG_MAX_STR_LEN, "[%10lu us] %s [%s]: ", timestamp, level_strings[level], tag);

    if (offset < 0 || offset >= LOG_MAX_STR_LEN) return;

    // 2. Append the user-provided message
    va_list args;
    va_start(args, format);
    int body_len = vsnprintf(item.msg + offset, LOG_MAX_STR_LEN - offset - 2, format, args);
    va_end(args);

    if (body_len > 0) {
        offset += body_len;
    }

    // 3. Append a newline
    strcpy(item.msg + offset, "\r\n");

    // 4. Send the message to the queue
    xQueueSend(xLogQueue, &item, 20);
}