cmake_minimum_required(VERSION 3.0)
project(SuperJet)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(Source/third-party/fmtlib)
add_subdirectory(Source/third-party/backwandcpp)
add_subdirectory(Source/third-party/zip)

include_directories(
        Source/third-party/zip/src
)

add_subdirectory(Source/AeroJet)
add_subdirectory(Source/Tools)


option(AEROJET_RUN_TESTS "Enable tests" ON)

#if(AEROJET_RUN_TESTS)
#    enable_testing()
#    include_directories(Source/third-party/doctest/doctest)
#    add_subdirectory(Source/Tests)
#endif()

