# Pico-ROS as an ESP-IDF component.
#
# Builds picoros, picoserdes, Micro-CDR and zenoh-pico straight from this repository and its submodules as one
# component named `picoros`. Nothing is copied or vendored. zenoh-pico's `config.h` is generated at configure time
# by a configure-only CMake pass of the submodule with the espidf profile, so the feature set always matches the
# checked-out zenoh-pico and the flags below.
#
# Usage, in the project CMakeLists.txt before include($ENV{IDF_PATH}/tools/cmake/project.cmake):
#
#   list(APPEND EXTRA_COMPONENT_DIRS "${CMAKE_CURRENT_LIST_DIR}/third_party/Pico-ROS-software/components")
#
#   # Optional. Shown with their defaults.
#   set(PICOROS_USER_TYPE_FILE "example_types.h")           # header with the ROS types, see src/picoserdes.h
#   set(PICOROS_USER_TYPE_INCLUDE_DIRS "<this repo>/examples") # where that header lives (absolute paths)
#   set(PICOROS_ZENOH_CMAKE_ARGS -DZ_FEATURE_MULTI_THREAD=1 ...) # zenoh-pico feature flags, see the list below
#   set(PICOROS_DEBUG OFF)                                    # Pico-ROS logging
#   set(PICOROS_ZENOH_LOG "")                                 # zenoh-pico log level: ERROR, WARN, INFO, DEBUG, TRACE
#
# and `REQUIRES picoros` in the components that use it. Add this directory's parent (`components`) to
# EXTRA_COMPONENT_DIRS, never the repository root: the root CMakeLists.txt is the host build. picoparams is not part
# of this component.

get_filename_component(PICOROS_ROOT "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
set(ZP_ROOT "${PICOROS_ROOT}/thirdparty/zenoh-pico")
set(CDR_ROOT "${PICOROS_ROOT}/thirdparty/Micro-CDR")

if(NOT DEFINED PICOROS_USER_TYPE_FILE)
    set(PICOROS_USER_TYPE_FILE "example_types.h")
endif()
if(NOT DEFINED PICOROS_USER_TYPE_INCLUDE_DIRS)
    set(PICOROS_USER_TYPE_INCLUDE_DIRS "${PICOROS_ROOT}/examples")
endif()
if(NOT DEFINED PICOROS_ZENOH_CMAKE_ARGS)
    # TCP client only; lease 60000 ms x 2 matches rmw_zenohd's default (see the top-level readme).
    set(PICOROS_ZENOH_CMAKE_ARGS
        -DZ_FEATURE_MULTI_THREAD=1
        -DZ_FEATURE_LINK_TCP=1
        -DZ_FEATURE_LINK_UDP_MULTICAST=0
        -DZ_FEATURE_MULTICAST_TRANSPORT=0
        -DZ_FEATURE_SCOUTING=0
        -DZ_FEATURE_LINK_SERIAL=0
        -DZ_TRANSPORT_LEASE=60000
        -DZ_TRANSPORT_LEASE_EXPIRE_FACTOR=2)
endif()
if(NOT DEFINED PICOROS_DEBUG)
    set(PICOROS_DEBUG OFF)
endif()
if(NOT DEFINED PICOROS_ZENOH_LOG)
    set(PICOROS_ZENOH_LOG "")
endif()

# zenoh-pico sources: mirrors the Sources/LinkSources lists in zenoh-pico's CMakeLists.txt plus
# cmake/platforms/espidf.cmake.
file(GLOB_RECURSE ZP_SRCS
    "${ZP_ROOT}/src/api/*.c"
    "${ZP_ROOT}/src/collections/*.c"
    "${ZP_ROOT}/src/net/*.c"
    "${ZP_ROOT}/src/protocol/*.c"
    "${ZP_ROOT}/src/runtime/*.c"
    "${ZP_ROOT}/src/session/*.c"
    "${ZP_ROOT}/src/transport/*.c"
    "${ZP_ROOT}/src/utils/*.c"
    "${ZP_ROOT}/src/system/common/*.c")
file(GLOB ZP_LINK_SRCS
    "${ZP_ROOT}/src/link/config/*.c"
    "${ZP_ROOT}/src/link/multicast/*.c"
    "${ZP_ROOT}/src/link/unicast/*.c"
    "${ZP_ROOT}/src/link/transport/common/*.c")
list(APPEND ZP_SRCS ${ZP_LINK_SRCS}
    "${ZP_ROOT}/src/link/link.c"
    "${ZP_ROOT}/src/link/endpoint.c"
    "${ZP_ROOT}/src/link/transport/tcp/address.c"
    "${ZP_ROOT}/src/link/transport/udp/address.c"
    "${ZP_ROOT}/src/link/transport/upper/serial_protocol.c"
    "${ZP_ROOT}/src/link/transport/upper/tls_stream.c"
    "${ZP_ROOT}/src/system/espidf/system.c"
    "${ZP_ROOT}/src/system/socket/esp32.c"
    "${ZP_ROOT}/src/link/transport/tcp/tcp_esp32.c"
    "${ZP_ROOT}/src/link/transport/udp/udp_esp32.c"
    "${ZP_ROOT}/src/link/transport/serial/uart_espidf.c")
if("${PICOROS_ZENOH_CMAKE_ARGS}" MATCHES "Z_FEATURE_LINK_UDP_MULTICAST=1")
    list(APPEND ZP_SRCS "${ZP_ROOT}/src/link/transport/udp/udp_multicast_esp32.c")
endif()

set(CDR_SRCS
    "${CDR_ROOT}/src/c/common.c"
    "${CDR_ROOT}/src/c/types/array.c"
    "${CDR_ROOT}/src/c/types/basic.c"
    "${CDR_ROOT}/src/c/types/sequence.c"
    "${CDR_ROOT}/src/c/types/string.c")

set(PICOROS_SRCS
    "${PICOROS_ROOT}/src/picoros.c"
    "${PICOROS_ROOT}/src/picoserdes.c")

# zenoh-pico generates include/zenoh-pico/config.h at CMake configure time. Run a configure-only pass of its own
# CMakeLists with the espidf profile into the build directory (no compiler is invoked; the system name and
# processor only have to let project() succeed). Skipped during IDF's early requirements scan, where project
# variables are not available either.
set(ZP_GEN_DIR "${CMAKE_BINARY_DIR}/zenoh_pico_config")
if(NOT CMAKE_BUILD_EARLY_EXPANSION)
    if(NOT EXISTS "${ZP_ROOT}/CMakeLists.txt" OR NOT EXISTS "${CDR_ROOT}/CMakeLists.txt")
        message(FATAL_ERROR "Pico-ROS submodules are missing: run `git submodule update --init --recursive` in ${PICOROS_ROOT}")
    endif()
    message(STATUS "picoros: USER_TYPE_FILE=${PICOROS_USER_TYPE_FILE}; zenoh-pico args: ${PICOROS_ZENOH_CMAKE_ARGS}")
    execute_process(
        COMMAND ${CMAKE_COMMAND} -S "${ZP_ROOT}" -B "${ZP_GEN_DIR}" -G "${CMAKE_GENERATOR}"
            -DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}
            -DCMAKE_C_COMPILER_WORKS=ON -DCMAKE_C_COMPILER_FORCED=ON -DCMAKE_C_COMPILER_ID=GNU
            -DCMAKE_SYSTEM_NAME=Generic -DCMAKE_SYSTEM_PROCESSOR=riscv32
            -DZP_PLATFORM=espidf -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF
            -DBUILD_EXAMPLES=OFF -DBUILD_TESTING=OFF -DBUILD_TOOLS=OFF
            ${PICOROS_ZENOH_CMAKE_ARGS}
        RESULT_VARIABLE ZP_CFG_RES
        OUTPUT_VARIABLE ZP_CFG_OUT
        ERROR_VARIABLE ZP_CFG_OUT)
    if(NOT ZP_CFG_RES EQUAL 0 OR NOT EXISTS "${ZP_GEN_DIR}/include/zenoh-pico/config.h")
        message(FATAL_ERROR "zenoh-pico config generation failed:\n${ZP_CFG_OUT}")
    endif()
