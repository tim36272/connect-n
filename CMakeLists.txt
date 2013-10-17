cmake_minimum_required(VERSION 2.8)
project( Connect C CXX)

set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_ADDITIONAL_VERSIONS "1.48" "1.46")

find_package( Boost REQUIRED COMPONENTS thread )
find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

add_library(Connect Connect.cpp)
target_link_libraries(Connect)
link_libraries(Connect)

add_library(utility utility.cpp)
target_link_libraries(utility)
link_libraries(utility)

add_executable( main main.cpp )
target_link_libraries( main ${Boost_LIBRARIES} ${OpenCV_LIBRARIES})
