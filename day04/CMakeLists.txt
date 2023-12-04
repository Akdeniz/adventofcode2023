
get_filename_component(TARGET ${CMAKE_CURRENT_SOURCE_DIR} NAME)
add_executable(${TARGET} main.cpp)
target_compile_definitions(${TARGET} PRIVATE "-DSRC_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\"")