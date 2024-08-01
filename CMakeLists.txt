cmake_minimum_required(VERSION 3.10)

project(PolyskelProject)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files for the shared library
set(LIBRARY_SOURCES
    lav.cpp
    lavertex.cpp
    polyskel.cpp
    slav.cpp
    util.cpp
)

# Add header files (not necessary for the build, but useful for IDEs)
set(LIBRARY_HEADERS
    event.h
    lav.h
    lavertex.h
    polyskel.h
    slav.h
    util.h
    vec.h
)

file(COPY ${CMAKE_SOURCE_DIR}/test DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/examples DESTINATION ${CMAKE_BINARY_DIR})


# Create the shared library
add_library(polyskel SHARED ${LIBRARY_SOURCES})

# Add the executable
add_executable(demo demo.cpp)

# Link the shared library to the executable
target_link_libraries(demo polyskel)

# Enable testing
enable_testing()

# Add a simple test (for example purposes, you should replace with actual tests)
add_test(NAME CompareScriptTest
          COMMAND python3 test/test.py )
#         COMMAND ${CMAKE_COMMAND} -E env python3 ${CMAKE_SOURCE_DIR}/test/compare.py)

# Optionally, you can add include directories if needed
target_include_directories(polyskel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(demo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})