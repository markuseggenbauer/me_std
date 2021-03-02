import os

from conans import ConanFile, CMake, tools


class MeStdTestConan(ConanFile):
    generators = "cmake_find_package"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def test(self):
        if not tools.cross_building(self):
            self.run(".%spackagetest_example" % os.sep)
