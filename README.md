# rclone_cpp [![Build and Test](https://github.com/Sudo-Rahman/rclone_cpp/actions/workflows/build_and_test.yml/badge.svg)](https://github.com/Sudo-Rahman/rclone_cpp/actions/workflows/build_and_test.yml)

rclone_cpp is a C++ library for rclone. Execute rclone commands, add parser to transform the output of command on object,
add options to the command, etc.

## Features

Most of commands of rclone are supported.

- [x] version
- [x] listremotes
- [x] delete remote
- [x] config
- [x] lsjson
- [x] lsf
- [x] ls
- [x] lsl
- [x] lsd
- [x] lsf
- [x] copyto
- [x] moveto
- [x] delete
- [x] mkdir
- [x] cat
- [x] about
- [x] size
- [x] tree
- [x] bisync
- [x] cleanup
- [x] copyurl
- [x] check

## How to it works

"process" class contains all functions for execute command, get output string, exit code, etc.
You can add options to the command using the "basic_option" class. or create your own option class by inheriting from the "
basic_option" class.
If you want to transform the output of command on object, "parsers" contains many parser for parse the output of
command, but if you want to create your own parser, you can do it by inheriting from the "basic_parser" class.

## Installation

### Dependencies

- [conan](https://conan.io/)
- [cmake](https://cmake.org/)

### Build

```bash
git clone https://github.com/Sudo-Rahman/rclone_cpp.git
cd rclone_cpp
conan install . --build=missing
conan create .
```
After this, the library will be available in your local conan cache.

## Import
You need to use conan to import the library in cmake.

ConanFile.txt
```txt
[requires]
rclone_cpp/[>=0.1.0]

[generators]
CMakeDeps
CMakeToolchain
```

CmakeLists.txt
```cmake
find_package(rclone_cpp)

target_link_libraries(your_target PRIVATE rclone_cpp::rclone_cpp)
```

## Usage

```cpp
#include <iostream>
#include <iridium/rclone.hpp>

auto main() -> int
{
	process::initialize();

	auto rclone = ir::process();

	auto remote = ire::remote::create_shared_ptr("test", ire::remote::remote_type::google_drive, "");
	auto root_file = ire::file{
					nullptr, "/", 0, true, std::chrono::system_clock::now(),
					remote
			};

	process::add_global_option(iro::listing::fast_list());


    auto f_parser = irp::file_parser::ptr(&root_file,
                            [](const ire::file &file)
                            {
                                std::cout << file << std::endl;
                            },   irp::file_parser::json);

	rclone.lsjson(root_file)
			.every_line_parser(f_parser)
			.execute()
			.wait_for_start()
			.wait_for_finish();
			
	return 0;
}
```