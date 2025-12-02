cmake_minimum_required(VERSION 3.16)

project(XNet LANGUAGES CXX)


find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)


qt_standard_project_setup()

add_subdirectory(src/UI)
add_subdirectory(src/app)
add_subdirectory(src/CLI)
add_subdirectory(src/core)
add_subdirectory(src/UI/pages)

qt_add_executable(XNet
   
    
    
)
set_target_properties(XNet PROPERTIES OUTPUT_NAME "xml_editor")


target_link_libraries(XNet
    PRIVATE
        app
        UI
        core
        CLI
        Qt::Core
        Qt::Widgets
)


include(GNUInstallDirs)


install(TARGETS XNet
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET XNet
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})