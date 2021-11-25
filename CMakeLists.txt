cmake_minimum_required(VERSION 3.0)
project("bookon")

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# cmake magical command for conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# using git version
set(${PROJECT_NAME}_MAJOR_VERSION "0")
set(${PROJECT_NAME}_MINOR_VERSION "1")
include(cmake_modules/version.cmake)

# osx icon
set(MACOSX_BUNDLE_ICON_FILE bookon.icns)
set(ICON_PATH "${CMAKE_SOURCE_DIR}/art/bookon.icns")
set_source_files_properties(${ICON_PATH} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

#include
INCLUDE_DIRECTORIES(src)
ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} MACOSX_BUNDLE WIN32
        src/app.cpp
        src/framemain.cpp
        src/bitmap.cpp
        src/frameabout.cpp
        ${CMAKE_BINARY_DIR}/version.cpp
        ${ICON_PATH}
        src/resource.rc)
target_link_libraries(${CMAKE_PROJECT_NAME} ${CONAN_LIBS})

# unit testing
enable_testing()
add_subdirectory(test)


install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin
        BUNDLE DESTINATION .)
include(CPack)