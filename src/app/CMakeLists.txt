add_library(app STATIC
    main.cpp
)

target_link_libraries(app
    PRIVATE 
        UI 
        core
        CLI
        Qt6::Core Qt6::Widgets
)

target_include_directories(app
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../CLI 
    
)