cmake_minimum_required(VERSION 3.4)
include(CMakeDependentOption)

# Technically not needed for this file. This is builtin CMAKE global variable.
# option(BUILD_SHARED_LIBS "Build shared instead of static library" OFF)

cmake_dependent_option(PUGIXML_BUILD_SHARED_AND_STATIC_LIBS
  "Build both shared and static libraries" OFF
  "BUILD_SHARED_LIBS" OFF)

# Expose options from the pugiconfig.hpp
option(PUGIXML_WCHAR_MODE "Enable wchar_t mode" OFF)
option(PUGIXML_COMPACT "Enable compact mode" OFF)

# Advanced options from pugiconfig.hpp
option(PUGIXML_NO_XPATH "Disable XPath" OFF)
option(PUGIXML_NO_STL "Disable STL" OFF)
option(PUGIXML_NO_EXCEPTIONS "Disable Exceptions" OFF)
mark_as_advanced(PUGIXML_NO_XPATH PUGIXML_NO_STL PUGIXML_NO_EXCEPTIONS)

if (NOT DEFINED CMAKE_CXX_STANDARD_REQUIRED)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

if (NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 11)
endif()

set(libs)

if (BUILD_SHARED_LIBS)
  add_library(pugixml-shared SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/pugixml.cpp)
  add_library(pugixml::shared ALIAS pugixml-shared)
  list(APPEND libs pugixml-shared)

  set_property(TARGET pugixml-shared PROPERTY EXPORT_NAME shared)
  target_include_directories(pugixml-shared
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
endif()

if (NOT BUILD_SHARED_LIBS OR PUGIXML_BUILD_SHARED_AND_STATIC_LIBS)
  add_library(pugixml-static STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/pugixml.cpp)
  add_library(pugixml::static ALIAS pugixml-static)
  list(APPEND libs pugixml-static)

  set_property(TARGET pugixml-static PROPERTY EXPORT_NAME static)
  target_include_directories(pugixml-static
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
endif()

if (BUILD_SHARED_LIBS)
  set(pugixml-alias pugixml-shared)
else()
  set(pugixml-alias pugixml-static)
endif()
add_library(pugixml INTERFACE)
target_link_libraries(pugixml INTERFACE ${pugixml-alias})
add_library(pugixml::pugixml ALIAS pugixml)