set(CMAKE_AUTORCC ON)

add_library(UI_Pages STATIC
    WelcomePage.cpp
    WelcomePage.h
    WelcomePage.ui
    Aboutpage.cpp
    Aboutpage.h
    Aboutpage.ui
    VerificationPage.h
    VerificationPage.cpp
    VerificationPage.ui
    resources.qrc
   
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