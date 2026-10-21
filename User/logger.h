#ifndef LOGGER_H_
#define LOGGER_H_

#include "stdint.h"

// Перечисление уровней логирования
typedef enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
} log_level_t;

// Инициализация логгера (создает очередь и задачу вывода)
void Logger_Init(void);

// Основная функция логирования (форматированный вывод)
void Logger_Log(log_level_t level, const char *tag, const char *format, ...);

// Удобные макросы для вызова из кода
#define LOG_D(tag, fmt, ...) Logger_Log(LOG_LEVEL_DEBUG, tag, fmt, ##__VA_ARGS__)
#define LOG_I(tag, fmt, ...) Logger_Log(LOG_LEVEL_INFO,  tag, fmt, ##__VA_ARGS__)
#define LOG_W(tag, fmt, ...) Logger_Log(LOG_LEVEL_WARN,  tag, fmt, ##__VA_ARGS__)
#define LOG_E(tag, fmt, ...) Logger_Log(LOG_LEVEL_ERROR, tag, fmt, ##__VA_ARGS__)

#endif /* LOGGER_H_ */
