cmake_minimum_required(VERSION 3.5)
project("bookon")

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# cmake magical command for conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# using git version
set(${PROJECT_NAME}_MAJOR_VERSION "1")
set(${PROJECT_NAME}_MINOR_VERSION "1")
include(cmake_modules/version.cmake)

# defining cmake version for installer mainly
set(CMAKE_PROJECT_VERSION_MAJOR ${${PROJECT_NAME}_MAJOR_VERSION})
set(CMAKE_PROJECT_VERSION_MINOR ${${PROJECT_NAME}_MINOR_VERSION})
set(CMAKE_PROJECT_VERSION_PATCH ${GIT_NUMBER})

# osx icon
set(MACOSX_BUNDLE_ICON_FILE bookon.icns)
set(ICON_PATH "${CMAKE_SOURCE_DIR}/art/bookon.icns")
set_source_files_properties(${ICON_PATH} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

# remove deprecated warnings on windows
if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif ()

INCLUDE_DIRECTORIES(src)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# put source files into a library for unit testing
find_package(Protobuf REQUIRED)
set(Protobuf_DEBUG ON)
message("Protobuf found: ${Protobuf_FOUND}")
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS src/bookmark.proto)
set(SRC_FILES
        ${PROTO_SRCS}
        src/bookmark.cpp)
add_library(${PROJECT_NAME}_lib OBJECT ${SRC_FILES})

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} MACOSX_BUNDLE WIN32
        src/app.cpp
        src/framemain.cpp
        src/bitmap.cpp
        src/frameabout.cpp
        src/contolmain.cpp
        src/framebookmark.cpp
        src/bookdnd.cpp
        src/treedata.cpp
        ${CMAKE_BINARY_DIR}/version.cpp
        ${ICON_PATH}
        src/resource.rc)
target_link_libraries(${CMAKE_PROJECT_NAME} ${CONAN_LIBS} ${PROJECT_NAME}_lib)
if(UNIX AND NOT APPLE)
    find_package(PNG REQUIRED)
    target_link_libraries(${PROJECT_NAME} ${PNG_LIBRARY} xkbcommon)
    target_link_libraries(${PROJECT_NAME}_lib ${PNG_LIBRARY} xkbcommon)
endif(UNIX AND NOT APPLE)

# unit testing
enable_testing()
add_subdirectory(test)


install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin
        BUNDLE DESTINATION .)

# installer
if (MSVC)
    file(GLOB LIBS "${CMAKE_BINARY_DIR}/bin/*${CMAKE_SHARED_LIBRARY_SUFFIX}")
    install(FILES ${LIBS} DESTINATION bin)
    set(CPACK_GENERATOR "ZIP")
endif ()

if (APPLE)
    set(CPACK_GENERATOR "DragNDrop")
endif ()

if (UNIX AND NOT APPLE)
    INSTALL(FILES install/bookon.desktop DESTINATION share/applications)
    INSTALL(FILES art/bookon128.png DESTINATION share/icons)
    SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "lucsch") #required for debian packages
    SET(CPACK_DEBIAN_PACKAGE_VERSION ${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${GIT_NUMBER})
    SET(CPACK_DEBIAN_FILE_NAME ${PROJECT_NAME}-${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${GIT_NUMBER}-Linux.deb)
endif (UNIX AND NOT APPLE)

include(CPack)