#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

/*
 * Files contains functions for logging game informations.
 */

/*
 * ANSI style formatting constants.
 */
constexpr char ResetStyleAnsi[]{"\033[0m"};
constexpr char GreenColorAnsi[]{"\033[32m"};
constexpr char WhiteColorAnsi[]{"\033[37m"};
constexpr char RedColorAnsi[]{"\033[31m"};

/*
 * Types of possible logging version.
 */
enum class LoggerLevel {
    ALL = 0,
    ERRORS = 1,
    GAME_INFO = 2,
    NONE = 3,
};

/*
 * Current logger version.
 */
constexpr LoggerLevel DEBUG{LoggerLevel::GAME_INFO};

/*
 * Function prints single value to stream.
 */
template<typename T>
constexpr static void logItem(std::ostream &stream, T const &t) {
    stream << t;
}

/*
 * Function prints values to stream.
 */
template<typename T, typename ...Args>
constexpr static void logItem(std::ostream &stream, T const &t, Args &&...args) {
    stream << t;
    logItem(stream, args...);
}

/*
 * Function prints program errors and exit program with status code 1.
 */
template<typename ...Args>
constexpr void fatalError(Args &&...args) {
    logItem(std::cerr, RedColorAnsi, args..., '\n', ResetStyleAnsi);
    exit(EXIT_FAILURE);
}

/*
 * Function logs game errors.
 */
template<typename ...Args>
constexpr void logError(Args &&...args) {
    if constexpr (DEBUG == LoggerLevel::ALL ||
                  DEBUG == LoggerLevel::ERRORS) {
        logItem(std::cout, RedColorAnsi, args..., '\n', ResetStyleAnsi);
    }
}

/*
 * Function logs game important information.
 */
template<typename ...Args>
constexpr void logImportant(Args &&...args) {
    if constexpr (DEBUG == LoggerLevel::ALL ||
                  DEBUG == LoggerLevel::GAME_INFO) {
        logItem(std::cout, GreenColorAnsi, args..., '\n', ResetStyleAnsi);
    }
}

/*
 * Function logs game information.
 */
template<typename ...Args>
constexpr void log(Args &&...args) {
    if constexpr (DEBUG == LoggerLevel::ALL ||
                  DEBUG == LoggerLevel::GAME_INFO) {
        logItem(std::cout, WhiteColorAnsi, args..., '\n', ResetStyleAnsi);
    }
}

#endif //LOGGER_H
