#pragma once

#include "../entities/version.hpp"
#include "basic_parser.hpp"

namespace iridium::rclone::parser
{
	class version_parser : public basic_parser<entities::version>
	{
	public :
		explicit version_parser(std::function<void(entities::version)> callback)
			: basic_parser(std::move(callback)) {}

		static auto ptr(std::function<void(entities::version)> callback) -> std::shared_ptr<basic_parser>
		{
			return std::make_shared<version_parser>(std::move(callback));
		}

		void parse(const std::string &data) const override;
	};
} // namespace iridium::rclone
