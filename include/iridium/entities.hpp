#pragma once

#include "rclone_about.hpp"

#ifdef _WIN32
#define RCLONE_CPP_EXPORT __declspec(dllexport)
#else
#define RCLONE_CPP_EXPORT
#endif

namespace Iridium
{
    namespace rclone
    {
        class RCLONE_CPP_EXPORT about;
    }
}