#pragma once
#include <chrono>
#include <iostream>
#include <string>
#include <windows.h>
#include <thread>
struct c_logging {
private:
	enum class level
	{
		info,
		success,
		warning,
		error,
		debug
	};
	const char* reset = "\x1b[0m";
	const char* red = "\x1b[31m";
	const char* green = "\x1b[32m";
	const char* yellow = "\x1b[33m";
	const char* blue = "\x1b[34m";
	const char* magenta = "\x1b[35m";
	const char* cyan = "\x1b[36m";

	inline const char* level_color(level lvl)
	{
		switch (lvl)
		{
		case level::info:    return cyan;
		case level::success: return green;
		case level::warning: return yellow;
		case level::error:   return red;
		case level::debug:   return magenta;
		default:             return reset;
		}
	}

	inline const char* level_text(level lvl)
	{
		switch (lvl)
		{
		case level::info:    return "?";
		case level::success: return "+";
		case level::warning: return "-";
		case level::error:   return "!";
		case level::debug:   return ">";
		default:             return ">";
		}
	}

public:
	inline bool init()
	{
		HANDLE h_out = GetStdHandle(STD_OUTPUT_HANDLE);
		if (h_out == INVALID_HANDLE_VALUE)
			return false;

		DWORD mode = 0;
		if (!GetConsoleMode(h_out, &mode))
			return false;

		mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		return SetConsoleMode(h_out, mode);
	}

	inline void timestamp()
	{
		auto now = std::chrono::system_clock::now();
		std::time_t time = std::chrono::system_clock::to_time_t(now);

		tm local_tm{};
		localtime_s(&local_tm, &time);

		printf("[%02d/%02d/%04d] ",
			local_tm.tm_mon + 1,
			local_tm.tm_mday,
			local_tm.tm_year + 1900,
			local_tm.tm_hour,
			local_tm.tm_min,
			local_tm.tm_sec
		);
	}

	template<typename... Args>
	inline void log(level lvl, const char* fmt, Args... args)
	{
		timestamp();
		printf("%s[%s]%s ", level_color(lvl), level_text(lvl), reset);
		printf(fmt, args...);
		printf("\n");
	}

	inline void log(level lvl, const char* msg)
	{
		timestamp();
		printf("%s[%s]%s %s\n", level_color(lvl), level_text(lvl), reset, msg);
	}

	template<typename... Args>
	inline std::string input(const char* fmt, Args... args)
	{
		timestamp();
		printf("%s[INPUT]%s ", blue, reset);
		printf(fmt, args...);
		fflush(stdout);

		std::string s;
		std::getline(std::cin, s);
		return s;
	}

	inline std::string input(const char* msg)
	{
		timestamp();
		printf("%s[INPUT]%s %s", blue, reset, msg);
		fflush(stdout);

		std::string s;
		std::getline(std::cin, s);
		return s;
	}

	template<typename... Args>
	inline void info(const char* fmt, Args... args) { log(level::info, fmt, args...); }

	template<typename... Args>
	inline void success(const char* fmt, Args... args) { log(level::success, fmt, args...); }

	template<typename... Args>
	inline void warning(const char* fmt, Args... args) { log(level::warning, fmt, args...); }

	template<typename... Args>
	inline void error(const char* fmt, Args... args) { log(level::error, fmt, args...); }

	template<typename... Args>
	inline void debug(const char* fmt, Args... args) { log(level::debug, fmt, args...); }
};
