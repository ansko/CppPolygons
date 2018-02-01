cmake_minimum_required (VERSION 2.6) 
 
set (PROJECT polygonal) 
 
project (${PROJECT}) 

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
endif (CMAKE_COMPILER_IS_GNUCXX)

set (HEADERS 
	src/geometries/line_segment.hpp
	src/geometries/plane.hpp
	src/geometries/point.hpp
	src/geometries/polygon.hpp
	src/geometries/polygonal_cylinder.hpp
    src/geometries/vector.hpp
    src/det.hpp
    src/settings_parser.hpp
    src/CSGPrinterCircles.hpp
   # src/tactoid.hpp
   # src/geometries/polygonal_cylinder_in_the_shell.hpp
    src/percolation/percolation_checker.cpp)

set (SOURCES
    src/det.cpp 
    src/geometries/line_segment.cpp
   # src/tactoid.cpp
    src/geometries/plane.cpp
    src/geometries/point.cpp
    src/geometries/polygon.cpp
    src/geometries/polygonal_cylinder.cpp
    src/geometries/vector.cpp
    src/settings_parser.cpp
    src/CSGPrinterCircles.cpp
   # src/geometries/polygonal_cylinder_in_the_shell.cpp
    src/percolation/percolation_checker.cpp
   # just one of these!
   # src/mains/main_exfoliated.cpp
   # src/mains/main_exfoliated_interface.cpp)
   # src/mains/main_exfoliated_shell.cpp)
    src/mains/main_exfoliated_interface_percolation.cpp)
 
add_executable (${PROJECT} ${HEADERS} ${SOURCES})

set_target_properties(${PROJECT} PROPERTIES COMPILE_FLAGS "-std=c++14 -g -lm")
