#pragma once

#include "about.hpp"
#include "size.hpp"

#ifdef _WIN32
#define RCLONE_CPP_EXPORT __declspec(dllexport)
#else
#define RCLONE_CPP_EXPORT
#endif

namespace Iridium::rclone
{
    RCLONE_CPP_EXPORT class  about;
    RCLONE_CPP_EXPORT class size;
}