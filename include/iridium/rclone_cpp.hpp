#pragma once

#include "rclone_file.hpp"
#include "rclone_remote.hpp"
#include "rclone.hpp"


#ifdef _WIN32
  #define RCLONE_CPP_EXPORT __declspec(dllexport)
#else
  #define RCLONE_CPP_EXPORT
#endif

namespace Iridium
{
    class RCLONE_CPP_EXPORT rclone_file;
    class RCLONE_CPP_EXPORT rclone_remote;
    class RCLONE_CPP_EXPORT rclone;
}
