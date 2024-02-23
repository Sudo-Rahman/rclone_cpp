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

    RCLONE_CPP_EXPORT class entitie;

    RCLONE_CPP_EXPORT class remote;

    RCLONE_CPP_EXPORT class file;

    RCLONE_CPP_EXPORT class about;

    RCLONE_CPP_EXPORT class size;

}