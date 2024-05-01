//AlSch092 @ Github
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <cstdarg>
#include <mutex>

enum LogType
{
    Info,
    Warning,
    Err,
    Detection,
};

class Logger 
{
public:

    static void log(const std::string filename, LogType type, const std::string& message)
    {
        std::ofstream logFile;
        std::string msg_with_errorcode;

        logFile.open(filename.c_str(), std::ios::out | std::ios::app);
        if (!logFile.is_open()) 
        {
            std::cerr << "Error: Could not open log file: " <<  "UltimateAnticheat.log" << std::endl;
            return;
        }

        if (logFile.is_open()) 
        {          
            switch (type)
            {
            case Err:
                msg_with_errorcode = "[ERROR] ";
                break;
            case Detection:
                msg_with_errorcode = "[DETECTION] ";
                break;
            case Info:
                msg_with_errorcode = "[INFO] ";
                break;
            case Warning:
                msg_with_errorcode = "[WARNING] ";
                break;

            default:
                msg_with_errorcode = "[ERROR] ";
                break;
            }

            msg_with_errorcode = msg_with_errorcode + message;

            std::time_t now = std::time(nullptr);
            char timestamp[64];
            std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
            
            logFile << "[" << timestamp << "] " << msg_with_errorcode << std::endl;

            if (logFile.fail()) 
            {
                std::cerr << "[ERROR] Failed to write to log file @ Logger::log." << std::endl;
            }

            logFile.flush();
        }
        else 
        {
            std::cerr << "[ERROR] Log file is not open @ Logger::log." << std::endl;
        }

        printf("%s\n", msg_with_errorcode.c_str());
        logFile.close();
    }

    static void logw(const std::string filename, LogType type, const std::wstring& message) //wide string log
    {
        std::wofstream logFile;
        std::wstring msg_with_errorcode;

        logFile.open(filename.c_str(), std::ios::out | std::ios::app);
        if (!logFile.is_open())
        {
            std::cerr << "Error: Could not open log file: " << "UltimateAnticheat.log" << std::endl;
            return;
        }

        if (logFile.is_open())
        {
            switch (type)
            {
            case Err:
                msg_with_errorcode = L"[ERROR] ";
                break;
            case Detection:
                msg_with_errorcode = L"[DETECTION] ";
                break;
            case Info:
                msg_with_errorcode = L"[INFO] ";
                break;
            case Warning:
                msg_with_errorcode = L"[WARNING] ";
                break;

            default:
                msg_with_errorcode = L"[ERROR] ";
                break;
            }

            msg_with_errorcode = msg_with_errorcode + message;

            std::time_t now = std::time(nullptr);
            wchar_t timestamp[64];
            std::wcsftime(timestamp, sizeof(timestamp), L"%Y-%m-%d %H:%M:%S", std::localtime(&now));

            logFile << L"[" << timestamp << L"] " << msg_with_errorcode << std::endl;

            if (logFile.fail())
            {
                std::cerr << "[ERROR] Failed to write to log file @ Logger::log." << std::endl;
            }

            logFile.flush();
        }
        else
        {
            std::cerr << "[ERROR] Log file is not open @ Logger::log." << std::endl;
        }

        wprintf(L"%s\n", msg_with_errorcode.c_str());
        logFile.close();
    }

    template<typename... Args>
    static void logf(const char* filename, LogType type, const char* format, Args... args)
    {
        if (format == NULL)
            return;

        const int buffSize = 1024;
        char buffer[buffSize] {0};

        int ret = std::snprintf(buffer, buffSize, format, args...);
        if (ret >= 0 && ret < buffSize)
        {
            log(filename, type, std::string(buffer));
        }
        else 
        {
            std::cerr << "Error: Failed to format log message." << std::endl;
        }
    }

    template<typename... Args>
    static void logfw(const char* filename, LogType type, const wchar_t* format, Args... args)
    {
        if (format == NULL)
            return;

        const int buffSize = 1024;
        wchar_t buffer[buffSize]{ 0 };

        int ret = std::snwprintf(buffer, buffSize, format, args...);
        if (ret >= 0 && ret < buffSize)
        {
            log(filename, type, std::string(buffer));
        }
        else
        {
            std::cerr << "Error: Failed to format log message." << std::endl;
        }
    }
};