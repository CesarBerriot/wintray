cmake_minimum_required(VERSION 3.31)
project(wintray)

set(REQUIRED_CXX_STANDARD 17)

if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD ${REQUIRED_CXX_STANDARD})
endif()

if(CMAKE_CXX_STANDARD LESS REQUIRED_CXX_STANDARD)
  message(FATAL_ERROR "Minimum required C++ standard version is " ${REQUIRED_CXX_STANDARD})
endif()

if(NOT DEFINED WINTRAY_EXAMPLE)
  if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(WINTRAY_EXAMPLE ON)
  else()
    set(WINTRAY_EXAMPLE OFF)
  endif()
endif()

include(FetchContent)
FetchContent_Declare(fetch_github_content GIT_REPOSITORY https://github.com/CesarBerriot/fetch_github_content.git GIT_TAG 1.0.0)
FetchContent_MakeAvailable(fetch_github_content)
include(${fetch_github_content_SOURCE_DIR}/fetch_github_content.cmake)

fetch_github_content(
  discover_sources CesarBerriot 1.0.0 INCLUDE_DEFAULT
  hard_assert CesarBerriot 1.2.3
  make_string CesarBerriot 2.2.3
)

add_library(wintray)
discover_sources(wintray)
target_include_directories(wintray PUBLIC include)
target_link_libraries(wintray PRIVATE hard_assert make_string)
target_compile_definitions(wintray PRIVATE MODULE_NAME="WinTray")

if(WINTRAY_EXAMPLE)
  add_subdirectory(example)
endif()