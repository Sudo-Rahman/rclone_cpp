#pragma once

#include "options/basic_option.hpp"
#include "options/filter.hpp"
#include "options/listing.hpp"
#include "options/performance.hpp"
#include "options/tree.hpp"
#include "options/logging.hpp"

#ifdef _WIN32
#define RCLONE_CPP_EXPORT __declspec(dllexport)
#else
#define RCLONE_CPP_EXPORT
#endif

namespace iridium::rclone::option
{
    class RCLONE_CPP_EXPORT basic_option;

}

namespace ir = iridium::rclone;
namespace iro = iridium::rclone::option;
