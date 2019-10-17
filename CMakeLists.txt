cmake_minimum_required(VERSION 2.8.3)
project(OpticalMirroredSafe)

## Compile as C++11, 
add_compile_options(-std=c++11)

###########
## Build ##
###########

## Specify additional locations of header files
include_directories(include)

## Declare a C++ library
add_library(${PROJECT_NAME} src/OpticalSafe.cpp src/OpticalBeam.cpp)
#add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS})
target_link_libraries(${PROJECT_NAME})

## Declare a C++ executable
add_executable(${PROJECT_NAME}_ex src/main.cpp)
#add_dependencies(${PROJECT_NAME}_ex ${${PROJECT_NAME}_EXPORTED_TARGETS})
target_link_libraries(${PROJECT_NAME}_ex ${PROJECT_NAME})
