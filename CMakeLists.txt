cmake_minimum_required(VERSION 3.28)
project(cellular_automata CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
  src/systems/run_benchmark.cpp
  src/systems/run_benchmark.h
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