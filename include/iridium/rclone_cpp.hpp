#pragma once

#include "file.hpp"
#include "remote.hpp"
#include "process.hpp"


#ifdef _WIN32
  #define RCLONE_CPP_EXPORT __declspec(dllexport)
#else
  #define RCLONE_CPP_EXPORT
#endif

namespace Iridium::rclone
{
    class RCLONE_CPP_EXPORT remote;
    class RCLONE_CPP_EXPORT file;
    class RCLONE_CPP_EXPORT process;
}
