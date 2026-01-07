cmake_minimum_required(VERSION 3.19)
project(Leica0701 LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools SerialPort)

qt_standard_project_setup()

qt_add_executable(Leica0701
    WIN32 MACOSX_BUNDLE
    main.cpp
    leica.cpp
    leica.h
    leica.ui
    ../Banco1/geral.h
    ../Banco1/geral.cpp
)

qt_add_translations(
    TARGETS Leica0701
    TS_FILES Leica0701_pt_BR.ts
)

target_link_libraries(Leica0701
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::SerialPort
)

include(GNUInstallDirs)

install(TARGETS Leica0701
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Leica0701
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
