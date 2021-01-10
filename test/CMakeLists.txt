find_package(GTest)

me_add_unittest(
  unit_me_std
  SOURCES
  optional_ref.cpp
  SOURCE_DEPENDS
  GTest::gtest
  CONTAINS
  GTest::gtest_main)
