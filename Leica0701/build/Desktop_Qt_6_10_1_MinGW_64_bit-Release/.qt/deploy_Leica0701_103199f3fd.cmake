include("C:/Tony/Soft/soft2026/Leica0701/build/Desktop_Qt_6_10_1_MinGW_64_bit-Release/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Leica0701-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase;qtserialport")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "C:/Tony/Soft/soft2026/Leica0701/build/Desktop_Qt_6_10_1_MinGW_64_bit-Release/Leica0701.exe"
    GENERATE_QT_CONF
)
