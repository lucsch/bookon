from conans import ConanFile, CMake

class BasicwxWidgets(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = ["wxwidgets/3.2.5@terranum-conan+wxwidgets/stable",
                "gtest/1.14.0",
                "protobuf/3.21.9",
                "zlib/1.3"]

    generators = "cmake", "gcc", "txt"
    def configure(self):
        if self.settings.os == "Linux":
            self.options["wxwidgets"].png = "system"

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")  # From bin to bin
        self.copy("*.dylib*", dst="bin", src="lib")  # From lib to bin

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
