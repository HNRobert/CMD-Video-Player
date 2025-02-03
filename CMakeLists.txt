cmake_minimum_required(VERSION 3.29)
project(cmdp)

set(CMAKE_CXX_STANDARD 20)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Flags Debug: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")

# Find required packages
find_package(OpenCV REQUIRED)
message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV include path: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")

find_package(SDL2 REQUIRED)
message(STATUS "SDL2 include path: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2 libraries: ${SDL2_LIBRARIES}")

find_package(PkgConfig REQUIRED)
pkg_check_modules(AVCODEC REQUIRED IMPORTED_TARGET libavcodec)
pkg_check_modules(AVFORMAT REQUIRED IMPORTED_TARGET libavformat)
pkg_check_modules(AVUTIL REQUIRED IMPORTED_TARGET libavutil)
pkg_check_modules(SWRESAMPLE REQUIRED IMPORTED_TARGET libswresample)
message(STATUS "FFmpeg components found:")
message(STATUS "  AVCODEC include: ${AVCODEC_INCLUDE_DIRS}")
message(STATUS "  AVFORMAT include: ${AVFORMAT_INCLUDE_DIRS}")
message(STATUS "  AVUTIL include: ${AVUTIL_INCLUDE_DIRS}")
message(STATUS "  SWRESAMPLE include: ${SWRESAMPLE_INCLUDE_DIRS}")
message(STATUS "  AVCODEC lib: ${AVCODEC_LIBRARIES}")
message(STATUS "  AVFORMAT lib: ${AVFORMAT_LIBRARIES}")
message(STATUS "  AVUTIL lib: ${AVUTIL_LIBRARIES}")
message(STATUS "  SWRESAMPLE lib: ${SWRESAMPLE_LIBRARIES}")

# Special handling for readline on macOS
if(APPLE)
    find_path(READLINE_INCLUDE_DIR readline/readline.h
        PATHS
        /opt/homebrew/opt/readline/include  # M Series Mac
        /usr/local/opt/readline/include     # Intel Mac
        /usr/include
        /usr/local/include
    )
    find_library(READLINE_LIBRARY
        NAMES readline
        PATHS
        /opt/homebrew/opt/readline/lib      # M Series Mac
        /usr/local/opt/readline/lib         # Intel Mac
        /usr/lib
        /usr/local/lib
    )
    if(NOT READLINE_INCLUDE_DIR OR NOT READLINE_LIBRARY)
        message(FATAL_ERROR "readline not found. Please install with: brew install readline")
    endif()
    message(STATUS "Readline include path: ${READLINE_INCLUDE_DIR}")
    message(STATUS "Readline library: ${READLINE_LIBRARY}")
else()
    pkg_check_modules(READLINE REQUIRED IMPORTED_TARGET readline)
    message(STATUS "Readline include path: ${READLINE_INCLUDE_DIRS}")
    message(STATUS "Readline library: ${READLINE_LIBRARIES}")
endif()

find_package(Curses REQUIRED)
message(STATUS "Curses include path: ${CURSES_INCLUDE_DIRS}")
message(STATUS "Curses library: ${CURSES_LIBRARIES}")

# Add executable
add_executable(cmdp
    src/player-basic.cpp
    src/player-core.cpp
    src/main.cpp
)

# Include directories
target_include_directories(cmdp PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
    ${AVCODEC_INCLUDE_DIRS}
    ${AVFORMAT_INCLUDE_DIRS}
    ${AVUTIL_INCLUDE_DIRS}
    ${SWRESAMPLE_INCLUDE_DIRS}
    ${CURSES_INCLUDE_DIRS}
    ${READLINE_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(cmdp
    ${OpenCV_LIBS}
    ${SDL2_LIBRARIES}
    PkgConfig::AVCODEC
    PkgConfig::AVFORMAT
    PkgConfig::AVUTIL
    PkgConfig::SWRESAMPLE
    ${CURSES_LIBRARIES}
    ${READLINE_LIBRARY}
)

# Install targets
install(TARGETS cmdp DESTINATION bin)
install(FILES
    include/cmdp/player-basic.hpp
    include/cmdp/player-core.hpp
    include/cmdp/render-basic.hpp
    DESTINATION include/cmdp
)
