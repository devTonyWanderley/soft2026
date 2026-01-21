cmake_minimum_required(VERSION 3.19)
project(TrHor LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

qt_add_executable(TrHor
    WIN32 MACOSX_BUNDLE
    main.cpp
    trhor.cpp
    trhor.h
    trhor.ui
)

qt_add_translations(
    TARGETS TrHor
    TS_FILES TrHor_pt_BR.ts
)

target_link_libraries(TrHor
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS TrHor
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET TrHor
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
