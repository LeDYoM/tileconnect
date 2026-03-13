cmake_minimum_required(VERSION 3.16)
project(
"Tile Connect Library"
  VERSION 0.0.1.1
  DESCRIPTION "Tile Connect Library"
  # [HOMEPAGE_URL <url-string>]
  LANGUAGES CXX
)

set(ctl tc)

add_library(${ctl} STATIC)
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
