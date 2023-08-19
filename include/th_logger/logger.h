#pragma once

#include <chrono>
#include <concepts>
#include <cstdint>
#include <format>
#include <memory>
#include <mutex>
#include <ostream>
#include <source_location>
#include <type_traits>
#include <vector>

#include "th_logger/log_level.h"
#include "th_logger/log_utils.h"

namespace Th::Log
{

/// @brief Logger for logging.
///
/// Outputs logs in the following format:
/// [time] [log level] [name]: [message]
/// @tparam OStreamType std::basic_ostream or type that satisfies IsOStreamLike<Stream>
/// @tparam Mutex synchronise logging with mutex
template <typename OStreamType, bool Mutex = false>
    requires IsOStreamLike<OStreamType>
class Logger
{
	using StreamInternal =
	    std::conditional_t<std::movable<OStreamType>, OStreamType, OStreamType &>;

public:
	/// @brief Constructor for ostream.
	/// @param name Name of this logger
	/// @param stream Stream associated with this logger.
	explicit Logger(const std::string & name, std::ostream & stream) noexcept;

	/// @brief Move constructor for ofstream.
	/// @param name Name of this logger
	/// @param stream Stream associated with this logger.
	explicit Logger(const std::string & name, std::ofstream && stream) noexcept;

	/// @brief Name getter
	/// @return Logger name
	const std::string & GetName() const noexcept;

	/// @brief Stream getter
	/// @return Stream associated with this logger
	const std::ostream & GetStream() const noexcept;

	/// @brief Non-const stream getter
	/// @return Stream associated with this logger
	std::ostream & GetStream() noexcept;

	/// @brief Set the default log level
	/// @param level log level
	void SetDefaultLogLevel(LogLevel level) noexcept;

	/// @brief Flush after Log message
	/// @param forceFlush true - flush, false - don't flush
	void SetForceFlush(bool forceFlush) noexcept;

	/// @brief Log a message at default level.
	/// @tparam ...Args message types
	/// @param ...args message to print
	template <typename... Args>
	void Log(Args &&... args);

	/// @brief Log a message at specified level.
	/// @tparam ...Args message types
	/// @param level log level
	/// @param ...args message to print
	template <typename... Args>
	void Log(LogLevel level, Args &&... args);

	/// @brief Log a message at default level.
	/// @tparam InputIt iterator
	/// @param begin first element
	/// @param end last element
	template <typename InputIt>
	void LogIt(InputIt begin, InputIt end);

	/// @brief Log a message at specified level.
	/// @tparam InputIt iterator
	/// @param level log level
	/// @param begin first element
	/// @param end last element
	template <typename InputIt>
	void LogIt(LogLevel level, InputIt begin, InputIt end);

	/// @brief Log a message at default level. Uses `std::format()`
	/// @see https://en.cppreference.com/w/cpp/utility/format/format
	/// @tparam ...Args message types
	/// @param fmt format
	/// @param ...args message to print
	template <typename... Args>
	void LogF(std::format_string<Args...> fmt, Args &&... args);

	/// @brief Log a message at specified level. Uses `std::format()`
	/// @see https://en.cppreference.com/w/cpp/utility/format/format
	/// @tparam ...Args message types
	/// @param level log level
	/// @param fmt format
	/// @param ...args message to print
	template <typename... Args>
	void LogF(LogLevel level, std::format_string<Args...> fmt, Args &&... args);

	/// @brief Log current source code location (for debugging purposes).
	/// @param level log level. Default: LogLevel::Debug
	/// @param src source location ( std::source_location::current() )
	void LogLoc(LogLevel level = LogLevel::Debug,
	            std::source_location src = std::source_location::current());

protected:
	/// @brief Logger's name used in the output
	std::string _name;

	/// @brief Associated stream
	StreamInternal _stream;

	/// @brief Default log level
	LogLevel _defaultLevel = LogLevel::Info;

	/// @brief Force flush after each write
	bool _forceFlush = false;

