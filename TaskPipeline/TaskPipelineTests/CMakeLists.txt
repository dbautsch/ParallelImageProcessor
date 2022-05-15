cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(warnings "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings}")

file(GLOB TaskPipelineTestSources TaskPipelineTests/*.cpp)
add_executable(TaskPipelineTest ${TaskPipelineTestSources})
target_link_libraries(TaskPipelineTest TaskPipeline)
target_link_libraries(TaskPipelineTest gtest)
target_link_libraries(TaskPipelineTest pthread)
target_include_directories(TaskPipelineTest PUBLIC libTaskPipeline/)
