#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
#include "liblogger.h" // Заголовочный файл для библиотеки

// Очередь сообщений для потокобезопасной обработки
struct LogMessage {
    std::string message;
    LogLevel level;
};

std::queue<LogMessage> logQueue;
std::mutex queueMutex;
std::condition_variable condition;
bool isRunning = true;

// Поток для обработки логов
void logWorker(Logger* logger) {
    while (isRunning || !logQueue.empty()) {
        std::unique_lock<std::mutex> lock(queueMutex);
        condition.wait(lock, [] { return !logQueue.empty() || !isRunning; });

        while (!logQueue.empty()) {
            LogMessage logMessage = logQueue.front();
            logQueue.pop();
            lock.unlock();

            // Записываем сообщение в журнал
            logger->log(logMessage.message, logMessage.level);

            lock.lock();
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <log_file> <default_log_level>" << std::endl;
        return 1;
    }

    std::string logFileName = argv[1];
    LogLevel defaultLogLevel = static_cast<LogLevel>(std::stoi(argv[2]));

    try {
        // Инициализация логгера
        Logger logger(logFileName, defaultLogLevel);

        // Запуск потока для записи логов
        std::thread workerThread(logWorker, &logger);

        std::cout << "Enter log messages (format: <level> <message>), 'setlevel <level>' to change default level, or 'exit' to quit:" << std::endl;
        std::string input;

        while (true) {
            std::getline(std::cin, input);
            if (input == "exit") {
                break;
            }

            // Проверка команды "setlevel"
            if (input.rfind("setlevel ", 0) == 0) {
                try {
                    int newLevel = std::stoi(input.substr(9));
                    logger_set_default_level(&logger, newLevel); // Вызываем метод библиотеки
                    std::cout << "Default log level set to: " << newLevel << std::endl;
                } catch (...) {
                    std::cerr << "Invalid level. Use: setlevel <level>" << std::endl;
                }
                continue;
            }

            // Разбор уровня важности и сообщения
            size_t spacePos = input.find(' ');
            if (spacePos == std::string::npos) {
                std::cerr << "Invalid input format. Use: <level> <message>" << std::endl;
                continue;
            }

            int level = std::stoi(input.substr(0, spacePos));
            std::string message = input.substr(spacePos + 1);

            // Добавление сообщения в очередь
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                logQueue.push({message, static_cast<LogLevel>(level)});
            }
            condition.notify_one();
        }


        // Завершение работы
        isRunning = false;
        condition.notify_all();
        workerThread.join();

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}