cmake_minimum_required(VERSION 3.17)
project(adventofcode2022)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

file(GLOB subdirectories LIST_DIRECTORIES true "day*")
foreach(dir ${subdirectories})
    IF(IS_DIRECTORY ${dir})
        MESSAGE(STATUS "${dir}")
        add_subdirectory(${dir})
    ENDIF()
endforeach()