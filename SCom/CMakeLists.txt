cmake_minimum_required(VERSION 3.19)
project(SCom LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools SerialPort)

qt_standard_project_setup()

qt_add_executable(SCom
    WIN32 MACOSX_BUNDLE
    main.cpp
    serial.cpp
    serial.h
    serial.ui
)

qt_add_translations(
    TARGETS SCom
    TS_FILES SCom_pt_BR.ts
)

target_link_libraries(SCom
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::SerialPort
)

include(GNUInstallDirs)

install(TARGETS SCom
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET SCom
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
