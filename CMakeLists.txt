# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)
project ("STMAdvancedCPP")
set (CMAKE_CXX_STANDARD 14)

include_directories(
	"."
	"inc"
)

include_directories("src/1-Inheritance")
include_directories("src/2-SmartPointer")
include_directories("src/3-Static")
include_directories("src/4-CPUArchitecture")
include_directories("src/5-Templates")
include_directories("src/6-Exceptions")
include_directories("src/7-CustomComparators")
include_directories("src/8-Cpp11Features")

#add_subdirectory("src/1-Inheritance")
#add_subdirectory("src/2-SmartPointer")
#add_subdirectory("src/3-Static")
#add_subdirectory("src/4-CPUArchitecture")
#add_subdirectory("src/5-Templates")
#add_subdirectory("src/6-Exceptions")
#add_subdirectory("src/7-CustomComparators")
#add_subdirectory("src/8-Cpp11Features")

add_executable(${PROJECT_NAME} main.cpp "src/2-SmartPointer/bonus/shared.cpp")