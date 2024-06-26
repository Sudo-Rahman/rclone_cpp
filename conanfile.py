from conan import ConanFile
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps

required_conan_version = ">=2.0.0"


class rclone_cppRecipe(ConanFile):
    name = "rclone_cpp"
    version = "0.6.1"
    package_type = "library"

    # Optional metadata
    license = "MIT"
    author = "Rahman YILMAZ"
    url = "<https://github.com/Sudo-Rahman/rclone_cpp>"
    description = "C++ library for rclone"
    topics = ("<rclone>", "<C++>")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*", "include/*"

    @property
    def _min_cppstd(self):
        return 17

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def validate(self):
        if self.info.settings.compiler.get_safe("cppstd"):
            check_min_cppstd(self, self._min_cppstd)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["rclone_cpp"]

    def requirements(self):
        self.requires("boost/[>=1.80.0 <1.86.0]")

    def tools_requires(self):
        self.tool_requires("cmake/[>=3.25]")
