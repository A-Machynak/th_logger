#pragma once

#include "th_logger/logger.h"

namespace Th::Log
{

template <typename OStreamType, bool Mutex>
    requires IsOStreamLike<OStreamType>
Logger<OStreamType, Mutex>::Logger(const std::string & name, std::ostream & stream) noexcept
    : _name(name)
    , _stream(stream)
{
}

template <typename OStreamType, bool Mutex>
    requires IsOStreamLike<OStreamType>
Logger<OStreamType, Mutex>::Logger(const std::string & name, std::ofstream && stream) noexcept
    : _name(name)
    , _stream(std::move(stream))
{
}

template <typename OStreamType, bool Mutex>
    requires IsOStreamLike<OStreamType>
const std::string & Logger<OStreamType, Mutex>::GetName() const noexcept
{
	return _name;
}

template <typename OStreamType, bool Mutex>
    requires IsOStreamLike<OStreamType>
const std::ostream & Logger<OStreamType, Mutex>::GetStream() const noexcept
{
	return _stream;
}

template <typename OStreamType, bool Mutex>
    requires IsOStreamLike<OStreamType>
std::ostream & Logger<OStreamType, Mutex>::GetStream() noexcept
{
	return _stream;
}

template <typename OStreamType, bool Mutex>
    requires IsOStreamLike<OStreamType>
void Logger<OStreamType, Mutex>::SetDefaultLogLevel(LogLevel level) noexcept
{
	_defaultLevel = level;
}

template <typename OStreamType, bool Mutex>
    requires IsOStreamLike<OStreamType>
void Logger<OStreamType, Mutex>::SetForceFlush(bool forceFlush) noexcept
{
	_forceFlush = forceFlush;
}

template <typename OStreamType, bool Mutex>
    requires IsOStreamLike<OStreamType>
template <typename... Args>
void Logger<OStreamType, Mutex>::Log(Args &&... args)
{
	_Log(_defaultLevel, args...);
}

template <typename OStreamType, bool Mutex>
    requires IsOStreamLike<OStreamType>
template <typename... Args>
void Logger<OStreamType, Mutex>::Log(LogLevel level, Args &&... args)
{
	_Log(level, args...);
}

template <typename OStreamType, bool Mutex>
    requires IsOStreamLike<OStreamType>
template <typename InputIt>
void Logger<OStreamType, Mutex>::LogIt(InputIt begin, InputIt end)
{
	_LogIter(_defaultLevel, begin, end);
}

template <typename OStreamType, bool Mutex>
    requires IsOStreamLike<OStreamType>
template <typename InputIt>
void Logger<OStreamType, Mutex>::LogIt(LogLevel level, InputIt begin, InputIt end)
{
	_LogIter(level, begin, end);
}

template <typename OStreamType, bool Mutex>
    requires IsOStreamLike<OStreamType>
template <typename... Args>
void Logger<OStreamType, Mutex>::LogF(std::format_string<Args...> fmt, Args &&... args)
{
	_Log(_defaultLevel, std::format(fmt, std::forward<Args>(args)...));
}

template <typename OStreamType, bool Mutex>
    requires IsOStreamLike<OStreamType>
template <typename... Args>
void Logger<OStreamType, Mutex>::LogF(LogLevel level,
                                      std::format_string<Args...> fmt,
                                      Args &&... args)
{
	_Log(level, std::format(fmt, std::forward<Args>(args)...));
}

template <typename OStreamType, bool Mutex>
    requires IsOStreamLike<OStreamType>
void Logger<OStreamType, Mutex>::LogLoc(LogLevel level, std::source_location src)
{
	_Log(level, std::format("{}:{}@{}", src.file_name(), src.function_name(), src.line()));
}

template <typename OStreamType, bool Mutex>
    requires IsOStreamLike<OStreamType>
template <typename... Args>
void Logger<OStreamType, Mutex>::_Log(LogLevel level, Args &&... args)
{
	std::conditional_t<Mutex, std::lock_guard<std::mutex>, Empty> lock{_mtx};
	// [Time] [Name] [Level]: {User message}
	const auto now = std::chrono::system_clock::now();
	const auto time = std::chrono::time_point_cast<std::chrono::milliseconds>(now);

	// "[Time] [Name] [Level]: "
	_stream << std::format("[{}] [{}] [{}]: ", time, ToString(level), _name);

	// "{User message}"
	(_stream << ... << args) << "\n";

	if (_forceFlush) {
		std::flush(_stream);
	}
}

template <typename OStreamType, bool Mutex>
    requires IsOStreamLike<OStreamType>
template <typename InputIt>
void Logger<OStreamType, Mutex>::_LogIter(LogLevel level, InputIt begin, InputIt end)
{
	std::conditional_t<Mutex, std::lock_guard<std::mutex>, Empty> lock{_mtx};
	// [Time] [Name] [Level]: { vec }
	const auto now = std::chrono::system_clock::now();
	const auto time = std::chrono::time_point_cast<std::chrono::milliseconds>(now);

	// "[Time] [Name] [Level]: {"
	_stream << std::format("[{}] [{}] [{}]: {{ ", time, ToString(level), _name);

	for (; begin != end - 1; ++begin) {
		_stream << *begin << ", ";
	}
	_stream << *begin << " }\n";

	if (_forceFlush) {
		std::flush(_stream);
	}
}

}  // namespace Th::Log
