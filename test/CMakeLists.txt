# add tracks directory
add_definitions(-DTRACKS_DIR=\"${PROJECT_SOURCE_DIR}/test/test_files/\")

# create test executable
set(EXEC_NAME odr_parser_test)
set(TEST_SOURCES
    opendrive_1_4/ParseOpenDrive14Tests.cpp
    main.cpp)
# aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/opendrive_1_4/geometry SRC_ODR_1_4_GEOMETRY)

add_executable(${EXEC_NAME} ${TEST_SOURCES})
target_include_directories(${EXEC_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

add_test(NAME ${EXEC_NAME} COMMAND ${EXEC_NAME}
         WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(${EXEC_NAME} OpenDRIVE-lib GTest::gtest)

target_compile_options(${EXEC_NAME} PRIVATE ${TARGET_COMPILE_OPTIONS})
set_property(TARGET ${EXEC_NAME} APPEND_STRING PROPERTY LINK_FLAGS ${TARGET_LINK_FLAGS})