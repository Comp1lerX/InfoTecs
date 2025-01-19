#ifndef __LIBLOGGER_H__
#define __LIBLOGGER_H__

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <mutex>
#include <sstream>


enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3
};

class Logger {
private:
    std::ofstream logFile;
    LogLevel defaultLogLevel;
    std::mutex mtx; // Для потокобезопасности
    const int MAX_LOG_LEVEL = 3; //максимальный уровень классификации логов

    // Получение текущего времени в строковом формате
    std::string getCurrentTime();

public:
    // Конструктор для инициализации логгера
    Logger(const std::string& fileName, LogLevel level);

    // Деструктор
    ~Logger();

    // Установка уровня важности по умолчанию
    void setDefaultLogLevel(LogLevel level);

    // Запись сообщения в журнал
    void log(const std::string& message, LogLevel level);
};

extern "C" {
    Logger* create_logger(const char* fileName, int level);

    void destroy_logger(Logger* logger);

    void logger_log(Logger* logger, const char* message, int level);

    void logger_set_default_level(Logger* logger, int level);
}

#endif