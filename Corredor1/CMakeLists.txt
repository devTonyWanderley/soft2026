cmake_minimum_required(VERSION 3.19)
project(Corredor1 LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

qt_add_executable(Corredor1
    WIN32 MACOSX_BUNDLE
    main.cpp
    terrapleno.cpp
    terrapleno.h
    terrapleno.ui
    ../BancoCorredor/Alinhamento.cpp
    ../BancoCorredor/Engenharia.cpp
    ../BancoCorredor/GeometriaBase.cpp
    ../BancoCorredor/StorageProvider.cpp
    ../BancoCorredor/Topologia.cpp
)

qt_add_translations(
    TARGETS Corredor1
    TS_FILES Corredor1_pt_BR.ts
)

target_link_libraries(Corredor1
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS Corredor1
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Corredor1
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
include_directories("C:/Tony/Soft/soft2026/eigen-5.0.0")
include_directories("C:/Tony/Soft/soft2026/BancoCorredor")
install(SCRIPT ${deploy_script})
