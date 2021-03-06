import os
from conans import ConanFile, CMake, tools


class MeStdConan(ConanFile):
    scm = {
        "type": "git",
        "url" : "auto",
        "subfolder" : "me_std",
        "revision" : "auto",
        "password" : os.environ.get("SCM_SECRET", None)
    }    
    name = "me_std"
    license = "MIT License"
    author = "Markus Eggenbauer markus.eggenbauer@gmail.com"
    url = "https://github.com/markuseggenbauer/me_std.git"
    description = "My (Markus Eggenbauer) personal standard template library"
    topics = ("C++", "library")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    build_requires = "gtest/1.10.0"
    requires = "me_find_package/[~=1]", "me_build/[~=1]", "gtest/1.10.0"
    generators = "cmake_find_package"
    exports_sources = "CMakeLists.txt", "impl/*"

    def set_version(self):
        git = tools.Git(folder=self.recipe_folder)
        self.version = "%s" % (git.get_tag() or git.get_branch())

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

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
