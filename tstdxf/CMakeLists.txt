cmake_minimum_required(VERSION 3.19)
project(tstdxf LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

qt_add_executable(tstdxf
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ../Banco1/ed/gno.h
    ../Banco1/ed/glista.h
    ../Banco1/dxf.h
    ../Banco1/dxf.cpp
)

qt_add_translations(
    TARGETS tstdxf
    TS_FILES tstdxf_pt_BR.ts
)

target_link_libraries(tstdxf
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS tstdxf
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET tstdxf
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
