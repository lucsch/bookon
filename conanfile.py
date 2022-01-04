from conans import ConanFile, CMake


class BasicwxWidgets(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = ["wxwidgets/3.1.4@terranum-conan+wxwidgets/stable",
                # "wxwidgets/3.1.5@bincrafters/stable",
                "gtest/1.11.0",
                "protobuf/3.17.1"]

    generators = "cmake", "gcc", "txt"

    def configure(self):
        if self.settings.os == "Linux":
            self.options["wxwidgets"].webview = False # webview control isn't available on linux.

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")  # From bin to bin
        self.copy("*.dylib*", dst="bin", src="lib")  # From lib to bin

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
