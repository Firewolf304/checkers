cmake_minimum_required(VERSION 3.28)
project(checkers)

set(CMAKE_CXX_STANDARD 26)
option(ENABLE_BOOST_LOG "Enable Boost Logging" ON)
if(ENABLE_BOOST_LOG)
    message(STATUS "Boost Logging is enabled")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lboost_log -lboost_iostreams ")
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME OFF)
    set(Boost_INCLUDE_DIRS /usr/include/boost)
    find_package(Boost 1.86.0 REQUIRED COMPONENTS log log_setup system thread chrono filesystem atomic iostreams)
    include_directories(${Boost_INCLUDE_DIRS})
endif()
find_package(PkgConfig REQUIRED)
find_package(ftxui CONFIG REQUIRED)

add_executable(checkers main.cpp
        include/figures.hpp
        include/board.hpp
        include/include.hpp
        include/checkers.hpp
        include/local.hpp
)
if(ENABLE_BOOST_LOG)
    target_compile_definitions(checkers PRIVATE ENABLE_BOOST_LOG)
    if(Boost_FOUND)
        target_link_libraries(checkers PUBLIC ${Boost_LIBRARIES})
    else()
        message(FATAL_ERROR "Boost libraries not found!")
    endif()
endif()
target_link_libraries(checkers PRIVATE ftxui::dom ftxui::screen ftxui::component)