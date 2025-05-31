cmake_minimum_required(VERSION 3.13)

file(READ ${CMAKE_CURRENT_SOURCE_DIR}/version.txt version)
project(pico-ros VERSION ${version} LANGUAGES C)

option(PICOROS_BUILD_EXAMPLES "Build examples" ON)
option(PICOROS_BUILD_SERDES "Build picoserdes" ON)
message("PICOROS_BUILD_EXAMPLES: ${PICOROS_BUILD_EXAMPLES}")

if(PICOROS_BUILD_EXAMPLES AND NOT PICOROS_BUILD_SERDES)
error("PICOROS_BUILD_EXAMPLES needs PICOROS_BUILD_SERDES")
endif()
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

# add_definitions(-DZENOH_LOG_DEBUG)
add_subdirectory(thirdparty/zenoh-pico)

if(PICOROS_BUILD_EXAMPLES OR PICOROS_BUILD_SERDES)
  set(UCDR_SUPERBUILD OFF)
  set(UCDR_ISOLATED_INSTALL OFF)
  add_subdirectory(thirdparty/Micro-CDR)
endif()

add_library(picoros STATIC
    src/picoros.c
    src/picoros.h
)
target_include_directories(picoros PUBLIC
     src/
)

add_library(picoserdes STATIC
    src/picoserdes.c
    src/picoserdes.h
)

target_include_directories(picoserdes PUBLIC
     src/
)

target_link_libraries(picoros zenohpico::lib)
target_link_libraries(picoserdes microcdr)


##### EXAMPLES
if(PICOROS_BUILD_EXAMPLES)
  set(EXAMPLE_LIBS
            picoros
            microcdr
  )
  set(EXAMPLE_SRC
            examples/common.c
  )
  set(EXAMPLE_INCLUDE
            src/
            thirdparty/Micro-CDR/include
  )

  add_executable(srv_add2ints examples/srv_add2ints.c ${EXAMPLE_SRC})
  target_include_directories(srv_add2ints PUBLIC ${EXAMPLE_INCLUDE})
  target_link_libraries(srv_add2ints PRIVATE  ${EXAMPLE_LIBS})

  add_executable(talker examples/talker.c  ${EXAMPLE_SRC})
  target_include_directories(talker PUBLIC ${EXAMPLE_INCLUDE})
  target_link_libraries(talker PRIVATE  ${EXAMPLE_LIBS})

  add_executable(listener examples/listener.c  ${EXAMPLE_SRC})
  target_include_directories(listener PUBLIC ${EXAMPLE_INCLUDE})
  target_link_libraries(listener PRIVATE  ${EXAMPLE_LIBS})
endif()
