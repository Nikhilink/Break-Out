cmake_minimum_required(VERSION 3.12)
project(BreakOut)

set(CMAAKE_CXX_STANDARD 14)

# Add the path to the Raylib package config file
list(APPEND CMAKE_PREFIX_PATH "/path/to/raylib")

# Find the Raylib package
find_package(raylib 5.0 QUIET) # Let's find the installed raylib

# If Raylib is not installed on the system, fetch it and build from source
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
        raylib
        URL https://github.com/raysan5/raylib/archive/master.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

# Define the executable
add_executable(${PROJECT_NAME} 
                main.cpp 
                constants.h 
                logging.h 
                asset_loader.h 
                asset_loader.cpp 
                scenes.h scenes.cpp
                game_objects.h 
                game_objects.cpp)



# Link against raylib
target_link_libraries(${PROJECT_NAME} raylib)