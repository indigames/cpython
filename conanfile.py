import os
from pathlib import Path
from conans import ConanFile

class IgeConan(ConanFile):
    name = 'Python'
    version = '3.7.6'
    license = "MIT"
    author = "Indi Games"
    url = "https://github.com/indigames"
    description = name + " library for IGE Engine"
    topics = ("#Python", "#IGE", "#Games")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    generators = "cmake_find_package"
    exports_sources = ""
    requires = []
    short_paths = True

    def requirements(self):
        if (self.settings.os == "Windows") and (self.name != "zlib"):
            self.requires("zlib/1.2.11@ige/test")

    def build(self):
        if self.settings.os == "Windows":
            if self.settings.arch == "x86":
                self.run(f'cmake {self.source_folder} -A Win32')
            else:
                self.run(f'cmake {self.source_folder} -A X64')
        elif self.settings.os == "Android":
            toolchain = Path(os.environ.get("ANDROID_NDK_ROOT")).absolute().as_posix() + '/build/cmake/android.toolchain.cmake'
            if self.settings.arch == "armv7":
                self.run(f'cmake {self.source_folder} -G Ninja -DCMAKE_TOOLCHAIN_FILE={toolchain} -DANDROID_ABI=armeabi-v7a -DANDROID_PLATFORM=android-21 -DCMAKE_BUILD_TYPE=Release')
            else:
                self.run(f'cmake {self.source_folder} -G Ninja -DCMAKE_TOOLCHAIN_FILE={toolchain} -DANDROID_ABI=arm64-v8a -DANDROID_PLATFORM=android-21 -DCMAKE_BUILD_TYPE=Release')
        elif self.settings.os == "iOS":
            toolchain = Path(self.source_folder).absolute().as_posix() + '/cmake/ios.toolchain.cmake'
            self.run(f'cmake {self.source_folder} -G Xcode -DCMAKE_TOOLCHAIN_FILE={toolchain} -DIOS_DEPLOYMENT_TARGET=11.0 -DPLATFORM=OS64 -DCMAKE_BUILD_TYPE=Release')
        elif self.settings.os == "Macos":
            self.run(f'cmake {self.source_folder} -G Xcode -DOSX=1 -DCMAKE_BUILD_TYPE=Release')
        else:
            pass
        self.run('cmake --build . --config Release --target install')
        self._upload()

    def package(self):
        self.copy('*', src='build/install')

    def _upload(self):
        os.chdir(Path(self.build_folder).parent.absolute())
        self.run(f'conan export-pkg . {self.name}/{self.version}@ige/test --profile=cmake/profiles/{self.settings.os}_{self.settings.arch} --force')
        self.run(f'conan upload {self.name}/{self.version}@ige/test --remote ige-center --force --confirm')
