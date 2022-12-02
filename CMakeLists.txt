cmake_minimum_required(VERSION 3.14)

project(CodeSys-SFC-to-Cpp-converter LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(CodeSys-SFC-to-Cpp-converter
        main.cpp
        src/SFCConverter.cpp
        src/SFCConverter.hpp
        src/Converter.cpp
        src/Converter.hpp
        src/VarsConverter.cpp
        src/VarsConverter.hpp
        src/GeneralConverter.cpp
        src/GeneralConverter.hpp)
target_link_libraries(CodeSys-SFC-to-Cpp-converter Qt${QT_VERSION_MAJOR}::Core)

install(TARGETS CodeSys-SFC-to-Cpp-converter
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
