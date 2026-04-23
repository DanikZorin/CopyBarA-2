cmake_minimum_required(VERSION 3.15)
project(lab4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Workshops_system.db"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}"
)

find_package(SQLite3 REQUIRED)

include(FetchContent)
FetchContent_Declare(gtest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(gtest)

add_library(src 
    src/authenticator.cpp 
    src/date.cpp 
    src/server.cpp
)

target_include_directories(src PUBLIC include)
target_link_libraries(src PRIVATE SQLite3::SQLite3)

add_executable(lab4 src/main.cpp)
target_link_libraries(lab4 PRIVATE src)

add_custom_command(TARGET lab4 POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/Workshops_system.db" "$<TARGET_FILE_DIR:lab4>/Workshops_system.db" )

enable_testing()
add_executable(lab4test tests/tests.cpp) 
target_link_libraries(lab4test PRIVATE src GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(lab4test)

