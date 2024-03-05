#pragma once

#include "parsers/basic_parser.hpp"
#include "parsers/file_parser.hpp"
#include "parsers/remote_parser.hpp"
#include "parsers/about_parser.hpp"
#include "parsers/json_log_parser.hpp"

#ifdef _WIN32
#define RCLONE_CPP_EXPORT __declspec(dllexport)
#else
#define RCLONE_CPP_EXPORT
#endif


namespace iridium::rclone::parser
{
	class RCLONE_CPP_EXPORT file_parser;

	class RCLONE_CPP_EXPORT remote_parser;

	class RCLONE_CPP_EXPORT about_parser;

    class RCLONE_CPP_EXPORT json_log_parser;
} // namespace iridium::rclone::parser
