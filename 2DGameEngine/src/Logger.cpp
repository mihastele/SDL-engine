#include "Logger.h"
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <time.h>

std::vector<LogEntry> Logger::messages;

std::string CurrentDateToString() {
	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::string output(30, '\0');
	struct tm timeinfo;
	localtime_s(&timeinfo, &now);
	std::strftime(&output[0], output.size(), "%d-%b-%Y %H:%M:%S", &timeinfo);
	return output;
}

void Logger::Log(const std::string& message) {
	LogEntry logEntry;
	logEntry.type = LOG_INFO;
	logEntry.message = "LOG: [" + CurrentDateToString() + "]: " + message;
	// x1B[32m is a code to turn console text to green and \033[0m to reset the color
	std::cout << "\x1B[32m" << logEntry.message << "\033[0m" << std::endl;

	messages.push_back(logEntry);
}

void Logger::Err(const std::string& message) {
	LogEntry logEntry;
	logEntry.type = LOG_INFO;
	logEntry.message = "ERR: [" + CurrentDateToString() + "]: " + message;
	std::cerr << "\x1B[91m" << logEntry.message << "\033[0m" << std::endl;

	messages.push_back(logEntry);
}