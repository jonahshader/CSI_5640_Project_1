cmake_minimum_required(VERSION 3.28)
project(cellular_automata C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable OpenACC
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # Check GCC version as OpenACC support requires GCC 5.0+
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
        message(FATAL_ERROR "GCC version must be at least 5.0 for OpenACC support")
    endif()

    # Add OpenACC flags for GCC
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenacc")

    # Optional: Add optimization flags that work well with OpenACC
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

    message(STATUS "Configured OpenACC support for GCC")
else()
    message(WARNING "OpenACC support is currently configured only for GNU C++ compiler. Detected compiler: ${CMAKE_CXX_COMPILER_ID}. OpenACC flags will not be added.")
endif()

# Include FetchContent for downloading dependencies
include(FetchContent)

# SDL2
FetchContent_Declare(
        SDL2
        URL https://github.com/libsdl-org/SDL/releases/download/release-2.30.3/SDL2-2.30.3.tar.gz
        URL_HASH SHA256=820440072f8f5b50188c1dae104f2ad25984de268785be40c41a099a510f0aec
)
FetchContent_MakeAvailable(SDL2)

# TODO: imgui?

add_executable(cellular_automata src/main.cpp
  src/systems/render_state.cpp
  src/systems/render_state.h
  src/systems/types.h
  src/systems/update_state.cpp
  src/systems/update_state.h
  src/systems/run_benchmarks.cpp
  src/systems/run_benchmarks.h
  src/systems/benchmark.h
  src/systems/benchmark.cpp
)

# Link libraries
target_link_libraries(cellular_automata PRIVATE
  SDL2::SDL2
)

# Include headers
target_include_directories(cellular_automata PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Platform-specific configurations
if(WIN32)
    target_link_libraries(cellular_automata PRIVATE SDL2::SDL2main)

    # Define SDL_MAIN_HANDLED
    target_compile_definitions(${PROJECT_NAME} PRIVATE SDL_MAIN_HANDLED)
    if(MSVC)
        # Use the MultiThreaded DLL runtime library for both Debug and Release
        set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    endif()
endif()

# Copy SDL2 DLL to output directory on Windows
if(WIN32)
    add_custom_command(TARGET cellular_automata POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL2::SDL2>
            $<TARGET_FILE_DIR:cellular_automata>
    )
endif()
