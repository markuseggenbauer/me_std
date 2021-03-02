from conans import ConanFile, CMake, tools


class MeStdConan(ConanFile):
    name = "me_std"
    version = "1.0"
    license = "MIT License"
    author = "Markus Eggenbauer markus.eggenbauer@gmail.com"
    url = "https://github.com/markuseggenbauer/me_std"
    description = "My (Markus Eggenbauer) personal standard template library"
    topics = ("C++", "library")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    build_requires = "gtest/1.10.0", "build_api/1.0"
    requires = "build_api/1.0"
    generators = "cmake_find_package"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def source(self):
        git = tools.Git()
        git.clone("https://github.com/markuseggenbauer/me_std.git", "main")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        self.run("cmake --build . --target me_std %s" % cmake.build_config)

    def package(self):
        self.copy("*.hpp", dst="include", src="impl/inc")
        self.copy("*.so*", dst="lib", src="")
        self.copy("*.a*", dst="lib", src="")
        self.copy("*.dll", dst="lib", src="")
        self.copy("*.lib", dst="lib", src="")

    def package_info(self):
        self.cpp_info.libs = ["me_std"]
        self.cpp_info.includedirs = ['include']