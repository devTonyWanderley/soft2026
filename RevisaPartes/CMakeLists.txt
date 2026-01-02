cmake_minimum_required(VERSION 3.19)
project(RevisaPartes LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

qt_add_executable(RevisaPartes
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ../Banco1/geral.h
    ../Banco1/geral.cpp
    ../Banco1/Tipos/ponto.h ../Banco1/Tipos/ponto.cpp
    ../Banco1/ed/glista.h ../Banco1/ed/gno.h
)

qt_add_translations(
    TARGETS RevisaPartes
    TS_FILES RevisaPartes_pt_BR.ts
)

target_link_libraries(RevisaPartes
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS RevisaPartes
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET RevisaPartes
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
