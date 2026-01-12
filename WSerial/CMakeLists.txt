cmake_minimum_required(VERSION 3.19)
project(WSerial LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools SerialPort)

qt_standard_project_setup()

qt_add_executable(WSerial
    WIN32 MACOSX_BUNDLE
    main.cpp
    serial.cpp
    serial.h
    serial.ui
)

qt_add_translations(
    TARGETS WSerial
    TS_FILES WSerial_pt_BR.ts
)

target_link_libraries(WSerial
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::SerialPort
)

include(GNUInstallDirs)

install(TARGETS WSerial
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET WSerial
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
