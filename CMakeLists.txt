cmake_minimum_required(VERSION 3.13)

project(cpplox)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_TESTS "Enable unit tests" OFF)

if (ENABLE_TESTS) # Add Google Test
include(FetchContent)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-1.12.1 # Use a stable release version
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif ()

file(GLOB_RECURSE INTERPRETER_SOURCES src/*.cpp include/*.h)
list(REMOVE_ITEM INTERPRETER_SOURCES src/main.cpp)

add_library(interpreter_lib ${INTERPRETER_SOURCES})
target_include_directories(interpreter_lib PUBLIC include)
target_include_directories(interpreter_lib PUBLIC external)

add_executable(interpreter src/main.cpp)
target_link_libraries(interpreter PRIVATE interpreter_lib)

if (ENABLE_TESTS) # Tests
enable_testing()
add_executable(interpreter_tests
        tests/parser_test.cpp
)
target_link_libraries(interpreter_tests
        PRIVATE
        interpreter_lib
        gtest_main
)
include(GoogleTest)
gtest_discover_tests(interpreter_tests
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tests)
endif()
