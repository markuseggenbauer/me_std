me_add_interface_package(pkg_me_std PUBLIC_HEADER_DIR inc PUBLIC_HEADERS optional_ref.hpp)

me_add_packagetest(
    pkg_me_std
    SOURCE_DIR src/me_std
    SOURCES test.optional_ref.cpp
    SOURCE_DEPENDS GTest::gtest
    CONTAINS GTest::gtest_main
)
