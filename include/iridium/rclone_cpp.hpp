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
    RCLONE_CPP_EXPORT class remote;
    RCLONE_CPP_EXPORT class file;
    RCLONE_CPP_EXPORT class process;
}
