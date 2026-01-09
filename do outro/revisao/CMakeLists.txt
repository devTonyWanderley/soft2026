cmake_minimum_required(VERSION 3.19)
project(revisao LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools SerialPort)

qt_standard_project_setup()

qt_add_executable(revisao
    WIN32 MACOSX_BUNDLE
    main.cpp
    revjaneiro.cpp
    revjaneiro.h
    revjaneiro.ui
    ../Banco2/tipos.h
    ../Banco2/tipos.cpp
)

qt_add_translations(
    TARGETS revisao
    TS_FILES revisao_pt_BR.ts
)

target_link_libraries(revisao
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::SerialPort
)

include(GNUInstallDirs)

install(TARGETS revisao
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET revisao
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
