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

	using RCLONE_CPP_EXPORT remote = entity::remote;

	using RCLONE_CPP_EXPORT file = entity::file;

	using RCLONE_CPP_EXPORT about = entity::about;

	using RCLONE_CPP_EXPORT size = entity::size;

	using RCLONE_CPP_EXPORT version = entity::version;
}

namespace ir = iridium::rclone;
using ire = iridium::rclone::entity;