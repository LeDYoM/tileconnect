cmake_minimum_required(VERSION 3.16)
project(
"Tile Connect"
  VERSION 0.0.1.1
  DESCRIPTION "Tile Connect"
  # [HOMEPAGE_URL <url-string>]
  LANGUAGES CXX
)

set(_REPO_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# First for the generic no-config case (e.g. with mingw, Ninja, Makefiles...)
if(DEFINED CMAKE_BUILD_TYPE)
  message("Mono-config generator. Config type: " ${CMAKE_BUILD_TYPE})
  message("Output directory " ${_REPO_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
      ${_REPO_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
      ${_REPO_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
      ${_REPO_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})
endif()

# Second, for multi-config builds (e.g. msvc)
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
  message("For config " ${OUTPUTCONFIG} " we have "
          ${_REPO_OUTPUT_DIRECTORY}/${OUTPUTCONFIG})
  string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG}
      ${_REPO_OUTPUT_DIRECTORY}/${OUTPUTCONFIG})
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG}
      ${_REPO_OUTPUT_DIRECTORY}/${OUTPUTCONFIG})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG}
      ${_REPO_OUTPUT_DIRECTORY}/${OUTPUTCONFIG})
endforeach()

include(functions.cmake)
testing_init()

add_subdirectory(src/libtc)
add_subdirectory(src/executable)
