cmake_minimum_required(VERSION 3.19)
project(QTop LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools SerialPort)

qt_standard_project_setup()

qt_add_executable(QTop
    WIN32 MACOSX_BUNDLE
    main.cpp
    campo.cpp
    campo.h
    campo.ui
    ../Banco/cfgcom.ui
    ../Banco/cfgcom.h
    ../Banco/cfgcom.cpp
)

qt_add_translations(
    TARGETS QTop
    TS_FILES QTop_pt_BR.ts
)

target_link_libraries(QTop
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::SerialPort
)

include(GNUInstallDirs)

install(TARGETS QTop
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET QTop
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