endif()

idf_component_register(
    SRCS ${ZP_SRCS} ${CDR_SRCS} ${PICOROS_SRCS}
    INCLUDE_DIRS
        "${ZP_GEN_DIR}/include"
        "${ZP_ROOT}/include"
        "${CDR_ROOT}/include"
        "${PICOROS_ROOT}/thirdparty/config"
        "${PICOROS_ROOT}/src"
        ${PICOROS_USER_TYPE_INCLUDE_DIRS}
    REQUIRES esp_driver_uart pthread lwip freertos)

target_compile_definitions(${COMPONENT_LIB} PUBLIC
    ZENOH_ESPIDF ZENOH_C_STANDARD=11 ZENOH_COMPILER_GCC
    USER_TYPE_FILE="${PICOROS_USER_TYPE_FILE}")
if(PICOROS_DEBUG)
    target_compile_definitions(${COMPONENT_LIB} PUBLIC PICOROS_DEBUG)
endif()
if(NOT PICOROS_ZENOH_LOG STREQUAL "")
    string(TOUPPER "${PICOROS_ZENOH_LOG}" ZP_LOG)
    if(NOT ZP_LOG MATCHES "^(ERROR|WARN|INFO|DEBUG|TRACE)$")
        message(FATAL_ERROR "PICOROS_ZENOH_LOG must be one of ERROR, WARN, INFO, DEBUG, TRACE (got '${PICOROS_ZENOH_LOG}')")
    endif()
    target_compile_definitions(${COMPONENT_LIB} PRIVATE ZENOH_LOG_${ZP_LOG})
endif()

# Third-party code is built as-is; don't let IDF's -Werror=all reject upstream style.
target_compile_options(${COMPONENT_LIB} PRIVATE
    -Wno-error -Wno-unused-parameter -Wno-sign-compare -Wno-missing-field-initializers
    -Wno-implicit-fallthrough -Wno-unused-function -Wno-unused-variable -Wno-unused-but-set-variable
    -Wno-stringop-truncation -Wno-format-truncation -Wno-array-bounds -Wno-maybe-uninitialized
    -Wno-stringop-overflow -Wno-type-limits -Wno-cast-function-type -Wno-discarded-qualifiers)
