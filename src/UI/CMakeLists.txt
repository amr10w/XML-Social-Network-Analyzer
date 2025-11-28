add_library(UI STATIC
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

target_link_libraries(UI
    
    PRIVATE core 
    Qt6::Core Qt6::Widgets
)

target_include_directories(UI
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../core
)