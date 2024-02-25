#pragma once

#include "entities/about.hpp"
#include "entities/size.hpp"
#include "entities/entitie.hpp"
#include "entities/file.hpp"
#include "entities/remote.hpp"

#ifdef _WIN32
#define RCLONE_CPP_EXPORT __declspec(dllexport)
#else
#define RCLONE_CPP_EXPORT
#endif

namespace iridium::rclone
{
	class RCLONE_CPP_EXPORT entitie;

	class RCLONE_CPP_EXPORT remote;

	class RCLONE_CPP_EXPORT file;

	class RCLONE_CPP_EXPORT about;

	class RCLONE_CPP_EXPORT size;
}
