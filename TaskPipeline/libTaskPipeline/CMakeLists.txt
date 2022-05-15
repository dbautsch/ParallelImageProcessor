cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(warnings "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings}")

file(GLOB libTaskPipelineSources libTaskPipeline/*.cpp libTaskPipeline/PipelineStages/*.cpp)
add_library(TaskPipeline SHARED ${libTaskPipelineSources})
target_include_directories(TaskPipeline PRIVATE libTaskPipeline/)
