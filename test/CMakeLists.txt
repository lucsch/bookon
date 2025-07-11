set(UNIT_TESTING_DATA_PATH "${CMAKE_CURRENT_SOURCE_DIR}/data")
set(UNIT_TESTING_DATA_OUTPUT_PATH "${CMAKE_BINARY_DIR}/unit_testing_output")

# replace path on Windows with backslash
if (MSVC)
    string(REPLACE "/" "\\\\" UNIT_TESTING_DATA_PATH ${UNIT_TESTING_DATA_PATH})
    string(REPLACE "/" "\\\\" UNIT_TESTING_DATA_OUTPUT_PATH ${UNIT_TESTING_DATA_OUTPUT_PATH})
    if (${UNIT_TESTING_DATA_PRIVATE})
        string(REPLACE "/" "\\\\" UNIT_TESTING_DATA_PRIVATE ${UNIT_TESTING_DATA_PRIVATE})
    endif ()
endif ()

configure_file("test_param.h.in" "${CMAKE_BINARY_DIR}/test_param.h" @ONLY)
include_directories(${CMAKE_BINARY_DIR})
include_directories(${Gtest_INCLUDE_DIRS})

add_executable(UnitTests
        test.cpp )
#         $<TARGET_OBJECTS:${PROJECT_NAME}_lib>)
target_link_libraries(UnitTests ${PROJECT_NAME}_lib ${wxwidgets_LIBRARIES} ${protobuf_LIBRARIES} ${GTest_LIBRARIES})
add_test(NAME test COMMAND UnitTests)

# suppressing warning for missing PDB files for linked libs
if (MSVC)
    set_target_properties(UnitTests PROPERTIES LINK_FLAGS "/ignore:4099")
endif ()