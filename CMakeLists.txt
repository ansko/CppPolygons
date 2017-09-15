cmake_minimum_required (VERSION 2.6) 
 
set (PROJECT cpp_polygons) 
 
project (${PROJECT}) 

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
endif (CMAKE_COMPILER_IS_GNUCXX)

set (HEADERS 
	src/line_segment.hpp
	src/plane.hpp
	src/point.hpp
	src/polygon.hpp
	src/polygonal_cylinder.hpp
    src/vector.hpp
    src/det.hpp)

set (SOURCES
        src/det.cpp 
        src/line_segment.cpp
	    src/mainExfoliation.cpp
        #src/main.cpp
        src/plane.cpp
	    src/point.cpp
        src/polygon.cpp
        src/polygonal_cylinder.cpp
        src/vector.cpp)
 
add_executable (${PROJECT} ${HEADERS} ${SOURCES})

set_target_properties(${PROJECT} PROPERTIES COMPILE_FLAGS "-std=c++14 -g -lm")
