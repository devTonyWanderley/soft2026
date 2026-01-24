cmake_minimum_required(VERSION 3.19)
project(Corredor LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

qt_add_executable(Corredor
    WIN32 MACOSX_BUNDLE
    main.cpp
    testador.cpp
    testador.h
    testador.ui
    ../Banco/revTipos.h
    ../Banco/revTipos.cpp
    ../Banco/importengine.h
    ../Banco/importengine.cpp
)

qt_add_translations(
    TARGETS Corredor
    TS_FILES Corredor_pt_BR.ts
)

target_link_libraries(Corredor
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS Corredor
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Corredor
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

include_directories("C:/2026/Soft/eigen-5.0.0")

install(SCRIPT ${deploy_script})
