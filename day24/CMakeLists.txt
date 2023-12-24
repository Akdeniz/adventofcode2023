
get_filename_component(TARGET ${CMAKE_CURRENT_SOURCE_DIR} NAME)
add_executable(${TARGET} main.cpp
        main.cpp)
target_compile_definitions(${TARGET} PRIVATE "-DSRC_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\"")

find_package(Z3 REQUIRED)

message(STATUS "Z3_FOUND: ${Z3_FOUND}")
message(STATUS "Found Z3 ${Z3_VERSION_STRING}")
message(STATUS "Z3_DIR: ${Z3_DIR}")

message(STATUS "Z3_CXX_INCLUDE_DIRS: ${Z3_CXX_INCLUDE_DIRS}")

target_include_directories(${TARGET} PRIVATE ${Z3_CXX_INCLUDE_DIRS})
target_link_libraries(${TARGET} PRIVATE ${Z3_LIBRARIES})
