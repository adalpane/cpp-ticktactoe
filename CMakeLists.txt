cmake_minimum_required(VERSION 3.24)
project(cpp_tictactoe)

set(CMAKE_CXX_STANDARD 20)

add_executable(cpp_tictactoe main.cpp match.cpp play.cpp strategy1.cpp types.h match.h play.h strategy1.h strategy2.h strategy2.cpp montecarlo.cpp)
