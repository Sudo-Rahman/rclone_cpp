import os

from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout
from conan.tools.build import can_run


class rclone_cppTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("boost/[>=1.80.0 <1.86.0]")
        self.requires("rclone_cpp/[0.6]")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def layout(self):
        cmake_layout(self)

    def test(self):
        if can_run(self):
            base_cmd = os.path.join(self.cpp.build.bindir)
            tests = [
                "rclone_file_test -l all",
                "rclone_remote_test -l all",
                "rclone_json_log_test -l all",
                "rclone_process_test -l all",
                "rclone_process_pool_test -l all"
            ]
            for test in tests:
                cmd = f"{os.path.join(base_cmd, test)}"
                self.run(cmd, env="conanrun")
