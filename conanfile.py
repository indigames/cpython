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

    def package(self):
        self.copy("*.h", dst="include", src="Release/include")
        if self.settings.os == "Windows":
            self.copy("*.h", dst="include", src="Release/include/PC")
            self.copy("*.lib", dst="lib", keep_path=False, src="Release/libs/pc/x64")
            self.copy("*.dll", dst="bin", keep_path=False, src="Release/libs/pc/x64")
        elif self.settings.os == "Macos":
            self.copy("*.h", dst="include", src="Release/include/Mac")
            self.copy("*.so", dst="lib", keep_path=False, src="Release/libs/macos/x64")
            self.copy("*.a", dst="bin", keep_path=False, src="Release/libs/macos/x64")
        elif self.settings.os == "Android":
            self.copy("*.h", dst="include", src="Release/include/Android")
            self.copy("*.so", dst="lib", keep_path=False, src="Release/libs/android/arm64-v8a")
            self.copy("*.a", dst="bin", keep_path=False, src="Release/libs/android/arm64-v8a")
        else:
            self.copy("*.h", dst="include", src="Release/include/IOS")
            self.copy("*.so", dst="lib", keep_path=False, src="Release/libs/ios/arm64")
            self.copy("*.a", dst="lib", keep_path=False, src="Release/libs/ios/arm64")

    def package_info(self):
        self.cpp_info.libs = ["Python37"]
