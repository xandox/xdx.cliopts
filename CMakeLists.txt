cmake_minimum_required(VERSION 3.10)

project(xdx.cliopts CXX)

include($ENV{XDX_CMAKE_ROOT}/xdx.cmake)

xdx_project(
    NAME xdx.cliopts
    TYPE static
    NAMESPCE xdx::cliopts
    INCLUDE
        details/from_string.hpp
        argv.hpp
        argument.hpp
        flag.hpp
        options.hpp
        parser.hpp
        cliopts.hpp
    SOURCES
        cliopts.cpp
        parser.cpp
    TESTS
        parser.tests.cpp
        options.tests.cpp
        cliopts.tests.cpp
)