	/// @brief Mutex for locking
	std::conditional<Mutex, std::mutex, void *>::type _mtx;

	/// @brief Logging implementation
	/// @tparam ...Args message types
	/// @param level log level
	/// @param ...args message to print
	template <typename... Args>
	void _Log(LogLevel level, Args &&... args);

	/// @brief Logging implementation for iterable objects
	/// @tparam InputIt iterator
	/// @param level log level
	/// @param begin first element
	/// @param end last element
	template <typename InputIt>
	void _LogIter(LogLevel level, InputIt begin, InputIt end);

public:
	/// @brief Shortcuts for Log(Level, args);

	// clang-format off

	template <typename... Args>
	void Trace(Args &&... args) { Log(LogLevel::Trace, std::forward<Args>(args)...); };
	template <typename InputIt>
	void TraceIt(InputIt begin, InputIt end) { LogIt(LogLevel::Trace, begin, end); };
	template <typename... Args>
	void TraceF(std::format_string<Args...> fmt, Args &&... args) { LogF(LogLevel::Trace, fmt, std::forward<Args>(args)...); };

	template <typename... Args>
	void Debug(Args &&... args) { Log(LogLevel::Debug, std::forward<Args>(args)...); };
	template <typename InputIt>
	void DebugIt(InputIt begin, InputIt end) { LogIt(LogLevel::Debug, begin, end); };
	template <typename... Args>
	void DebugF(std::format_string<Args...> fmt, Args &&... args) { LogF(LogLevel::Debug, fmt, std::forward<Args>(args)...); };

	template <typename... Args>
	void Info(Args &&... args) { Log(LogLevel::Info, std::forward<Args>(args)...); };
	template <typename InputIt>
	void InfoIt(InputIt begin, InputIt end) { LogIt(LogLevel::Info, begin, end); };
	template <typename... Args>
	void InfoF(const std::format_string<Args...> fmt, Args &&... args) { LogF(LogLevel::Info, fmt, std::forward<Args>(args)...); };

	template <typename... Args>
	void Success(Args &&... args) { Log(LogLevel::Success, std::forward<Args>(args)...); };
	template <typename InputIt>
	void SuccessIt(InputIt begin, InputIt end) { LogIt(LogLevel::Success, begin, end); };
	template <typename... Args>
	void SuccessF(std::format_string<Args...> fmt, Args &&... args) { LogF(LogLevel::Success, fmt, std::forward<Args>(args)...); };

	template <typename... Args>
	void Warn(Args &&... args) { Log(LogLevel::Warn, std::forward<Args>(args)...); };
	template <typename InputIt>
	void WarnIt(InputIt begin, InputIt end) { LogIt(LogLevel::Warn, begin, end); };
	template <typename... Args>
	void WarnF(std::format_string<Args...> fmt, Args &&... args) { LogF(LogLevel::Warn, fmt, std::forward<Args>(args)...); };

	template <typename... Args>
	void Error(Args &&... args) { Log(LogLevel::Error, std::forward<Args>(args)...); };
	template <typename InputIt>
	void ErrorIt(InputIt begin, InputIt end) { LogIt(LogLevel::Error, begin, end); };
	template <typename... Args>
	void ErrorF(std::format_string<Args...> fmt, Args &&... args) { LogF(LogLevel::Error, fmt, std::forward<Args>(args)...); };

	template <typename... Args>
	void Fatal(Args &&... args) { Log(LogLevel::Fatal, std::forward<Args>(args)...); };
	template <typename InputIt>
	void FatalIt(InputIt begin, InputIt end) { LogIt(LogLevel::Fatal, begin, end); };
	template <typename... Args>
	void FatalF(std::format_string<Args...> fmt, Args &&... args) { LogF(LogLevel::Fatal, fmt, std::forward<Args>(args)...); };
	// clang-format on
};

}  // namespace Th::Log

#include "th_logger/logger.hpp"
