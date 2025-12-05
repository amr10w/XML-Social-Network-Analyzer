set(CMAKE_AUTORCC ON)

add_library(UI_Pages STATIC
    WelcomePage.cpp
    WelcomePage.h
    WelcomePage.ui
    TeamPage.cpp      
    TeamPage.h       
    TeamPage.ui       
    resources.qrc
    ProcessingPage.cpp
    ProcessingPage.h
    ProcessingPage.ui
    )

set_property(TARGET UI_Pages PROPERTY AUTORCC ON)

target_link_libraries(UI_Pages
    PRIVATE
    Qt6::Core
    Qt6::Widgets
)

target_include_directories(UI_Pages
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)