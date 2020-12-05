cmake_minimum_required(VERSION 3.16)

project (day_5)

find_package(Threads)

# Get paths and files.
set(RootDir "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(UtilityDir "${RootDir}/utility")

# Silver
file(GLOB_RECURSE SilverSourceFiles "${CMAKE_CURRENT_SOURCE_DIR}/day_5.cpp")
include_directories ("${UtilityDir}")
add_executable(day_5 "${SilverSourceFiles}")
set_target_properties( day_5
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${RootDir}/bin"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${RootDir}/bin"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${RootDir}/bin"
  VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)