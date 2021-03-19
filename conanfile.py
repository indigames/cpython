from conans import ConanFile, CMake


class IgePython37Conan(ConanFile):
    name = "igePython37"
    version = "0.0.1"
    license = "MIT"
    author = "Indi Games"
    url = "https://github.com/indigames/cpython"
    description = "Python 37 customized for IGE Engine"
    topics = ("#Python", "#IGE", "#Games")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    generators = "cmake"
    exports_sources = "*"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

        # Explicit way:
        # self.run('cmake %s/hello %s'
        #          % (self.source_folder, cmake.command_line))
        # self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        self.copy("*.h", dst="include", src="Include")
        if self.settings.os == "Windows":
            self.copy("*.h", dst="include", src="PC")
        elif self.settings.os == "Macos":
            self.copy("*.h", dst="include", src="Mac")
        elif self.settings.os == "Android":
            self.copy("*.h", dst="include", src="Android")
        else:
            self.copy("*.h", dst="include", src="IOS")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["Python37"]
