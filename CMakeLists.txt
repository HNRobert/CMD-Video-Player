cmake_minimum_required(VERSION 3.29)
project(cmdp)

set(CMAKE_CXX_STANDARD 20)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Flags Debug: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")

set(DEP_TYPE "B" CACHE STRING "Install using dependencies from Homebrew(B), APT(A) or Win(W)")

if(DEP_TYPE STREQUAL "B")
    message("Install using Homebrew Dependencies")
    # Determine Homebrew prefix based on platform and architecture
    if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
            set(DEP_PREFIX "/opt/homebrew/opt")
        else()
            set(DEP_PREFIX "/usr/local/opt")
        endif()
    else() # Linux
        set(DEP_PREFIX "/home/linuxbrew/.linuxbrew/opt")
    endif()
    message(STATUS "Using Homebrew prefix: ${DEP_PREFIX}")

elseif(DEP_TYPE STREQUAL "A")
    message("Install using APT Dependencies")


elseif(DEP_TYPE STREQUAL "W")
    message("Install using Windows Dependencies")
endif()

include_directories(
    ${DEP_PREFIX}/ffmpeg/include
    ${DEP_PREFIX}/ncurses/include
    ${DEP_PREFIX}/opencv/include/opencv4
    ${DEP_PREFIX}/readline/include
    ${DEP_PREFIX}/sdl2/include
)

link_directories(
    ${DEP_PREFIX}/ffmpeg/lib
    ${DEP_PREFIX}/ncurses/lib
    ${DEP_PREFIX}/opencv/lib
    ${DEP_PREFIX}/readline/lib
    ${DEP_PREFIX}/sdl2/lib
)

add_executable(cmdp
    src/player-basic.cpp
    src/player-core.cpp
    src/main.cpp
)

target_include_directories(cmdp PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# target_link_libraries expects library names or paths
target_link_libraries(cmdp
    avcodec
    avformat
    avutil
    swresample
    opencv_core
    opencv_highgui
    opencv_imgproc
    opencv_videoio
    SDL2
    readline
    ncurses
)

# Install the executable
install(TARGETS cmdp DESTINATION bin)

# Install header files
install(FILES
    include/cmdp/player-basic.hpp
    include/cmdp/player-core.hpp
    include/cmdp/render-basic.hpp
    DESTINATION include/cmdp
)
