import os

from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout
from conan.tools.build import can_run


class rclone_cppTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("boost/[>=1.80.0]")
        self.requires("rclone_cpp/[>=0.1]")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def layout(self):
        cmake_layout(self)

    def test(self):
        if can_run(self):
            cmd1 = os.path.join(self.cpp.build.bindir, "test")
            cmd2 = os.path.join(self.cpp.build.bindir, "rclone_file_test")
            cmd3 = os.path.join(self.cpp.build.bindir, "rclone_remote_test")
            self.run(cmd1, env="conanrun")
            self.run(cmd2, env="conanrun")
            self.run(cmd3, env="conanrun")
