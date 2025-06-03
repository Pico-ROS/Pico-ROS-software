cmake_minimum_required(VERSION 3.13)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

file(READ ${CMAKE_CURRENT_SOURCE_DIR}/version.txt version)
project(pico-ros VERSION ${version} LANGUAGES C)

option(PICOROS_BUILD_EXAMPLES "Build examples" ON)
option(PICOROS_BUILD_TESTS "Build tests" ON)
message("PICOROS_BUILD_EXAMPLES: ${PICOROS_BUILD_EXAMPLES}")

set(CMAKE_C_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
      ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()


# zenoh-pico
set(BUILD_EXAMPLES OFF)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(thirdparty/zenoh-pico)

# Micro-CDR
set(UCDR_SUPERBUILD OFF)
set(UCDR_ISOLATED_INSTALL OFF)
add_subdirectory(thirdparty/Micro-CDR)

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

add_library(picoparams STATIC
    src/picoparams.c
    src/picoparams.h
)
target_include_directories(picoparams PUBLIC
     src/
)

if (TYPE_FILE)
  message("USER_TYPE_FILE: ${TYPE_FILE}")
  target_compile_definitions(picoserdes PUBLIC -DUSER_TYPE_FILE="${TYPE_FILE}")
endif()
target_link_libraries(picoros zenohpico::lib)
target_link_libraries(picoserdes microcdr)
target_link_libraries(picoparams zenohpico::lib microcdr picors picoserdes)

# Add test executable
if(PICOROS_BUILD_TESTS)
  add_executable(test_picoserdes test/test_picoserdes.c)
  target_include_directories(test_picoserdes PRIVATE src)
  target_link_libraries(test_picoserdes PRIVATE picoserdes microcdr)
  add_test(NAME test_picoserdes COMMAND test_picoserdes)
endif()

# Build examples if enabled
if(PICOROS_BUILD_EXAMPLES)
  add_library(examples_serdes STATIC
    src/picoserdes.c
    src/picoserdes.h
  )
  target_include_directories(examples_serdes PUBLIC examples)
  target_compile_definitions(examples_serdes PUBLIC -DUSER_TYPE_FILE="example_types.h")
  target_link_libraries(examples_serdes microcdr)

  set(EXAMPLE_LIBS
            picoros
            examples_serdes
  )
  set(EXAMPLE_SRC
            src/picoparams.c
            src/picoparams.h
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

  add_executable(odometry_listener examples/odometry_listener.c  ${EXAMPLE_SRC})
  target_include_directories(odometry_listener PUBLIC ${EXAMPLE_INCLUDE})
  target_link_libraries(odometry_listener PRIVATE  ${EXAMPLE_LIBS})

  add_executable(odometry_publisher examples/odometry_publisher.c  ${EXAMPLE_SRC})
  target_include_directories(odometry_publisher PUBLIC ${EXAMPLE_INCLUDE})
  target_link_libraries(odometry_publisher PRIVATE  ${EXAMPLE_LIBS})

  add_executable(params_server examples/params_server.c  ${EXAMPLE_SRC})
  target_include_directories(params_server PUBLIC ${EXAMPLE_INCLUDE})
  target_link_libraries(params_server PRIVATE  ${EXAMPLE_LIBS})
endif()
