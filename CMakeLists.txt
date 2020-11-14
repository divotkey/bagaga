#
# Main CMake file for multi-project solution using AST-Utilities 
#

# Minimum required CMAKE version.
cmake_minimum_required(VERSION 3.1)

# Set project name (required by CMake)
project(AST)

# ASTU Library, must be in subdirectory 'astu'
add_subdirectory(${PROJECT_SOURCE_DIR}/astu astu)

# Collection of Sub-projects
add_subdirectory(${PROJECT_SOURCE_DIR}/client client)
#add_subdirectory(${PROJECT_SOURCE_DIR}/HelloWorld hello_world)
#add_subdirectory(${PROJECT_SOURCE_DIR}/HelloAstu hello_astu)

