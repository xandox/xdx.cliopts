cmake_minimum_required(VERSION 3.10)

project(xdx.cliopts CXX)

include($ENV{XDX_CMAKE_ROOT}/xdx.cmake)

xdx_project(
    NAME xdx.cliopts
    TYPE static
    NAMESPCE xdx::cliopts
    INCLUDE
        details/from_string.hpp
        argument.hpp
        argv.hpp
        builder.hpp
        cliopts.hpp
        details
        error.hpp
        flag.hpp
        options.hpp
        printer.hpp
        programm.hpp
        subcommand.hpp
        tokenizer.hpp
    SOURCES
        error.cpp
        flag.cpp
        options.cpp
        printer.cpp
        tokenizer.cpp
        parser.cpp
    TESTS
        tokenizer.tests.cpp
        options.tests.cpp
        parser.tests.cpp
)
