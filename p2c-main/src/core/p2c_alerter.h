#ifndef _P2C_ALERTER_H_
#define _P2C_ALERTER_H_

#ifndef _ALERT_LEVEL_
#define _ALERT_LEVEL_ 3
#endif

#include <iostream>
#include <fstream> // TODO: log file
#include <sstream>
enum alert_level
{
    DEBUG,
    TRACE,
    INFO,
    WARN,
    ERROR,
    FATAL
};
class p2c_alerter
{
private:
    static void _alerting(alert_level level, std::string message) // TODO:change to buffer type
    {
        if (message.back() != '\n')
            message += '\n';
        switch (level)
        {
        case alert_level::DEBUG:
#if (_ALERT_LEVEL_ <= 0)
            std::cerr << "DEBUG : " << message;
#endif
            break;
        case alert_level::TRACE:
#if (_ALERT_LEVEL_ <= 1)
            std::cerr << "TRACE : " << message;
#endif
            break;
        case alert_level::INFO:
#if (_ALERT_LEVEL_ <= 2)
            std::cerr << "INFO : " << message;
#endif
            break;
        case alert_level::WARN:
#if (_ALERT_LEVEL_ <= 3)
            std::cerr << "WARN : " << message;
#endif
            break;
        case alert_level::ERROR:
#if (_ALERT_LEVEL_ <= 4)
            std::cerr << "ERROR : " << message;
#endif
            exit(1);
            break;
        case alert_level::FATAL:
#if (_ALERT_LEVEL_ <= 5)
            std::cerr << "FATAL : " << message;
#endif
            abort();
            break;
        default:
            break;
        }
    };

public:
    static void alerting(alert_level level) { _alerting(level, ""); };
    static void alerting(alert_level level, std::string message) { _alerting(level, message); };
    template <typename t>
    static void alerting(alert_level level, std::string message, t arg)
    {
        std::stringstream ss;
        ss << arg;
        std::string _str;
        while (ss >> _str)
            message += " " + _str;
        alerting(level, message);
    }
    template <typename t, typename... ts>
    static void alerting(alert_level level, std::string message, t arg, ts... args)
    {
        std::stringstream ss;
        ss << arg;
        std::string _str;
        ss >> _str;
        message += " " + _str;
        alerting(level, message, args...);
    }
};

#endif