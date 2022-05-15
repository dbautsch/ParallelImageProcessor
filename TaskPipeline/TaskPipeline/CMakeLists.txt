cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(warnings "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings}")

file(GLOB TaskPipelineSources TaskPipeline/*.cpp)
add_executable(TaskPipelineMain ${TaskPipelineSources})
target_link_libraries(TaskPipelineMain TaskPipeline)
target_link_libraries(TaskPipelineMain pthread)
target_include_directories(TaskPipelineMain PUBLIC libTaskPipeline/)