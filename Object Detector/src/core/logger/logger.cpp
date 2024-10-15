#include "pch.h"
#include "logger.h"

namespace Logger_c
{
    std::ofstream Logger::logFile("\\ObjectDetectorLogs.log");
    std::mutex Logger::logMutex;

    Logger::Logger(const std::string& tag) : tag(tag)
    {
        // Optional: any initialization if needed
    }

    Logger::~Logger()
    {
        writeLog();
    }

    void Logger::writeLog()
    {
        std::lock_guard<std::mutex> lock(logMutex);

        std::string timestamp = getCurrentTime();
        std::string message = buffer.str();

        // Log to console
        std::cout << "[" << timestamp << "] [" << tag << "] " << message << std::endl;

        // Log to file
        if (logFile.is_open())
        {
            logFile << "[" << timestamp << "] [" << tag << "] " << message << std::endl;
            logFile.flush();
        }
        else
        {
            std::cerr << "Error: Log file is not open!" << std::endl;
        }
    }

    std::string Logger::getCurrentTime()
    {
        std::time_t now = std::time(nullptr);
        struct tm localTime;

        localtime_s(&localTime, &now);

        std::ostringstream ss;
        ss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

} // namespace Logger
