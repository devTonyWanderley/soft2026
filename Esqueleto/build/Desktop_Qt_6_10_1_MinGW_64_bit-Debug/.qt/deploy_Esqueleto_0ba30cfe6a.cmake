include("C:/Tony/Soft/soft2026/Esqueleto/build/Desktop_Qt_6_10_1_MinGW_64_bit-Debug/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Esqueleto-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase;qtserialport")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "C:/Tony/Soft/soft2026/Esqueleto/build/Desktop_Qt_6_10_1_MinGW_64_bit-Debug/Esqueleto.exe"
    GENERATE_QT_CONF
)
