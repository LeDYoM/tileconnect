cmake_minimum_required(VERSION 3.16)
project(
"Tile Connect"
  VERSION 0.0.1.1
  DESCRIPTION "Tile Connect"
  # [HOMEPAGE_URL <url-string>]
  LANGUAGES CXX
)

include(functions.cmake)

add_subdirectory(src/libtc)
add_subdirectory(src/executable)
add_subdirectory(tests)
