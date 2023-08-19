#pragma once

namespace Th::Log
{
/// @brief Log level
enum class LogLevel
{
	Trace = 0,
	Debug = 1,
	Info = 2,
	Success = 3,
	Warn = 4,
	Error = 5,
	Fatal = 6
};

/// @brief LogLevel to string
/// @param level log level
/// @return string
const char * ToString(LogLevel level);
}  // namespace Th::Log
