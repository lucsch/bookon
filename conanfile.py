from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout

class BasicwxWidgets(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = ["wxwidgets/3.3.2",
                "gtest/1.17.0",
                "protobuf/7.35.0",
                "zlib/1.3.2"]

    generators = "CMakeDeps", "CMakeToolchain"
    default_options = {
        "wxwidgets/*:secretstore": False,
    }

    def layout(self):
        cmake_layout(self)

    def build_requirements(self):
        self.tool_requires("protobuf/7.35.0")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
