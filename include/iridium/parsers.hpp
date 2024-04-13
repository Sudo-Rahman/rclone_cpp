#pragma once

#include "parsers/basic_parser.hpp"
#include "parsers/file_parser.hpp"
#include "parsers/remote_parser.hpp"
#include "parsers/about_parser.hpp"
#include "parsers/json_log_parser.hpp"
#include "parsers/version_parser.hpp"
#include "parsers/size_parser.hpp"

#ifdef _WIN32
#define RCLONE_CPP_EXPORT __declspec(dllexport)
#else
#define RCLONE_CPP_EXPORT
#endif



namespace ir = iridium::rclone;
namespace irp = iridium::rclone::parser;