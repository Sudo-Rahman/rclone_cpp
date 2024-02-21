#pragma once

#include "entities/about.hpp"
#include "entities/size.hpp"
#include "entities/entitie.hpp"

#ifdef _WIN32
#define RCLONE_CPP_EXPORT __declspec(dllexport)
#else
#define RCLONE_CPP_EXPORT
#endif

namespace iridium::rclone
{

    RCLONE_CPP_EXPORT class entitie;

    RCLONE_CPP_EXPORT class about;

    RCLONE_CPP_EXPORT class size;
}