include(FetchContent)

if(NOT TARGET GTest::gtest_main)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

add_executable(TestMatcher TestMatcher.cpp)
target_link_libraries(TestMatcher PRIVATE PupKoCore GTest::gtest_main)
add_test(NAME TestMatcher COMMAND TestMatcher)

add_executable(TestHabit TestHabit.cpp)
target_link_libraries(TestHabit PRIVATE PupKoCore GTest::gtest_main)
add_test(NAME TestHabit COMMAND TestHabit)

add_executable(TestSession TestSession.cpp)
target_link_libraries(TestSession PRIVATE PupKoCore GTest::gtest_main)
add_test(NAME TestSession COMMAND TestSession)
