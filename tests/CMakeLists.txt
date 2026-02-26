cmake_minimum_required (VERSION 3.14)
project(tc_utests CXX)

set(ct ${PROJECT_NAME})

prepareTestLibrary()

add_executable(${ct})
set_project_options(${ct})

target_sources(${ct} PRIVATE
    board.tests.cpp
    catch_include.hpp)

target_link_libraries(${ct} PRIVATE Catch2::Catch2WithMain tc)
