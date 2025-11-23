cmake_minimum_required(VERSION 3.16)

project(XNet LANGUAGES CXX)


find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)


qt_standard_project_setup()

set(GRAPHVIZ_DIR "C:/Program Files/Graphviz") 


qt_add_executable(XNet

main.cpp
mainwindow.cpp
mainwindow.h
mainwindow.ui
disjointsets.h
graph.h
graph2img.h
)
file(GLOB GVC_LIBS "${GRAPHVIZ_DIR}/lib/*.lib")

include_directories("${GRAPHVIZ_DIR}/include/graphviz")

target_link_libraries(XNet
    PRIVATE
        Qt::Core
        Qt::Widgets
        ${GVC_LIBS}
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