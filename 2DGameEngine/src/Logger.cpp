#include "Logger.h"
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>

std::string CurrentDateToString() {
	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::string output(30, '\0');
	struct tm timeinfo;
	localtime_s(&timeinfo, &now);
	std::strftime(&output[0], output.size(), "%d-%b-%Y %H:%M:%S", &timeinfo);
	return output;
}

void Logger::Log(const std::string& message) {
	std::string output = "LOG: [" + CurrentDateToString() + "]: " + message;
	std::cout << "\x1B[32m" << output << "\033[0m" << std::endl;
}

void Logger::Err(const std::string& message) {
	std::string output = "ERR: [" + CurrentDateToString() + "]: " + message;
	std::cerr << "\x1B[91m" << output << "\033[0m" << std::endl;
}