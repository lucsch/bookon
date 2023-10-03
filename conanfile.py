from conans import ConanFile, CMake


class BasicwxWidgets(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = ["wxwidgets/3.2.2.1@terranum-conan+wxwidgets/stable",
                # "wxwidgets/3.1.5@bincrafters/stable",
                "gtest/1.14.0",
                "protobuf/3.21.12"]

    generators = "cmake", "gcc", "txt"
    def configure(self):
        pass
        if self.settings.os == "Linux":
            self.options["wxwidgets"].png = "system"

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")  # From bin to bin
        self.copy("*.dylib*", dst="bin", src="lib")  # From lib to bin

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
