cmake_minimum_required(VERSION 3.14)
project(MyLabProject)

set(CMAKE_CXX_STANDARD 17)

find_package(SQLite3 REQUIRED)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
FetchContent_MakeAvailable(googletest)

add_library(app_logic 
    src/date.cpp 
    src/server.cpp 
    src/authenticator.cpp
)
target_include_directories(app_logic PUBLIC include)
target_link_libraries(app_logic PRIVATE SQLite3::SQLite3)

add_executable(main_app src/main.cpp)
target_link_libraries(main_app PRIVATE app_logic)

add_executable(run_tests tests/test_main.cpp)
target_link_libraries(run_tests PRIVATE app_logic GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(run_tests)
