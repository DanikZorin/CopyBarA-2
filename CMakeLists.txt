cmake_minimum_required(VERSION 3.15)
project(lab4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(--coverage -O0)
add_link_options(--coverage)

find_package(SQLite3 REQUIRED)
if(NOT TARGET SQLite3::SQLite3)
    add_library(SQLite3::SQLite3 UNKNOWN IMPORTED)
    set_target_properties(SQLite3::SQLite3 PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${SQLITE3_INCLUDE_DIRS}"
        IMPORTED_LOCATION "${SQLITE3_LIBRARIES}"
    )
endif()

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

enable_testing()
add_executable(lab4test tests/tests.cpp) 
target_link_libraries(lab4test PRIVATE src GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(lab4test)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Workshops_system.db"
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
