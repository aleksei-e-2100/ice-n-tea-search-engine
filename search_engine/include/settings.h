/**
 *  @file  settings.h
 *  @brief iCE && tEA Search Engine module.
 *         Provides constants and settings.
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

inline const std::string appName = "iCE && tEA Search Engine";
inline const std::string appVersion = "1.0";

inline const std::string configFname = "config.json";
inline const std::string requestsFname = "requests.json";
inline const std::string answersFname = "answers.json";

inline constexpr int responsesLimit = 5;

#endif // SETTINGS_H
