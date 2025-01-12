#pragma once
#include "Static.hpp"
#include "Utils/Singleton.hpp"

namespace Database {
	class Database : public Utils::Singleton<Database> {
		friend class Utils::Singleton<Database>;
		Database() = default;

	public:
		~Database() = default;

		bool Connect();
	};
}