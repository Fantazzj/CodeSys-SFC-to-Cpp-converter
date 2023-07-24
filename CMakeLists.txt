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

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(QT_NO_DEBUG_OUTPUT)
    add_compile_definitions(QT_NO_INFO_OUTPUT)
endif ()

add_compile_definitions(SEPARATOR="------")

add_executable(CodeSys-SFC-to-Cpp-converter
        main.cpp
        src/SFCConverter.cpp
        src/SFCConverter.hpp
        src/Converter.cpp
        src/Converter.hpp
        src/VarsConverter.cpp
        src/VarsConverter.hpp
        src/GeneralConverter.cpp
        src/GeneralConverter.hpp
        res/icons.rc
        )

target_link_libraries(CodeSys-SFC-to-Cpp-converter Qt${QT_VERSION_MAJOR}::Core)

install(TARGETS CodeSys-SFC-to-Cpp-converter
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION .
        )

qt_generate_deploy_script(TARGET CodeSys-SFC-to-Cpp-converter
        OUTPUT_SCRIPT deploy_script
        CONTENT "
            qt_deploy_runtime_dependencies(
                EXECUTABLE CodeSys-SFC-to-Cpp-converter.exe
                BIN_DIR .
                PLUGINS_DIR .
                NO_TRANSLATIONS
                )"
        )
install(SCRIPT ${deploy_script})
