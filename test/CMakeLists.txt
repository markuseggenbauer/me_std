
find_package(GTest)

add_executable(test_me_std
    optional_ref.cpp
)

target_link_libraries(test_me_std 
    PRIVATE 
        me_std
        GTest::gtest
        GTest::gtest_main
)

add_test(test_me_std test_me_std)
