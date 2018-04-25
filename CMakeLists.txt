cmake_minimum_required(VERSION 3.10)

include($ENV{XDX_CMAKE_ROOT}/xdx.cmake)

xdx_static_lib_begin(
    NAME xdx.cliopts
    NAMESPCE xdx::cliopts
)

xdx_project_add_headers(
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
)

xdx_project_add_sources(
    error.cpp
    flag.cpp
    options.cpp
    printer.cpp
    tokenizer.cpp
    parser.cpp
)

xdx_project_add_tests(
    tokenizer.tests.cpp
    options.tests.cpp
    parser.tests.cpp
)

xdx_static_lib_end()
