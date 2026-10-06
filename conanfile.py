from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout

class BookonRecipe(ConanFile):
    name = "bookon"
    version = "1.3"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"
    default_options = {
        "wxwidgets/*:secretstore": False,
    }

    def requirements(self):
        self.requires("wxwidgets/3.3.3")
        self.requires("gtest/1.18.0")
        self.requires("protobuf/7.35.0")
        self.requires("zlib/1.3.2")
        if self.settings.os == "Linux":
            # conflict between wxwidgets 3.3.2 and libtiff/4.6.0
            self.requires("libwebp/1.6.0", override=True)

    def layout(self):
        cmake_layout(self)

    def build_requirements(self):
        self.tool_requires("protobuf/7.35.0")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
