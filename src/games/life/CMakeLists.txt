cmake_minimum_required(VERSION 3.16)
project(life-game
    DESCRIPTION "Implementation of the typical game of life using tc"
    LANGUAGES CXX
)

set(game_project life)

add_library(${game_project} SHARED)
set_project_options(${game_project})

target_sources(${game_project} PRIVATE
  FILE_SET CXX_MODULES
  FILES
    src/life_game.cpp
    src/life_module.ixx
)

target_link_libraries(${game_project} PRIVATE tci)
