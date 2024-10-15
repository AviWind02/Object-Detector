#pragma once

namespace Logger_c
{
    class Logger
    {
    public:
        Logger(const std::string& tag = "INFO"); // Constructor with default tag
        ~Logger();

        template <typename T>
        Logger& operator<<(const T& message)
        {
            buffer << message;
            return *this;
        }

    private:
        std::string tag;  // The tag for this log entry
        std::ostringstream buffer;
        static std::ofstream logFile;
        static std::mutex logMutex;

        void writeLog();
        std::string getCurrentTime();
    };



}
#define LOG Logger()
#define LOG_TAG(tag) Logger(tag)
