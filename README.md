# ThLogger
Another (C++20) logging library.

- Provides logging in the following format: `[Date Time] [Log level] [Logger name]: Log Message`
- Simple `lock_guard` to synchronise Log requests to the same logger from multiple threads

## Example usage

```cpp

#include "th_logger/logger.h"

#include <fstream>
#include <iostream>
#include <ostream>

using namespace Th::Log;

Logger<std::ofstream, false> FileLoggerMove(const std::string & fileName, const std::string & loggerName) {
	// Create an ofstream and move it.
	std::ofstream os1(fileName, std::ios::app);
	return Logger<std::ofstream, false>(loggerName, std::move(os1));
}

int main() {
	/// Init
	// Log to file. ofstream "owned" by logger.
	auto fLog = FileLoggerMove("output.log", "FileLoggerMove");

	// Log to cout
	Logger<std::ostream, false> logCout("CoutLogger", std::cout);
	
	// Logger from existing stream. Beware:
	// - Stream is "owned" by fLog and gets destroyed at the end of fLog's lifetime.
	// - Access to the stream isn't synchronized.
	Logger<std::ostream, false> fLog2("FileLogger", fLog.GetStream());

	// Logger with mutex - locks before each write
	//  ! Multiple loggers accessing the same resource aren't synchronized
	Logger<std::ostream, true> logWithMtx("CoutLogger2", std::cout);

	/// Setup
	logCout.SetDefaultLogLevel(LogLevel::Debug); // Default log level
	logCout.SetForceFlush(true);  // Flush after writing everything

	/// Logging
	// Regular:   Log(args) / Log(level, args) / Log(begin, end) / Log(level, begin, end)
	// Formatted: LogF(format, args) / Log(Level, format, args)
	// Location:  LogLoc(level, source_location)

	logCout.Log("Hello", "Log!");                    // HelloLog!
	logCout.Log(LogLevel::Info, "Hello", "Log!");    // HelloLog!
	logCout.LogF("{}: {}, {}", "Formatted", 0, 4.2); // Formatted: 0, 4.2
	logCout.LogLoc();                                // /path/app/main.cpp:int main()@41

	// Shortcuts for log levels (possibly with ["F"/"It"] suffix):
	// Trace/Debug/Info/Success/Warn/Error/Fatal
	std::vector<int> vec{1, 2, 3, 4, 5, 6, 7};

	fLog.Info("Stuff");                      // Stuff
	fLog2.ErrorF("{} + {} != {}", 2, 2, 4);  // 2 + 2 != 4
	logCout.DebugIt(vec.begin(), vec.end()); // { 1, 2, 3, 4, 5, 6, 7 }

	return 0;
}
```

Output:

cout
```
[2023-08-19 14:35:13.551] [Debug] [CoutLogger]: HelloLog!
[2023-08-19 14:35:13.551] [Info] [CoutLogger]: HelloLog!
[2023-08-19 14:35:13.551] [Debug] [CoutLogger]: Formatted: 0, 4.2
[2023-08-19 14:35:13.551] [Debug] [CoutLogger]: /path/app/main.cpp:int main()@41
[2023-08-19 14:35:13.552] [Debug] [CoutLogger]: { 1, 2, 3, 4, 5, 6, 7 }
```

output.log
```
[2023-08-19 14:35:13.552] [Info] [FileLoggerMove]: Stuff
[2023-08-19 14:35:13.552] [Error] [FileLogger]: 2 + 2 != 4
```

## TODO

- Tests/Docs
- Fix/Remove BehavesOStreamLike
- Ideally rewrite the whole thing with sinks.
