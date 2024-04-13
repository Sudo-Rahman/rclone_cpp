#pragma once

#include <string>
#include "entity.hpp"
#include <cstdint>


namespace iridium::rclone::entities
{
	class about : public entity
	{
	public:
		uint64_t total_bytes{};
		uint64_t used_bytes{};
		uint64_t free_bytes{};
		uint64_t trash_size{};
		uint64_t other{};

		friend auto operator<<(std::ostream& os, const about& about) -> std::ostream&;

		~about() override = default;
	};
} // namespace Iridium::process
