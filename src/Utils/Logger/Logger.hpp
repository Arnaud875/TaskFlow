#pragma once
#include "Static.hpp"
#include "Utils/Singleton.hpp"

namespace Utils {
	class Logger : public Singleton<Logger> {
		friend class Singleton<Logger>;
		Logger() = default;

	public:
		~Logger() = default;
	};
}