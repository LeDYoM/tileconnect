cmake_minimum_required(VERSION 3.16)
project(tci
  DESCRIPTION "Tile Connect Interface (tc)"
  LANGUAGES CXX
)

set(current_target tci)

add_library(${current_target} SHARED)
set_project_options(${current_target})

target_sources(${current_target} PRIVATE
  FILE_SET CXX_MODULES
  FILES
    src/idefaulted_game.cpp
    src/ievent.cpp
    src/igame.cpp
    src/tci_module.ixx
)

target_sources(${current_target} PUBLIC
  FILE_SET HEADERS
  FILES
    src/tci_export.hpp
)
