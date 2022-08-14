#pragma once

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>

enum Logtype {
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR
};

struct LogEntry {
	Logtype type;
	std::string message;

};

class Logger {
	public:
		static std::vector<LogEntry> messages;
		static void Log(const std::string& message);
		static void Err(const std::string& message);
};

#endif
