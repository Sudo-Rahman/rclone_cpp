#pragma once

#include "about.hpp"

#ifdef _WIN32
#define RCLONE_CPP_EXPORT __declspec(dllexport)
#else
#define RCLONE_CPP_EXPORT
#endif

namespace Iridium::rclone
{
    class RCLONE_CPP_EXPORT about;
}