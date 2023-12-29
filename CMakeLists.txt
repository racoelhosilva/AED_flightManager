cmake_minimum_required(VERSION 3.26)
project(AED2324_PRJ2_G15)

set(CMAKE_CXX_STANDARD 17)

add_executable(AED2324_PRJ2_G15
        main.cpp
        src/Graph.h
        src/Interface.cpp
        src/Interface.h
        src/Manager.cpp
        src/Manager.h
        src/Airport.cpp
        src/Airport.h
        src/Airline.cpp
        src/Airline.h
        src/Coordinate.cpp
        src/Coordinate.h
)
