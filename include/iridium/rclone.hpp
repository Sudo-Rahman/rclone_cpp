#pragma once

#include "entities.hpp"
#include "process.hpp"
#include "parsers.hpp"
#include "options.hpp"

namespace ir = iridium::rclone;
namespace ire = iridium::rclone::entities;
namespace irp = iridium::rclone::parser;
namespace iro = iridium::rclone::option;

namespace iridium::rclone
{
	/// Version of the library
	extern const auto *const VERSION = "0.6";
}
