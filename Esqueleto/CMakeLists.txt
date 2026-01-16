cmake_minimum_required(VERSION 3.19)
project(Esqueleto LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools SerialPort)

qt_standard_project_setup()

qt_add_executable(Esqueleto
    WIN32 MACOSX_BUNDLE
    main.cpp
    campo.cpp
    campo.h
    campo.ui
    ../Banco3/cfgcom.ui
    ../Banco3/cfgcom.h
    ../Banco3/cfgcom.cpp
)

qt_add_translations(
    TARGETS Esqueleto
    TS_FILES Esqueleto_pt_BR.ts
)

target_link_libraries(Esqueleto
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::SerialPort
)

include(GNUInstallDirs)

install(TARGETS Esqueleto
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Esqueleto
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
