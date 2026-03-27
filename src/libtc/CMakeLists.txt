cmake_minimum_required(VERSION 3.16)
project(
"Tile Connect Library"
  VERSION 0.0.1.1
  DESCRIPTION "Tile Connect Library"
  # [HOMEPAGE_URL <url-string>]
  LANGUAGES CXX
)

set(ctl tc)

if(DEFINED EXPORT_API_ONLY)
  message("Export API ONLY: YES")
  set(TC_BUILD_TESTS OFF)
else()
  message("Export API ONLY: NO")
  set(TC_BUILD_TESTS ON)
endif()

add_library(${ctl} SHARED)
set_project_options(${ctl})

target_sources(${ctl} PRIVATE
  FILE_SET CXX_MODULES
  FILES
    src/board.cpp
    src/common.cpp
    src/tc_module.cppm
    src/token_board.cpp
    src/token.cpp
)

target_sources(${ctl} PUBLIC
  FILE_SET HEADERS
  FILES
    src/tc_export.hpp
)

if(NOT DEFINED EXPORT_API_ONLY)
  set_target_properties(${ctl} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS true)
  target_compile_definitions(${ctl} PRIVATE tc_EXPORT_ALL)
  add_subdirectory(tests)
else()
  target_compile_definitions(${ctl} PRIVATE tc_EXPORT)
endif()
