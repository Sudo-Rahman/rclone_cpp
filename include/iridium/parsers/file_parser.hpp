#pragma once

#include "../entities/file.hpp"
#include "basic_parser.hpp"

namespace iridium::rclone::parser
{
	class file_parser : public basic_parser<entities::file>
	{
	public:
		enum parser_type
		{
			json,
			lsl
		};

		explicit file_parser(entities::file *parent,
		                     std::function<void(entities::file)> callback, parser_type type = json)
			: basic_parser(std::move(callback)), _parent(parent), _type(type) {}

		static auto ptr(entities::file *parent, std::function<void(entities::file)> callback, parser_type type = json)
			-> std::shared_ptr<file_parser> { return std::make_shared<file_parser>(parent, std::move(callback), type); }

		void parse(const std::string &data) const override;

	private:
		entities::file *_parent;
		parser_type _type;

		auto json_parse(const std::string &data) const -> void;

		auto lsl_parse(const std::string &data) const -> void;
	};
} // namespace iridium::rclone
