
#include "th_logger/log_level.h"

namespace Th::Log
{
const char * ToString(LogLevel level)
{
	switch (level) {
	case LogLevel::Trace:
		return "Trace";
	case LogLevel::Debug:
		return "Debug";
	case LogLevel::Info:
		return "Info";
	case LogLevel::Success:
		return "Success";
	case LogLevel::Warn:
		return "Warn";
	case LogLevel::Error:
		return "Error";
	case LogLevel::Fatal:
		return "Fatal";
	default:
		return "";
	}
}
}  // namespace Th::Log
