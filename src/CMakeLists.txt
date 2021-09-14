cmake_minimum_required(VERSION 3.4)

########################################################################################################################
### Includes ###
########################################################################################################################

find_package(Proj REQUIRED)
find_package(Boost REQUIRED)

if (${Boost_VERSION} LESS 106500)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_MATH_DISABLE_FLOAT128")
endif()

if((NOT PROJ_LIBRARY) OR (NOT PROJ_INCLUDE_DIR))
  message(FATAL "Dependency proj not found!")
endif()

set(OpenDRIVE_TARGET_INCLUDE_DIRECTORIES)
list(APPEND OpenDRIVE_TARGET_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/include)
list(APPEND OpenDRIVE_TARGET_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})
list(APPEND OpenDRIVE_TARGET_INCLUDE_DIRECTORIES ${PUGIXML_INCLUDE_DIR})
list(APPEND OpenDRIVE_TARGET_INCLUDE_DIRECTORIES ${PROJ_INCLUDE_DIR})

########################################################################################################################
### Library ###
########################################################################################################################
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/opendrive_1_4/geometry SRC_ODR_1_4_GEOMETRY)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/opendrive_1_4/parser SRC_ODR_1_4_PARSER)

add_library(OpenDRIVE-lib
        ${SRC_ODR_1_4_GEOMETRY}
        ${SRC_ODR_1_4_PARSER}
        opendrive_1_4/OpenDrive.cpp
        )

target_compile_definitions(OpenDRIVE-lib PUBLIC ${TARGET_COMPILE_DEFINITIONS})
target_compile_options(OpenDRIVE-lib PRIVATE ${TARGET_COMPILE_OPTIONS})
set_property(TARGET OpenDRIVE-lib APPEND_STRING PROPERTY LINK_FLAGS ${TARGET_LINK_FLAGS})

target_include_directories(OpenDRIVE-lib PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        ${OpenDRIVE_TARGET_INCLUDE_DIRECTORIES}
        )

set(additional_libs pugixml::pugixml)
target_link_libraries(OpenDRIVE-lib
        ${additional_libs}
        ${PROJ_LIBRARY}
        )