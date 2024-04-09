#pragma once

#include "entities/about.hpp"
#include "entities/size.hpp"
#include "entities/entity.hpp"
#include "entities/file.hpp"
#include "entities/remote.hpp"
#include "entities/version.hpp"

#ifdef _WIN32
#define RCLONE_CPP_EXPORT __declspec(dllexport)
#else
#define RCLONE_CPP_EXPORT
#endif

namespace iridium::rclone
{
	class RCLONE_CPP_EXPORT entity;

	namespace entities
	{
		using RCLONE_CPP_EXPORT remote = remote;

		using RCLONE_CPP_EXPORT file = file;

		using RCLONE_CPP_EXPORT about = about;

		using RCLONE_CPP_EXPORT size = size;

		using RCLONE_CPP_EXPORT version = version;
	}
}

namespace ir = iridium::rclone;
namespace ire = iridium::rclone::entities;