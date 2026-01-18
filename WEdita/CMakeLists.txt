cmake_minimum_required(VERSION 3.19)
project(WEdita LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

qt_add_executable(WEdita
    WIN32 MACOSX_BUNDLE
    main.cpp
    edita.cpp
    edita.h
    edita.ui
    ../BancoDxf/dxfparse.h
    ../BancoDxf/dxfparse.cpp
)

qt_add_translations(
    TARGETS WEdita
    TS_FILES WEdita_pt_BR.ts
)

target_link_libraries(WEdita
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS WEdita
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET WEdita
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
