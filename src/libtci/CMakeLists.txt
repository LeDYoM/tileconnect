cmake_minimum_required(VERSION 3.16)
project(
"Tile Connect Library Interface"
  LANGUAGES CXX
)

set(ctl tci)

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
    src/tci_module.ixx
)

target_sources(${ctl} PUBLIC
  FILE_SET HEADERS
  FILES
    src/tc_export.hpp
)
