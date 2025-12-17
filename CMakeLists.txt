cmake_minimum_required(VERSION 3.16)

project(iLoveXML LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Concurrent)

qt_standard_project_setup()

set(GRAPHVIZ_DIR "C:/Progra~1/Graphviz")
set(GRAPHVIZ_FOUND TRUE)

set(GRAPHVIZ_DLL_NAMES 
    "gvc.dll" 
    "cgraph.dll" 
    "cdt.dll" 
    "xdot.dll" 
    "pathplan.dll"
)

include_directories("${GRAPHVIZ_DIR}/include/graphviz")
file(GLOB GVC_LIBS "${GRAPHVIZ_DIR}/lib/*.lib")

add_subdirectory(src/UI)
add_subdirectory(src/app)
add_subdirectory(src/CLI)
add_subdirectory(src/core)
add_subdirectory(src/UI/pages)

qt_add_executable(ilovexml_gui 
    WIN32 
    src/app/GUI.cpp
    src/app/icon.rc
)

set_target_properties(ilovexml_gui PROPERTIES OUTPUT_NAME "ilovexml_gui")

target_link_libraries(ilovexml_gui
    PRIVATE
        app
        UI
        core
        CLI
        Qt::Core
        Qt::Widgets
        Qt::Concurrent
        ${GVC_LIBS}
)

qt_add_executable(ilovexml_cli 
    src/app/CLI.cpp
)

set_target_properties(ilovexml_cli PROPERTIES OUTPUT_NAME "ilovexml")

target_link_libraries(ilovexml_cli
    PRIVATE
        app
        UI
        core
        CLI
        Qt::Core
        Qt::Widgets
        ${GVC_LIBS}
)


foreach(DLL_NAME ${GRAPHVIZ_DLL_NAMES})
    add_custom_command(TARGET ilovexml_gui POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${GRAPHVIZ_DIR}/bin/${DLL_NAME}"
        "$<TARGET_FILE_DIR:ilovexml_gui>"
        COMMENT "Copying ${DLL_NAME} to GUI output..."
    )
    add_custom_command(TARGET ilovexml_cli POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${GRAPHVIZ_DIR}/bin/${DLL_NAME}"
        "$<TARGET_FILE_DIR:ilovexml_cli>"
        COMMENT "Copying ${DLL_NAME} to CLI output..."
    )
endforeach()

if(WIN32)
    # Attempt to find windeployqt from the Qt6 package location
    get_target_property(QT_QMAKE_EXECUTABLE Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QT_QMAKE_EXECUTABLE}" DIRECTORY)
    set(WINDEPLOYQT_EXE "${QT_BIN_DIR}/windeployqt.exe")

    add_custom_command(TARGET ilovexml_gui POST_BUILD
        COMMAND "${WINDEPLOYQT_EXE}" --dir "$<TARGET_FILE_DIR:ilovexml_gui>" "$<TARGET_FILE:ilovexml_gui>"
        COMMENT "Running windeployqt on ilovexml_gui..."
    )
endif()

include(GNUInstallDirs)

install(TARGETS ilovexml_gui ilovexml_cli
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET ilovexml_gui
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})