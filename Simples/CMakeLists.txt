cmake_minimum_required(VERSION 3.19)
project(Simples LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools SerialPort)

qt_standard_project_setup()

qt_add_executable(Simples
    WIN32 MACOSX_BUNDLE
    main.cpp
    scivil.cpp
    scivil.h
    scivil.ui
    ../Banco/forms/comserial.h
    ../Banco/forms/comserial.ui
    ../Banco/forms/comserial.cpp
    ../Banco1/geral.h
    ../Banco1/geral.cpp
    ../Banco1/Tipos/ponto.h
    ../Banco1/Tipos/ponto.cpp
    ../Banco1/ed/gno.h
    ../Banco1/ed/glista.h
    ../Banco1/edsup.h
    ../Banco1/edsup.cpp
    ../Banco1/Tipos/aresta.h
    ../Banco1/Tipos/aresta.cpp
    ../Banco1/Tipos/face.h
    ../Banco1/Tipos/face.cpp
    ../Banco1/superficie.h
    ../Banco1/superficie.cpp
)

qt_add_translations(
    TARGETS Simples
    TS_FILES Simples_pt_BR.ts
)

target_link_libraries(Simples
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::SerialPort
)

include(GNUInstallDirs)

install(TARGETS Simples
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Simples
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
