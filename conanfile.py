from conans import ConanFile, CMake


class BasicwxWidgets(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = ["wxwidgets/3.1.4@terranum-conan+wxwidgets/stable"]
    generators = "cmake", "gcc", "txt"

    # default_options = {"wxwidgets:shared": True}
    default_options = {"wxwidgets:webview": False}

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")  # From bin to bin
        self.copy("*.dylib*", dst="bin", src="lib")  # From lib to bin

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
