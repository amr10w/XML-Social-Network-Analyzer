add_library(CLI STATIC
    CLICommands.h
    CLICommands.cpp
)

target_link_libraries(CLI
    PRIVATE 
        core
        Qt6::Core Qt6::Widgets
)

target_include_directories(CLI
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../core
     
)