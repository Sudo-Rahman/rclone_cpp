#pragma once

namespace iridium::rclone
{
	class entitie
	{
		entitie() = default;

		friend class process;

	public:
		class about;

		class size;

		class file;

		class remote;
	};
} // namespace Iridium
