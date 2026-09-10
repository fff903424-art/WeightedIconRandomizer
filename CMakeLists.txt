cmake_minimum_required(VERSION 3.29)

project(WeightedIconRandomizer VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "GEODE_SDK is not set.")
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include/Config.hpp")
    message(FATAL_ERROR
        "Config.hpp is missing from: ${CMAKE_CURRENT_SOURCE_DIR}/include"
    )
endif()

add_subdirectory(
    $ENV{GEODE_SDK}
    ${CMAKE_CURRENT_BINARY_DIR}/geode
)

add_library(${PROJECT_NAME} SHARED
    src/Config.cpp
    src/Randomizer.cpp
    src/UI.cpp
    src/main.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

setup_geode_mod(${PROJECT_NAME})
