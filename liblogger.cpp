#include "liblogger.h"

// Конструктор для инициализации логгера
Logger::Logger(const std::string& fileName, LogLevel level) : defaultLogLevel(level) {
    logFile.open(fileName, std::ios::app);
    if (!logFile.is_open()) {
        throw std::runtime_error("Unable to open log file");
    }
}

// Деструктор
Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

// Получение текущего времени в строковом формате
std::string Logger::getCurrentTime() {
    std::time_t now = std::time(nullptr);
    char buf[80];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buf);
}

// Установка уровня важности по умолчанию
void Logger::setDefaultLogLevel(LogLevel level) {
    if (defaultLogLevel != level) {
        //получаем текущее время
        std::string timeStr = getCurrentTime();

        // Формируем сообщение об изменении уровня
        std::ostringstream oss;
        oss << "[SYSTEM] Default log level changed from " 
            << static_cast<int>(defaultLogLevel) 
            << " to " << static_cast<int>(level) 
            << " at " << timeStr;

        // Сохраняем сообщение в журнал
        logFile << oss.str() << std::endl;

        // Обновляем уровень важности
        defaultLogLevel = level;
    }
}

// Запись сообщения в журнал
void Logger::log(const std::string& message, LogLevel level) {
    std::lock_guard<std::mutex> lock(mtx);

    if (level < defaultLogLevel) {
        return; // Пропускаем сообщения ниже заданного уровня
    }

    // Определение уровня важности в текстовом формате
    std::string levelStr;
    switch (level) {
        case LogLevel::DEBUG:   levelStr = "DEBUG"; break;
        case LogLevel::INFO:    levelStr = "INFO"; break;
        case LogLevel::WARNING: levelStr = "WARNING"; break;
        case LogLevel::ERROR:   levelStr = "ERROR"; break;
    }

    // Запись сообщения в файл
    logFile << "[" << getCurrentTime() << "] [" << levelStr << "] " << message << std::endl;
}


// Экспортируем функции для использования в динамической библиотеке
extern "C" {
    Logger* create_logger(const char* fileName, int level) {
        return new Logger(fileName, static_cast<LogLevel>(level));
    }

    void destroy_logger(Logger* logger) {
        delete logger;
    }

    void logger_log(Logger* logger, const char* message, int level) {
        logger->log(message, static_cast<LogLevel>(level));
    }

    void logger_set_default_level(Logger* logger, int level) {
        logger->setDefaultLogLevel(static_cast<LogLevel>(level));
    }
}

