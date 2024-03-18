cmake_minimum_required(VERSION 3.10)
project(servercom)

find_package(CURL REQUIRED)

# Include directories (everything in the SDK is already included here)
set(INCLUDES
    .
    src
    src/lib/inc
    )
include_directories(${INCLUDES})

# Add your source files
set(SOURCES
    src/lib/file_utils.cpp
    src/main.cpp
)

# Create the executable
add_executable(servercom ${SOURCES})
install(TARGETS servercom RUNTIME DESTINATION bin)
target_link_libraries(servercom CURL::libcurl)
