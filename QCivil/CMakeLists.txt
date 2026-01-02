cmake_minimum_required(VERSION 3.19)
project(QCivil LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools SerialPort)

qt_standard_project_setup()

qt_add_executable(QCivil
    WIN32 MACOSX_BUNDLE
    main.cpp
    civil.cpp
    civil.h
    civil.ui
    ../Banco/geral.cpp ../Banco/geral.h
    ../Banco/ed/glista.h
    ../Banco/tipos/aresta.cpp ../Banco/tipos/aresta.h
    ../Banco/tipos/face.cpp ../Banco/tipos/face.h
    ../Banco/tipos/ecivil.cpp ../Banco/tipos/ecivil.h
    ../Banco/tipos/ponto.cpp ../Banco/tipos/ponto.h
    ../Banco/forms/comserial.h ../Banco/forms/comserial.ui ../Banco/forms/comserial.cpp
)

qt_add_translations(
    TARGETS QCivil
    TS_FILES QCivil_pt_BR.ts
)

target_link_libraries(QCivil
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::SerialPort
)

include(GNUInstallDirs)

install(TARGETS QCivil
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET QCivil
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
