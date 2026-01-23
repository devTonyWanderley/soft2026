cmake_minimum_required(VERSION 3.19)
project(criaTipos LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(criaTipos
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ../Banco/revTipos.h
    ../Banco/revTipos.cpp
)

target_link_libraries(criaTipos
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS criaTipos
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET criaTipos
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

# Adicionando a biblioteca Eigen
include_directories("C:/Tony/Soft/soft2026/eigen-5.0.0")

install(SCRIPT ${deploy_script})
