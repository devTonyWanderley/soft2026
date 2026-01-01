cmake_minimum_required(VERSION 3.19)
project(mdComSerial LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools SerialPort)

qt_standard_project_setup()

qt_add_executable(mdComSerial
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

qt_add_translations(
    TARGETS mdComSerial
    TS_FILES mdComSerial_pt_BR.ts
)

target_link_libraries(mdComSerial
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::SerialPort
)

include(GNUInstallDirs)

install(TARGETS mdComSerial
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET mdComSerial
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
