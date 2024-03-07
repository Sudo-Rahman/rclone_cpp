# rclone_cpp

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

"process" class contains all function for execute, get output string, exit code, etc.
You can add options to the command using the "option" class. or create your own option class by inheriting from the "
basic_option" class.
If you want to transform the output of command on object, "parsers" contains many parser for parse the output of
command, but if you want to create your own parser, you can do it by inheriting from the "basic_parser" class.

## Installation

### Dependencies

- [rclone](https://rclone.org/downloads/)
- [conan](https://conan.io/)
- [cmake](https://cmake.org/)

### Build

```bash
git clone https://github.com/Sudo-Rahman/rclone_cpp.git

cd rclone_cpp

conan create .
```
After this, the library will be available in your local conan cache.

## Import
You need to use conan to import the library in cmake.

ConanFile.txt
```txt
[requires]
boost/[>=1.80.0]
rclone_cpp/[>=0.1.0]

[generators]
CMakeDeps
CMakeToolchain
```

CmakeLists.txt
```cmake
find_package(rclone_cpp)
find_package(Boost)

target_link_libraries(your_target PRIVATE rclone_cpp::rclone_cpp boost::boost)
```

## Usage

```cpp
#include <iostream>
#include <iridium/entities.hpp>
#include <iridium/parsers.hpp>
#include <iridium/process.hpp>

using namespace boost::asio;
using namespace std;
using namespace iridium::rclone;

auto main() -> int
{
	process::initialize();

    auto rclone = new process();
	
    auto remote = entity::remote::create_shared_ptr(
		"test", entity::remote::remote_type::google_drive, "");
	auto root_file = entity::file{
					nullptr, "/", 0, true, boost::posix_time::second_clock::local_time(),
					remote
			};
			
	process::add_global_option(option::listing::fast_list());


	auto f_parser = parser::file_parser::create(
	new parser::file_parser(&root_file,
                [](const entity::file&file)
                {
                     std::cout << file << std::endl;
                }));

    rclone->lsjson(file)
    .every_line_parser(f_parser)
    .execute()
    .wait_for_start()
    .wait_for_finish();

    return 0;
}
```