# Pico-ROS barebones talker/listener for the Waveshare ESP32-P4-NANO (Ethernet).
#
#   idf.py set-target esp32p4
#   idf.py build flash monitor
cmake_minimum_required(VERSION 3.16)

# Pico-ROS, Micro-CDR and zenoh-pico come from this repository's `components/picoros`.
set(EXTRA_COMPONENT_DIRS "${CMAKE_CURRENT_LIST_DIR}/../../components")
set(PICOROS_DEBUG ON)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(picoros_esp32p4_nano)
