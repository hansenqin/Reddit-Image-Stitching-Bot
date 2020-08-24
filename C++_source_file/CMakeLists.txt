cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 14)
project(ImageStitch)

find_package(opencv REQUIRED)
include_directories(${opencv_INCLUDE_DIRECTORIES})

find_package(Boost 1.57.0 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(eigen3 REQUIRED )
include_directories(${Eigen3_INCLUDE_DIRECTORIES})

find_package(nlopt REQUIRED )
include_directories(${nlopt_INCLUDE_DIRECTORIES})

link_directories(/usr/local/lib)
link_libraries(nlopt)

add_executable(ImageStitch main.cpp obj_function.cpp functions.h mat2vec.cpp)
# linking
target_link_libraries(ImageStitch ${nlopt_LIBRARIES} ${Eigen3_LIBRARIES} ${OpenCV_LIBRARIES} ${Boost_LIBRARIES})

