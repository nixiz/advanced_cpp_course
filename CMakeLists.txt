# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)
project ("AdvancedCPP")
set (CMAKE_CXX_STANDARD 17)

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
add_subdirectory("src")

file (GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp 
)

add_executable(${PROJECT_NAME} "main.cpp" ${SRC_FILES})


if (UNIX)
find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(${PROJECT_NAME} PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(${PROJECT_NAME} "${CMAKE_THREAD_LIBS_INIT}")
endif()
endif (UNIX)