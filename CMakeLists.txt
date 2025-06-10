cmake_minimum_required(VERSION 3.10)
project(VSCodeClone VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
pkg_check_modules(GTKSOURCEVIEWMM REQUIRED gtksourceviewmm-3.0)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${GTKMM_INCLUDE_DIRS}
    ${GTKSOURCEVIEWMM_INCLUDE_DIRS}
)

# Add source files
file(GLOB SOURCES "src/*.cpp")

# Create executable
add_executable(vscode_clone ${SOURCES})

# Link libraries
target_link_libraries(vscode_clone
    ${GTKMM_LIBRARIES}
    ${GTKSOURCEVIEWMM_LIBRARIES}
)

# Add compiler flags
target_compile_options(vscode_clone PRIVATE
    ${GTKMM_CFLAGS_OTHER}
    ${GTKSOURCEVIEWMM_CFLAGS_OTHER}
) 