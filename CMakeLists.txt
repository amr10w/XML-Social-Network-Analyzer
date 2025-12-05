cmake_minimum_required(VERSION 3.16)

project(XNet LANGUAGES CXX)


find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)


qt_standard_project_setup()

# --- OPTIONAL DEPENDENCY: GRAPHVIZ ---
set(GRAPHVIZ_DIR "C:/Program Files/Graphviz")

# Check if Graphviz exists on this machine
if(EXISTS "${GRAPHVIZ_DIR}/bin")
    set(GRAPHVIZ_FOUND TRUE)
    message(STATUS "Graphviz found at ${GRAPHVIZ_DIR}. Features enabled.")
else()
    set(GRAPHVIZ_FOUND FALSE)
    message(WARNING "Graphviz NOT found. Building without Graphviz features.")
endif()

add_subdirectory(src/UI)
add_subdirectory(src/app)
add_subdirectory(src/CLI)
add_subdirectory(src/core)
add_subdirectory(src/UI/pages)

qt_add_executable(XNet
    
)

file(GLOB GVC_LIBS "${GRAPHVIZ_DIR}/lib/*.lib")

include_directories("${GRAPHVIZ_DIR}/include/graphviz")

set_target_properties(XNet PROPERTIES OUTPUT_NAME "xml_editor")


target_link_libraries(XNet
    PRIVATE
        app
        UI
        core
        CLI
        Qt::Core
        Qt::Widgets
        ${GVC_LIBS}
)


include(GNUInstallDirs)

# --- AUTO-COPY DLLS (ONLY IF FOUND) ---
if(GRAPHVIZ_FOUND)
    add_custom_command(TARGET XNet POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${GRAPHVIZ_DIR}/bin"
        "$<TARGET_FILE_DIR:XNet>"
        COMMENT "Graphviz found: Copying bin folder to output directory..."
    )
endif()

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