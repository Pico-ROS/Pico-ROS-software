cmake_minimum_required(VERSION 3.13)

file(READ ${CMAKE_CURRENT_SOURCE_DIR}/version.txt version)
project(pico-ros VERSION ${version} LANGUAGES C)

#set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(CMAKE_C_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
      ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

set(BUILD_EXAMPLES OFF)
set(BUILD_TESTING OFF)
set(BUILD_TESTING OFF)
set(BUILD_SHARED_LIBS OFF)
# set(PICO_STATIC ON)
add_definitions(-DZENOH_LINUX)
# add_definitions(-DZENOH_LOG_DEBUG)
add_subdirectory(thirdparty/zenoh-pico)



add_library(pros STATIC
    src/pico-ros.c
    src/pico-ros.h
)

target_include_directories(pros PUBLIC
     src/
)
target_link_libraries(pros zenohpico::lib)



##### EXAMPLES
set(UCDR_SUPERBUILD OFF)
set(UCDR_ISOLATED_INSTALL OFF)
set(PROJECT_NAME test)
add_subdirectory(thirdparty/Micro-CDR)

add_executable(srv_add2ints
  examples/srv_add2ints.c
  examples/common.c
)
target_include_directories(srv_add2ints PUBLIC
    src/
    thirdparty/Micro-CDR/include
)
target_link_libraries(srv_add2ints PRIVATE  pros microcdr)


add_executable(talker
  examples/talker.c
  examples/common.c
)
target_include_directories(talker PUBLIC
    src/
    thirdparty/Micro-CDR/include
)
target_link_libraries(talker PRIVATE  pros microcdr)

add_executable(listener
  examples/listener.c
  examples/common.c
)
target_include_directories(listener PUBLIC
    src/
    thirdparty/Micro-CDR/include
)
target_link_libraries(listener PRIVATE  pros microcdr)

