cmake_minimum_required(VERSION 3.0)

project(rtlib)

add_library(
    rtlib
    INTERFACE
)

target_include_directories(
    rtlib
    INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory(ci)
else()
    add_subdirectory(ci EXCLUDE_FROM_ALL)
endif()