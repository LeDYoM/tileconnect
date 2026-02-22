cmake_minimum_required(VERSION 3.16)
project(
  "Tile Connect"
  VERSION 0.0.1.1
  DESCRIPTION "Tile Connect"
  # [HOMEPAGE_URL <url-string>]
  LANGUAGES CXX
  )

  set(ct "tile_con")
  add_executable(${ct})
  target_sources(${ct} PRIVATE
    src/main.cpp)