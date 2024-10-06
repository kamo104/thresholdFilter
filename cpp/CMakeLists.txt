cmake_minimum_required(VERSION 3.29)

project(filter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenMP REQUIRED)

find_package(Boost 1.81 COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIR})

add_executable(${CMAKE_PROJECT_NAME} main2.cpp)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX ${Boost_LIBRARIES} ${X11_LIBRARIES})
