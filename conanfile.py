from conans import ConanFile, CMake

class BasicwxWidgets(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = ["wxwidgets/3.3.0@terranum-conan+wxwidgets/stable",
                "gtest/1.15.0",
                "protobuf/5.27.0",
                "zlib/1.3.1"]

    generators = "cmake_find_package", "cmake_paths"

    def configure(self):
        if self.settings.os == "Linux":
            self.options["wxwidgets"].png = "system"

    def build_requirements(self):
        self.tool_requires("protobuf/5.27.0")

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")  # From bin to bin
        self.copy("*.dylib*", dst="bin", src="lib")  # From lib to bin

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
