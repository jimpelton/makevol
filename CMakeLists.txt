cmake_minimum_required(VERSION 2.8.4)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake
      ${CMAKE_MODULE_PATH})

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}. (change with -DCMAKE_BUILD_TYPE=<build-type>")

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
endif()

###############################################################################

project(makevol)

set(HEADER_FILES 
    src/bbox.h
#    src/cuboidvolume.h
    src/point3.h
    src/sphericalvolume.h
    src/Volume.h
    src/clargs.h
)

set(SOURCE_FILES 
    src/makevol.cpp
    src/clargs.cpp
)

include_directories("3rdParty/tclap/include")
add_executable(makevol ${SOURCE_FILES} ${HEADER_FILES})

#target_link_libraries()
#install(TARGETS textureSlicesTest RUNTIME DESTINATION ${BIN_DIR})
