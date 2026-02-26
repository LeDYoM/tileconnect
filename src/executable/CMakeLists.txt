cmake_minimum_required(VERSION 3.16)
project(tc_run
  VERSION 0.0.1.1
  LANGUAGES CXX
)

set(ct tc_run)

add_executable(${ct})
set_project_options(${ct})

target_sources(${ct} PRIVATE
  src/main.cpp)

target_link_libraries(${ct} PRIVATE tc)