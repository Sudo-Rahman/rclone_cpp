#pragma once

#include "options/basic_option.hpp"
#include "options/filter.hpp"
#include "options/listing.hpp"
#include "options/performance.hpp"
#include "options/tree.hpp"

#ifdef _WIN32
#define RCLONE_CPP_EXPORT __declspec(dllexport)
#else
#define RCLONE_CPP_EXPORT
#endif

namespace iridium::rclone::option
{
	class RCLONE_CPP_EXPORT basic_option;
	class RCLONE_CPP_EXPORT filter;
	class RCLONE_CPP_EXPORT listing;
	class RCLONE_CPP_EXPORT performance;
	class RCLONE_CPP_EXPORT tree;
}