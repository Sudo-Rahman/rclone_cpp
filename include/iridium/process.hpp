#pragma once


#include "process/process.hpp"
#include "process/process_pool.hpp"


#ifdef _WIN32
#define RCLONE_CPP_EXPORT __declspec(dllexport)
#else
#define RCLONE_CPP_EXPORT
#endif

namespace iridium::rclone
{
	class RCLONE_CPP_EXPORT process;

    class RCLONE_CPP_EXPORT process_pool;

}
namespace ir = iridium::rclone;