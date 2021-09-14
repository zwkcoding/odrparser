cmake_minimum_required(VERSION 3.4)

if (BUILD_TESTING)
    add_subdirectory(googletest EXCLUDE_FROM_ALL)
endif ()
add_subdirectory(pugixml)
