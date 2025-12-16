# We change this to INTERFACE because the source files (GUI.cpp / CLI.cpp)
# are now compiled directly by the root CMakeLists.txt.
# This target now serves only to share dependencies and include paths.
add_library(app INTERFACE)

target_link_libraries(app
    INTERFACE
        UI
        core
        CLI
        Qt::Core
        Qt::Widgets
)

target_include_directories(app
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../CLI
)