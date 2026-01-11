cmake_minimum_required(VERSION 3.19)
project(CSerial LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools SerialPort)

qt_standard_project_setup()

qt_add_executable(CSerial
    WIN32 MACOSX_BUNDLE
    main.cpp
    serial.cpp
    serial.h
    serial.ui
)

qt_add_translations(
    TARGETS CSerial
    TS_FILES CSerial_pt_BR.ts
)

target_link_libraries(CSerial
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::SerialPort
)

include(GNUInstallDirs)

install(TARGETS CSerial
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET CSerial
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
